#include <sense/fb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Replace with any of the fonts in "fonts" directory
#include "fonts/5x5_font.h"

#define CHAR_WIDTH 6
#define CHAR_HEIGHT 8
#define DELAY 400

void draw(char c,sense_color_t color,sense_bitmap_t bitmap);


int main(int argc,char** argv){
    sense_bitmap_t fb = sense_alloc_fb();
    if (!fb){
        fprintf(stderr,"Could not allocate framebuffer: %s\n",sense_strerror(sense_errno()));
        exit(1);
    }
    sense_color_t color = sense_make_color_rgb(255,255,255);
    for (int i=1;i<argc;++i){
        char* word = argv[i];
        sense_bitmap_paint(fb,0);
        if (i > 1) usleep(DELAY*1000); // add a short delay between wordsmake
        while (*word){
            sense_bitmap_paint(fb,0);
            usleep(1000*50); // short delay for double letters (heLLo)
            draw(*word,color,fb);
            usleep(1000*DELAY);
            ++word;
        }
    }
    // clear display
    sense_bitmap_paint(fb,0);
    sense_free_bitmap(fb);
    return 0;
}

// code apdapted from http://jared.geek.nz/2014/jan/custom-fonts-for-microcontrollers
void draw(char c,sense_color_t color,sense_bitmap_t bitmap) {
    uint8_t i,j;

    // Convert the character to an index
    c = c & 0x7F;
    if (c < ' ') {
        c = 0;
    } else {
        c -= ' ';
    }
    const unsigned char* chr = font[c];
    // Draw pixels
    for (j=0; j<CHAR_WIDTH; j++) {
        for (i=0; i<CHAR_HEIGHT; i++) {
            if (chr[j] & (1<<i)) {
                sense_bitmap_set_pixel(bitmap,1+j, i,color);
            }
        }
    }
}
