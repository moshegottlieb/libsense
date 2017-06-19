#include <sense/fb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// Replace with any of the fonts in "fonts" directory
#include "fonts/5x5_font.h"

#define CHAR_WIDTH 6
#define CHAR_HEIGHT 8
#define SCROLL_DELAY 50

void draw(char c,int x,int y,sense_color_t color,sense_bitmap_t bitmap);
const unsigned char* character(char c);

int main(int argc,char** argv){
    if (argc != 2){
        fprintf(stderr,"Usage: %s {text}\n",argv[0]);
        exit(1);
    }
    sense_bitmap_t fb = sense_alloc_fb();
    if (!fb){
        fprintf(stderr,"Could not allocate framebuffer: %s\n",sense_strerror(sense_errno()));
        exit(1);
    }
    sense_color_t color = sense_make_color_rgb(255,255,255);
    sense_bitmap_t buffer = sense_alloc_bitmap();
    char* word = argv[1];
    int idx;
    int len = strlen(word);
    for (int x=0;x<len*SENSE_BITMAP_WIDTH;++x){
        sense_bitmap_paint(buffer,0);
        idx = x/SENSE_BITMAP_WIDTH;
        draw(word[idx],0 - x % SENSE_BITMAP_WIDTH,0,color,buffer);
        if ((idx + 1) < len){
            draw(word[idx+1],SENSE_BITMAP_WIDTH - x % SENSE_BITMAP_WIDTH,0,color,buffer);
        }
        sense_bitmap_cpy(fb,buffer);
        usleep(SCROLL_DELAY*1000);
    }
    // clear display
    sense_bitmap_paint(fb,0);
    sense_free_bitmap(fb);
    sense_free_bitmap(buffer);
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
    for (j=0 + ((1+x)<0?(1+x)*-1:0); j<CHAR_WIDTH && (1+j+x) < SENSE_BITMAP_WIDTH; ++j) {
        for (i=0 + (y<0?y*-1:0); i<CHAR_HEIGHT && (i+y < SENSE_BITMAP_HEIGHT); ++i) {
            if (chr[j] & (1<<i)) {
                sense_bitmap_set_pixel(bitmap,1+j+x, i+y,color);
            }
        }
    }
}
