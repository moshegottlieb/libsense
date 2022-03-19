#include <sense/fb.hpp>
#include "image.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <stdexcept>
#include <iostream>
#include <algorithm> 
#include <curl/curl.h>

//#define CALLS_PER_DAY 1000
//#define SAMPLE_INTERVAL (24*3600 / (int)((double)CALLS_PER_DAY * 0.9))
#define SAMPLE_INTERVAL 1
#define SCROLL_DELAY 100
#define INVALID_TEMP -9000.0
#define LINE_LEN 1024

#pragma clang diagnostic push
//#pragma clang diagnostic ignored "-Wmultichar"
#include "nlohmann_json/json.hpp"
#pragma clang diagnostic pop


using json = nlohmann::json;

// Replace with any of the fonts in "fonts" directory
#include "fonts/pzim3x5_font.h"

//#include "fonts/BMSPA_font.h"
//#define CHAR_WIDTH 8

#define CHAR_WIDTH (sizeof(font) / 96)
#define CHAR_HEIGHT 8
#define SCROLL_DELAY 100
#define LETTER_SPACE 1
#define LETTER_WIDTH (CHAR_WIDTH + LETTER_SPACE)


enum EDownloadState {
    DOWNLOADED=0,
    ERROR=1,
    PENDING=2,
    CLEAR=3,
};

struct Download {
    Download():degrees(0){}
    std::string text;
    std::string icon;
    double degrees;
    bool success;
};

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream);
void draw(char c,int x,int y,sense_color_t color,sense_bitmap_t bitmap);
const unsigned char* character(char c);
void print(const char* text,EDownloadState state);
void download(Download& dl);

#define BUFFER_SIZE 16384

sense_color_t color = sense_make_color_rgb(0xff,0xff,0xff);

struct Buffer {
    Buffer(){
        len = 0;
        data[0] = 0;
    }
    char data[BUFFER_SIZE];
    size_t len;
};

int main(int argc,char** argv){
    Download d;
    sense::bitmap background(true);
    bool at_least_one_success = false;
    while (true){
        try {
            if (at_least_one_success){
                print(d.text.c_str(),EDownloadState::PENDING);
            }
            download(d);
            if (d.success){
                at_least_one_success = true;
            }
            if (d.icon.size()){
                try {
                    std::string path("/usr/local/share/darksky/");
                    path += d.icon + ".png";
                    background = loadImage(path);
                } catch (...){
                    background = sense::bitmap(true);
                }
            }
            print(d.text.c_str(),d.success ? EDownloadState::DOWNLOADED : EDownloadState::ERROR );
        } catch (const std::exception& e){
            print(e.what(),EDownloadState::ERROR);
        }
        usleep(1000*1000);
    }
    return 0;
}

const unsigned char* character(char c){
    c = c & 0x7F;
    if (c < ' ') {
        c = 0;
    } else {
        c -= ' ';
    }
    return font[c];
}

// code apdapted from http://jared.geek.nz/2014/jan/custom-fonts-for-microcontrollers
void draw(char c,int x,int y,sense_color_t color,sense_bitmap_t bitmap) {
    int i,j;
    const unsigned char* chr = character(c);
    for (j=0 + ((LETTER_SPACE+x)<0?(1+x)*-1:0); j<CHAR_WIDTH && (1+j+x) < SENSE_BITMAP_WIDTH; ++j) {
        for (i=0 + (y<0?y*-1:0); i<CHAR_HEIGHT && (i+y < SENSE_BITMAP_HEIGHT); ++i) {
            if (chr[j] & (1<<i)) {
                sense_bitmap_set_pixel(bitmap,LETTER_SPACE +j+x, i+y,color);
            }
        }
    }
}

void indicate(EDownloadState state,sense_bitmap_t bitmap){
    sense_color_t indicator;            
    switch (state){
        case EDownloadState::ERROR:
            indicator = sense_make_color_rgb(255,0,0);                    
            break;
        case EDownloadState::PENDING:
            //indicator = sense_make_color_rgb(100,100,100);
            //break;
        default:
            return; // don't indicate
    }
    sense_bitmap_set_pixel(bitmap,SENSE_BITMAP_WIDTH - 1,SENSE_BITMAP_HEIGHT - 1,indicator);
}

void print(const char* text,EDownloadState state){
    int len = strlen(text);
    auto buffer = sense::bitmap(true,true);
    auto fb = sense::bitmap();
    if (len <= 2){
        if (len == 1){
            draw(text[0],2,0,color,buffer);
        } else {
            draw(text[0],0,0,color,buffer);
            draw(text[1],4,0,color,buffer);
        }
        indicate(state,buffer);
        fb = buffer;
    } else {
        int idx;
        char* ttext = (char*)alloca(len + 2);
        strncpy(ttext,text,len);
        ttext[len] = ' ';
        ttext[len+1] = 0;
        len += 1;
        text = ttext;
        for (int x=0;x<len*LETTER_WIDTH;++x){
            sense_bitmap_paint(buffer,0);
            idx = x/LETTER_WIDTH;
            draw(text[idx],0 - x % LETTER_WIDTH,0,color,buffer);
            if ((idx + 1) < len){
                draw(text[idx+1],LETTER_WIDTH - x % LETTER_WIDTH,0,color,buffer);
            }
            indicate(state,buffer);
            sense_bitmap_cpy(fb,buffer);
            usleep(SCROLL_DELAY*1000);
        }
    }
    
}



class Provider {
public:
    virtual const char* url() const noexcept = 0;
    void parse(const char* data,Download& download){
        parsed = json::parse(data);
        if (parsed.empty()){
            throw std::runtime_error("Unable to parse json");
        }
        download.degrees = degrees();
    }
protected:
    virtual std::string icon() const{
        static std::string EMPTY = "";
        return EMPTY;
    } 
    virtual double degrees() const = 0;
    json parsed;
};

class ProviderWeatherStation : public Provider {
public:
    virtual const char* url() const noexcept {
        return "http://192.168.4.22/";
    }
protected:
    virtual double degrees() const{
        return round(parsed["temp"].get<double>());
    }
};

class ProviderDarkSky : public Provider {
    public:
        virtual const char* url() const noexcept {
            return "https://api.darksky.net/forecast/[Your key here]/[longitude],[latitude]?exclude=minutely,hourly,alerts,flags&units=si";
        }
    protected:
        virtual std::string icon() const{
            return parsed["currently"]["icon"].get<std::string>();
        }
        virtual double degrees() const{
            return round(parsed["currently"]["temperature"].get<double>());
        }
    };


void download(Download& download){
    static time_t last_sample = 0;
    static char sline[LINE_LEN];
    static size_t slen = 0;
    static double last_degrees = INVALID_TEMP;
    auto now_t = time(nullptr);
    if ((now_t - last_sample) >= SAMPLE_INTERVAL){
		try {
            auto provider = ProviderWeatherStation();
			CURL *curl;
			CURLcode res;
			curl = curl_easy_init();
			Buffer buffer;
			if(curl) {
                const char* url = provider.url();
				curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
				curl_easy_setopt(curl, CURLOPT_URL, url);
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
                curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
				res = curl_easy_perform(curl);
				if(res != CURLE_OK) {
					std::string error = curl_easy_strerror(res);
					curl_easy_cleanup(curl);
					throw std::runtime_error(error);
				}
				curl_easy_cleanup(curl);
			}
            provider.parse(buffer.data,download);
			last_sample = time(nullptr);
            download.success = true;
            last_degrees = download.degrees;
		} catch (const std::exception&){
            download.success = false;
            download.degrees = last_degrees;
		}
        if (!download.success && last_degrees == INVALID_TEMP){
            strcpy(sline,"--c");
			slen = strlen(sline);
			color = sense_make_color_rgb(155,255,246);
        } else {
            double r,g,b;
            slen = snprintf(sline,LINE_LEN,"%d",(int)download.degrees);
            double normalized = (std::min(std::max(-5.0,download.degrees),35.0) + 5.0) / 40.0;
            b = 1.0 - normalized;
            r = normalized;
            g = 1.0 - fabs(normalized - 0.5) * 2.0; 
            color = sense_make_color_rgbf(r,g,b);
        }
        download.text = sline;
	}
}


static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream){
    size_t len = size * nmemb;
    Buffer& buffer = *((Buffer*)stream);
    if ((buffer.len + len) < BUFFER_SIZE){
        memcpy(buffer.data + buffer.len,ptr,len);
        buffer.len += len;
        buffer.data[buffer.len] = 0;
        return len;
    } else {
        return 0;
    }
}
