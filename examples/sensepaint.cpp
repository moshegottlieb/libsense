#include <iostream>
#include <stdexcept>
#include <string>
#include <wand/MagickWand.h>
#include <sense/fb.hpp>

void throwError(MagickWand* wand) throw (std::runtime_error){
    ExceptionType severity;
    char* description = MagickGetException(wand,&severity);
    std::string err(description);
    MagickRelinquishMemory(description);
    throw std::runtime_error(err);
}



int main(int argc,char** argv){
    PixelWand* pixel = nullptr;
    int ret = 0;
    try {
        if (argc != 2){
            throw std::runtime_error("Usage: " + std::string(argv[0]) +" color name (black, red, #ffaa00...)");
        }
        MagickWandGenesis();
        pixel = NewPixelWand();
        PixelSetColor(pixel,argv[1]);
        double r = PixelGetRed(pixel);
        double g = PixelGetGreen(pixel);
        double b = PixelGetBlue(pixel);
        sense::bitmap framebuffer;
        framebuffer.paint(sense_make_color_rgbf(r,g,b));
    } catch (const std::exception& e){
        std::cerr << e.what() << std::endl;
        ret = -1;
    }
    if (pixel){
        DestroyPixelWand(pixel);
        MagickWandTerminus();
    }
    return ret;
}