#ifndef _SENSE_FB_H
#define _SENSE_FB_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
* @file
* @author Moshe Gottlieb, Sharkfood
*/


/**
* Bitmap (memory or framebuffer) opaque structure
*/
typedef struct sense_bitmap *sense_bitmap_t;
/**
Color.<br/>
Sense HAT colors are represented in RGB 565 (16 bit) - in practice, values lower than `0x2e` are not visible
*/
typedef uint16_t sense_color_t;

/**
Allocate a framebuffer device.<br/>
If the device cannot be used - the return value is `NULL`, and the error code from `sense_errno` should be examined.<br/>
Allocated bitmaps must be freed using `sense_free_bitmap`.
* @see sense_alloc_bitmap
* @see sense_free_bitmap
* @see sense_errno
* @return A framebuffer bitmap or `NULL` if could not allocate bitmap
*/
sense_bitmap_t sense_alloc_fb();
/**
Allocate a memory bitmap<br/>
Allocated bitmaps must be freed using `sense_free_bitmap`.
@return A memory bitmap
@see sense_alloc_fb
@see sense_free_bitmap
*/
sense_bitmap_t sense_alloc_bitmap();
/**
* Copy a bitmap to another bitmap. If `source == target` the behaviour is undefined
*/
void sense_copy_bitmap(sense_bitmap_t target,const sense_bitmap_t source);
/**
* Free a bitmap
@param bitmap The bitmap to free
*/
void sense_free_bitmap(sense_bitmap_t bitmap);

/**
Get the internal buffer of a bitmap for direct manipulation.<br/>
The bitmap buffer is an array of size `SENSE_PIXELS` of `uint16_t` RGB 565 values.
@param  bitmap The bitmap
@see SENSE_PIXELS
@return Internal buffer (`SENSE_PIXELS` long)
*/
uint16_t* sense_bitmap_get_buffer(sense_bitmap_t bitmap);

/**
Copy bitmap.<br/>
Can be used to copy framebuffer to memory and vice versa.<br/>
The behavior is undefined if dst == src
@param dst Destination bitmap
@param src Source bitmap
*/
void sense_bitmap_cpy(sense_bitmap_t dst,const sense_bitmap_t src);
/**
* Make a color from a floating point RGB represnetation
* @param r Red value `(0..1)`
* @param g Green value `(0..1)`
* @param b Blue value `(0..1)`
* @return RGB 565 color
*/
sense_color_t sense_make_color_rgbf(double r,double g,double b);
/**
* Make a color from RGB values
* @param r Red value `(0..255)`
* @param g Green value `(0..255)`
* @param b Blue value `(0..255)`
* @return RGB 565 color
*/
sense_color_t sense_make_color_rgb(uint8_t r,uint8_t g,uint8_t b);
/**
* Make a color from RGB values (565)
* @param r Red value `(0..31)`
* @param g Green value `(0..63)`
* @param b Blue value `(0..31)`
* @return RGB 565 color
*/
sense_color_t sense_make_color_rgb565(uint8_t r,uint8_t g,uint8_t b);

/**
* Last error in sense library.<br/>
* This value is only updated when calling a function that specifies it may set an error.
* @return The last error in the sense library
*/
int sense_errno();

/**
* Error code to string
* @return String representation of the error code. May fall back to C lib `strerror` if the error is not specific to the sense library
*/
const char* sense_strerror(int err);

/**
* Set a pixel in a bitmap
* @param bitmap The bitmap to draw on
* @param x X coordinates `(0..SENSE_BITMAP_WIDTH-1)`
* @param y Y coordinates `(0..SENSE_BITMAP_HEIGHT-1)`
* @param color The color to paint
* @see sense_errno
* @return `0` on success, otherwise check `sense_errno`
*/
int sense_bitmap_set_pixel(sense_bitmap_t bitmap,uint8_t x,uint8_t y,sense_color_t color);

/**
* Paint a bitmap with a given color
* @param bitmap Bitmap to paint
* @param color The fill color for the bitmap
*/
void sense_bitmap_paint(sense_bitmap_t bitmap,sense_color_t color);


/**
* Bitmap pixel width
*/
#define SENSE_BITMAP_WIDTH 8
/**
* Bitmap pixel height
*/
#define SENSE_BITMAP_HEIGHT 8
/**
* Number of pixels in the bitmap
*/
#define SENSE_PIXELS (SENSE_BITMAP_WIDTH * SENSE_BITMAP_HEIGHT)
/**
* Buffer size in bytes for the bitmap
*/
#define SENSE_BUFFER_SIZE (SENSE_PIXELS * sizeof(uint16_t))

#ifdef __cplusplus
}
#endif



#endif