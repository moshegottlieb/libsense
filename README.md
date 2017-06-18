# libsense
Sense HAT for the Raspberry Pi C/C++ API

I don't know about you guys, but I'm not much of a python guy.
While I can code in just about any programming language - I tend to stick to compiled languages.
Anyway, everything is written in C and wrapped in some other language binding? right? that's the only way to make a cross-language project.
When I got my [Sense HAT](https://www.raspberrypi.org/products/sense-hat/) for my (4th? 5th? dunno) [Raspberry Pi](https://www.raspberrypi.org/) - I was surprised to see it only came with python API wrappers.
It even comes with an emulator which is very cool, however, it didn't feel right to me.
That's when libsense was born.
It currently covers the framebuffer only but I promise to add support for the gyroscope, accelerometer, magnetometer and temperature, barometric pressure and humidity sensors.
Find everything you need in `fb.h` or `fb.hpp` for the C++ flavor.

## Install
(no dependencies, sweet!)

`make`
`sudo make install`

Then from your C files:
`#include <sense/fb.h>`
Or from your C++ files (can use the C version too, of course):
`#include <sense/fb.hpp>`

## Documentation

To see the [doxygen](http://doxygen.org/) docs available as well, to build just type:
`make doc`
HTML Documentation will be available in the `docs/html` folder.

## Examples

See the `examples/` directory.
Type:
`make` to build both examples, `sensepaint` and `senseimage`.
Both examples require the [Image Magick](https://www.imagemagick.org/) [Image Wand](https://www.imagemagick.org/script/magick-wand.php) library.
Once built, try something like:
`./sensepaint green` or `./sensepaint "#ffbb00"` to paint all LEDs.
`./senseimage sense_logo.png` will draw the logo on the LED matrix.
The later will resize any image to 8x8 and draw it on the matrix.
Don't expect your DSLR camera photos to look very good using this method.

## C++
Writing shared libraries in C is easy.
The rules for when to bump up the so name is simple and clear.
When dealing with C++ - that's not the case.
The list of things that break binary compatibility is very long, and it's super easy to break.
Since the C++ code is just a wrapper - I decided to include it as an inline header, that way the shared library doesn't have any C++ issues.

