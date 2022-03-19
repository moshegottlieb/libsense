#ifndef _SENSE_FB_HPP
#define _SENSE_FB_HPP
#include "fb.h"
#include <stdexcept>
#include <cstring>

/**
@file
@author Moshe Gottlieb
*/

namespace sense {

    /**
    @class bitmap
    @desc Bitmap wrapper for framebuffer or memory bitmap
    @see sense_alloc_bitmap sense_alloc_fb
    */
    class bitmap {
    public:
        /**
        Create a bitmap (framebuffer or memory)
        @param memory When true, a memory bitmap is created, otherwise a framebuffer device is used.
        @throws std::runtime_error
        */
        bitmap(bool memory = false, bool clear = false) throw (std::runtime_error){
            _bmp = memory?sense_alloc_bitmap():sense_alloc_fb();
            if (!_bmp){
                throw std::runtime_error(sense_strerror(sense_errno()));
            }
            if (clear){
                sense_bitmap_paint(_bmp,0);
            }
        }




        /**
         Copy CTOR of a MEMORY bitmap
         @param 
         */
        bitmap(const bitmap& other){
            _bmp = sense_alloc_bitmap();
            *this = other;
        }
        /**
        Cleanup and relinquish resources
        */
        virtual ~bitmap() noexcept{
            sense_free_bitmap(_bmp);   
        }
        /**
        Set a pixel to a given color
        @see sense_color_t
        @see sense_bitmap_set_pixel
        @throws std::runtime_error
        */
        void setPixel(uint8_t x,uint8_t y,sense_color_t color) throw (std::runtime_error){
            if (sense_bitmap_set_pixel(_bmp,x,y,color) != 0){
                throw std::runtime_error(sense_strerror(sense_errno()));
            }
        }
        /**
        Paint a bitmap.<br/>
        This method works much faster than iterating the pixels and setting the color using `setPixel`.
        @param color The color to paint the bitmap with.
        @see sense_bitmap_paint
        */
        void paint(sense_color_t color) noexcept{
            sense_bitmap_paint(_bmp,color);
        }
        /**
        Copy contents of a bitmap to another bitmap.
        Can mix framebuffer and memory bitmaps.
        */
        bitmap& operator = (const bitmap& other) noexcept {
            memcpy(sense_bitmap_get_buffer(_bmp),sense_bitmap_get_buffer(const_cast<bitmap&>(other)),SENSE_BUFFER_SIZE);
            return *this;
        }
        /**
        Use this operator if you want to use the C API with this class.
        @return A bitmap handle.
        */
        operator sense_bitmap_t() noexcept{
            return _bmp;
        }
    private:
        sense_bitmap_t _bmp;
    };
    

    /**
    @class color
    @desc Color wrapper
    */
    class color {
    public:
        /**
        Create a color (as easy as `{0xff,0xff,0xff}` for white)
        @param r Red (0..255)
        @param g Green (0..255)
        @param b Blue (0..255)
        */
        color(uint8_t r,uint8_t g,uint8_t b) noexcept{
            _color = sense_make_color_rgb(r,g,b);
        }
        /**
        Create a color from an existing color
        @param c Sense color
        */
        color(sense_color_t c) noexcept
        :_color(c){

        }
        /**
        @return sense_color_t raw type (RGB565 16 bit color)
        */
        operator sense_color_t() const noexcept {
            return _color;
        }
    private:
        sense_color_t _color;
    };
    

}


#endif
