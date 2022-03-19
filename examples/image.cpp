#include "image.hpp"
#include <iostream>
#include <wand/MagickWand.h>
#include <unistd.h>

void throwError(MagickWand* wand) throw (std::runtime_error){
    ExceptionType severity;
    char* description = MagickGetException(wand,&severity);
    std::string err(description);
    MagickRelinquishMemory(description);
    throw std::runtime_error(err);
}

void cleanup(MagickWand* image_wand,PixelWand* pixel){
    if (image_wand){
        DestroyMagickWand(image_wand);
        if (pixel){
            DestroyPixelWand(pixel);
        }
        MagickWandTerminus();
    }
}

sense::bitmap drawImage(const std::string& filename) throw (std::exception) {
    auto image = loadImage(filename);
    sense::bitmap framebuffer;
    framebuffer = image;
    return image;
}
sense::bitmap loadImage(const std::string& filename) throw (std::exception){
    MagickWand* image_wand = nullptr;
    PixelWand* pixel = nullptr;
    int ret = 0;
    try {
        MagickWandGenesis();
        MagickWand* image_wand = NewMagickWand();
        MagickBooleanType status;

        status = MagickReadImage(image_wand,filename.c_str());
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
        cleanup(image_wand,pixel);
        return memory_bmp;
    } catch (const std::exception& e){
        cleanup(image_wand,pixel);
        throw;
    }
}