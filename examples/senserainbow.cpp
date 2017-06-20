/**
Copied from the sense hat python example: rainbow.py
Try to measure CPU using:

top -p `ps -e | grep senserainbow | cut -f1 -d" "` (I got max 1.3% cpu, 0.2 mem on RPi3)

and for the python version:

top -p `ps -e | grep rainbow.py | cut -f1 -d" "` (I got 46.3% cpu, 1.6% mem on the same RPi 3)

*/

#include <sense/fb.hpp>
#include <unistd.h>


uint8_t pixels[64][3] = {
    {255, 0, 0}, {255, 0, 0}, {255, 87, 0}, {255, 196, 0}, {205, 255, 0}, {95, 255, 0}, {0, 255, 13}, {0, 255, 122},
    {255, 0, 0}, {255, 96, 0}, {255, 205, 0}, {196, 255, 0}, {87, 255, 0}, {0, 255, 22}, {0, 255, 131}, {0, 255, 240},
    {255, 105, 0}, {255, 214, 0}, {187, 255, 0}, {78, 255, 0}, {0, 255, 30}, {0, 255, 140}, {0, 255, 248}, {0, 152, 255},
    {255, 223, 0}, {178, 255, 0}, {70, 255, 0}, {0, 255, 40}, {0, 255, 148}, {0, 253, 255}, {0, 144, 255}, {0, 34, 255},
    {170, 255, 0}, {61, 255, 0}, {0, 255, 48}, {0, 255, 157}, {0, 243, 255}, {0, 134, 255}, {0, 26, 255}, {83, 0, 255},
    {52, 255, 0}, {0, 255, 57}, {0, 255, 166}, {0, 235, 255}, {0, 126, 255}, {0, 17, 255}, {92, 0, 255}, {201, 0, 255},
    {0, 255, 66}, {0, 255, 174}, {0, 226, 255}, {0, 117, 255}, {0, 8, 255}, {100, 0, 255}, {210, 0, 255}, {255, 0, 192},
    {0, 255, 183}, {0, 217, 255}, {0, 109, 255}, {0, 0, 255}, {110, 0, 255}, {218, 0, 255}, {255, 0, 183}, {255, 0, 74}
};

void next_color(uint8_t* pix){
    uint8_t r = pix[0];
    uint8_t g = pix[1];
    uint8_t b = pix[2];
    if (r == 255 && g < 255 && b == 0) g += 1;
    if (g == 255 && r > 0 && b == 0) r -= 1;
    if (g == 255 && b < 255 && r == 0) b += 1;
    if (b == 255 && g > 0 && r == 0) g -= 1;
    if (b == 255 && r < 255 && g == 0) r += 1;
    if (r == 255 && b > 0 && g == 0) b -= 1;
    pix[0] = r;
    pix[1] = g;
    pix[2] = b;
}


int main(){
    sense::bitmap mem(true);
    sense::bitmap fb;
    while (true){
        for (uint8_t y=0;y<SENSE_BITMAP_HEIGHT;++y){
            for (uint8_t x=0;x<SENSE_BITMAP_WIDTH;++x){
                int idx = y*SENSE_BITMAP_WIDTH + x;
                next_color(pixels[idx]);
                mem.setPixel(x,y,sense::color(pixels[idx][0],pixels[idx][1],pixels[idx][2]));
            }
        }
        fb = mem;
        usleep(200);
    }


    return 0;
}
