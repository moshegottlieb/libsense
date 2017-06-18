# libsense ![logo](https://raw.githubusercontent.com/moshegottlieb/libsense/master/examples/sense_logo.png)
### Sense HAT for the Raspberry Pi C/C++ API
I don't know about you guys, but I'm not much of a python guy.<br/>
While I can code in just about any programming language - I tend to stick to compiled languages.<br/>
Anyway, everything is written in C and wrapped in some other language binding. Right? that's the only way to make a cross-language project.

When I got my [Sense HAT](https://www.raspberrypi.org/products/sense-hat/) for my (4th? 5th? dunno) [Raspberry Pi](https://www.raspberrypi.org/) - I was surprised to see it only came with python API wrappers.<br/>
It even comes with an emulator which is very cool, however, it didn't feel right to me.<br/>
That's when libsense was born.<br/>
It currently covers the framebuffer only but I promise to add support for the gyroscope, accelerometer, magnetometer and temperature, barometric pressure and humidity sensors.<br/>
Find everything you need in `fb.h` or `fb.hpp` for the C++ flavor.<br/>
<br/>
## Install
(no dependencies, sweet!)<br/>
<br/>
`make`<br/>
`sudo make install`<br/>
<br/>
Then from your C files:<br/>
`#include <sense/fb.h>`<br/>
Or from your C++ files (can use the C version too, of course):<br/>
`#include <sense/fb.hpp>`<br/>
Link with `-lsense`.<br/>

## Documentation
[doxygen](http://doxygen.org/) docs available as well, to build just type:<br/>
`make doc`<br/>
HTML Documentation will be available in the `docs/html` folder.<br/>
<br/>
## Examples
See the `examples/` directory.<br/>
Type:<br/>
`make` to build both examples, `sensepaint` and `senseimage`.<br/>
Both examples require the [Image Magick](https://www.imagemagick.org/) [Image Wand](https://www.imagemagick.org/script/magick-wand.php) library (`sudo apt-get install libmagickwand-dev` on Raspbian and _probably_ Ubuntu).<br/>
Once built, try something like:<br/>
`./sensepaint green` or `./sensepaint "#ffbb00"` to paint all LEDs.<br/>
`./senseimage sense_logo.png` will draw the logo on the LED matrix.<br/>
The later will resize any image to 8x8 and draw it on the matrix.<br/>
Don't expect your DSLR camera photos to look very good using this method.<br/>
<br/>
## C++
Writing shared libraries in C is easy.<br/>
The rules for when to bump up the so name is simple and clear.<br/>
When dealing with C++ - that's not the case.<br/>
The list of things that break binary compatibility is very long, and it's super easy to break.<br/>
Since the C++ code is just a wrapper - I decided to include it as an inline header, that way the shared library doesn't have any C++ issues.<br/>

## Contributing to libsense

Yes please! see the [guideline](CONTRIBUTING.md) first.
