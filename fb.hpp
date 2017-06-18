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
        bitmap(bool memory = false) throw (std::runtime_error){
            bmp = memory?sense_alloc_bitmap():sense_alloc_fb();
            if (!bmp){
                throw std::runtime_error(sense_strerror(sense_errno()));
            }
        }
        /**
        Cleanup and relinquish resources
        */
        virtual ~bitmap() noexcept{
            sense_free_bitmap(bmp);   
        }
        /**
        Set a pixel to a given color
        @see sense_color_t
        @see sense_bitmap_set_pixel
        @throws std::runtime_error
        */
        void setPixel(uint8_t x,uint8_t y,sense_color_t color) throw (std::runtime_error){
            if (sense_bitmap_set_pixel(bmp,x,y,color) != 0){
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
            sense_bitmap_paint(bmp,color);
        }
        /**
        Copy contents of a bitmap to another bitmap.
        Can mix framebuffer and memory bitmaps.
        */
        bitmap& operator = (const bitmap& other) noexcept {
            memcpy(sense_bitmap_get_buffer(bmp),sense_bitmap_get_buffer(const_cast<bitmap&>(other)),SENSE_BUFFER_SIZE);
            return *this;
        }
        /**
        Use this operator if you want to use the C API with this class.
        @return A bitmap handle.
        */
        operator sense_bitmap_t() noexcept{
            return bmp;
        }
    private:
        sense_bitmap_t bmp;
    };

}


#endif