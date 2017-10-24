#include "fb.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <unistd.h>
 

static int _sense_err = 0;

void _sense_bitmap_fb_free(sense_bitmap_t bmp);
void _sense_bitmap_mem_free(sense_bitmap_t bmp);

const char* sense_strerror(int err){
    switch (err){
        case EACCES:
            return "Cannot memory map framebuffer device, make sure you have permission for the device (usually /dev/fb1)";
        case ENODEV:
            return "Can't find Sense HAT framebuffer";
        default:
            return strerror(err);
    }

}

struct sense_bitmap {
    uint16_t* bmp;
    void (*dtor)(sense_bitmap_t);    
};

void _sense_bitmap_fb_free(sense_bitmap_t bmp){
    munmap(bmp->bmp,SENSE_BUFFER_SIZE);
}
void _sense_bitmap_mem_free(sense_bitmap_t bmp){
    free(bmp->bmp);
}

void sense_free_bitmap(sense_bitmap_t bmp){
    if (bmp){
        bmp->dtor(bmp);
        free(bmp);
    }
}

uint16_t* sense_bitmap_get_buffer(sense_bitmap_t bmp){
    return bmp->bmp;
}

sense_bitmap_t sense_alloc_fb(){
    _sense_err = ENODEV;
    int i=0;
    int fd;
    sense_bitmap_t ret = NULL;
    do {
        char fbname[10];
        sprintf(fbname,"/dev/fb%d",i++);
        fd = open(fbname,O_RDWR);
        if (fd < 0){
            if (errno == ENOENT)
                break; // else keep on trying
        } else {
            struct fb_fix_screeninfo fbinfo;
            if (ioctl(fd, FBIOGET_FSCREENINFO, &fbinfo) == 0){
                if (strcmp(fbinfo.id, "RPi-Sense FB") == 0){
                    ret = (sense_bitmap_t)malloc(sizeof(struct sense_bitmap));
                    ret->bmp = (uint16_t*)mmap(NULL,SENSE_BUFFER_SIZE ,PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
                    ret->dtor = _sense_bitmap_fb_free;
                    if (!ret->bmp){
                        sense_free_bitmap(ret);
                        ret = NULL;
                        _sense_err = EACCES;
                    } else {
                        _sense_err = 0;
                    }
                }
            }
            close(fd);
        }
    } while (ret == NULL && i<100); // if you have more than 99 framebuffer devices on your raspberry pi, this library is not for you.
    return ret;
}

sense_bitmap_t sense_alloc_bitmap(){
    sense_bitmap_t ret = (sense_bitmap_t)malloc(sizeof(struct sense_bitmap));
    ret->bmp = (uint16_t*)malloc(SENSE_BUFFER_SIZE);
    ret->dtor = _sense_bitmap_mem_free;
    return ret;
}

void sense_bitmap_cpy(sense_bitmap_t dst,const sense_bitmap_t src){
    memcpy(dst->bmp,src->bmp,SENSE_BUFFER_SIZE);
}

sense_color_t sense_make_color_rgbf(double r,double g,double b){
    r*=31.0; r+= 0.5;
    g*=63.0; g+= 0.5;
    b*=31.0; b+= 0.5;
    return sense_make_color_rgb565(r,g,b);
}

sense_color_t sense_make_color_rgb(uint8_t r,uint8_t g,uint8_t b){
    r = (float)r / 255.0 * 31.0 + 0.5;
    g = (float)g / 255.0 * 63.0 + 0.5;
    b = (float)b / 255.0 * 31.0 + 0.5;
    return sense_make_color_rgb565(r,g,b);
}

sense_color_t sense_make_color_rgb565(uint8_t r,uint8_t g,uint8_t b){
    sense_color_t color = r << 11 | g << 5 | b;
    return color;
}

int sense_errno(){
    return _sense_err;
}
int sense_bitmap_set_pixel(sense_bitmap_t bmp,uint8_t x,uint8_t y,sense_color_t color){
    if (x > SENSE_BITMAP_WIDTH || y > SENSE_BITMAP_HEIGHT){
        _sense_err = EINVAL;
        return -1;
    }
    sense_bitmap_get_buffer(bmp)[x + y * SENSE_BITMAP_WIDTH] = color;
    _sense_err = 0;
    return 0;
}

void sense_bitmap_paint(sense_bitmap_t bmp,sense_color_t color){
    // do a buffered copy, works much better for framebuffers, a bit redundant for memory based bitmap but who cares.
    uint16_t buffer[SENSE_PIXELS];
    int i;
    // I could check if the color can be encoded in one byte (left byte equals right byte) for a more performant version of this assignment, but I guess this would work just fine anyway
    for (i=0;i<SENSE_PIXELS;++i) buffer[i] = color;
    memcpy(bmp->bmp,buffer,SENSE_BUFFER_SIZE);
}
