#include <iostream>
#include "image.hpp"

int main(int argc,char** argv){
    int ret = 0;
    try {
        if (argc != 2){
            throw std::runtime_error("Usage: " + std::string(argv[0]) +" {image file}");
        }
        auto image = drawImage(argv[1]);
    } catch (const std::exception& e){
        std::cerr << e.what() << std::endl;
        ret = -1;
    }
    return ret;
}