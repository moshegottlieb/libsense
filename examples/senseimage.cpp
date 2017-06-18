#include <iostream>
#include <stdexcept>
#include <string>
#include <wand/MagickWand.h>
#include <sense/fb.hpp>
#include <unistd.h>

void throwError(MagickWand* wand) throw (std::runtime_error){
    ExceptionType severity;
    char* description = MagickGetException(wand,&severity);
    std::string err(description);
    MagickRelinquishMemory(description);
    throw std::runtime_error(err);
}

int main(int argc,char** argv){
    MagickWand* image_wand = nullptr;
    PixelWand* pixel = nullptr;
    int ret = 0;
    try {
        if (argc != 2){
            throw std::runtime_error("Usage: " + std::string(argv[0]) +" {image file}");
        }
        MagickWandGenesis();
        MagickWand* image_wand = NewMagickWand();
        MagickBooleanType status;

        status = MagickReadImage(image_wand,argv[1]);
        if (status == MagickFalse){
            throwError(image_wand);
        }
        status = MagickResizeImage(image_wand,SENSE_BITMAP_WIDTH,SENSE_BITMAP_HEIGHT,BoxFilter,1);
        if (status == MagickFalse){
            throwError(image_wand);
        }
        sense::bitmap memory_bmp(true);
        pixel = NewPixelWand();
        for (uint8_t y=0;y<SENSE_BITMAP_HEIGHT;++y)
        for (uint8_t x=0;x<SENSE_BITMAP_WIDTH;++x){
            MagickGetImagePixelColor(image_wand,x,y,pixel);
            double r = PixelGetRed(pixel);
            double g = PixelGetGreen(pixel);
            double b = PixelGetBlue(pixel);
            memory_bmp.setPixel(x,y,sense_make_color_rgbf(r,g,b));
        }
        sense::bitmap framebuffer;
        framebuffer = memory_bmp;
    } catch (const std::exception& e){
        std::cerr << e.what() << std::endl;
        ret = -1;
    }
    if (image_wand){
        DestroyMagickWand(image_wand);
        if (pixel){
            DestroyPixelWand(pixel);
        }
        MagickWandTerminus();
    }
    return ret;
}