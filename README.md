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

Depending on your setup, you may want to allow the current user access to the devices, otherwise you'd need root access to run apps linked to libsense.<br/>
In Raspbian:<br/>
`sudo usermod -aG input,video,i2c {your username here}`
Or change the permissions (not recommended) of:<br/>
`/dev/fb1`, `/dev/i2c-1`, `/dev/input/event0`<br/>
Currently, since libsense only works with the LED matrix (as a framebuffer), there are no dependencies.<br/>
However, future versions will require `i2c`, in Raspbian:<br/>
`sudo apt-get install libi2c-dev` and also: `sudo raspi-config` --> advanced options --> enable i2c<br/>
<br/>
That said, the build process is dead simple:<br/><br/>
`make`<br/>
`sudo make install`<br/>
<br/>
Then from your C files:<br/><br/>
`#include <sense/fb.h>`<br/><br/>
Or from your C++ files (can use the C version too, of course):<br/><br/>
`#include <sense/fb.hpp>`<br/><br/>
Link with `-lsense`.<br/>

## Documentation
[doxygen](http://doxygen.org/) docs available as well, to build just type:<br/>
`make doc`<br/>
HTML Documentation will be available in the `docs/html` folder.<br/>
<br/>
## Examples
See the `examples/` directory.<br/>
Type:<br/>
`make` to build examples, `sensepaint`,`senseimage`,`senserainbow` and `sensetext`.<br/>
Built binaries are placed in the `build` directory.<br/>
Both `sensepaint` and `senseimage` require the [Image Wand](https://www.imagemagick.org/script/magick-wand.php) library (`sudo apt-get install libmagickwand-dev` on Raspbian and _probably_ Ubuntu), it's a part of the [Image Magick](https://www.imagemagick.org/) project.<br/>
Also, if you plan to use clang instead of gcc, make sure to install the openmp libraries (required by Image Magick):<br/>
`sudo apt install libomp-dev`<br/>
Once built, try something like:<br/>
`build/sensepaint green` or `build/sensepaint "#ffbb00"` to paint all LEDs.<br/>
`build/senseimage sense_logo.png` will draw the logo on the LED matrix.<br/>
The later will resize any image to 8x8 and draw it on the matrix.<br/>
Don't expect your DSLR camera photos to look very good using this method.<br/>
How about writing something?<br/>
Luckily, Jared Sanson has a nice collection of 6x8 fonts exactly for this purpose (well, I suppose not _exactly_ for this purpose).<br/>
It's available [here](http://jared.geek.nz/2014/jan/custom-fonts-for-microcontrollers) and it's used by the `sensetext` example.<br/>
Just type something like: `build/sensetext "hello world!"` and enjoy the magic of text printing out on your 8x8 LED matrix.<br/>
I do not recommend on reading [Ulysses](https://en.wikipedia.org/wiki/Ulysses_(novel)) this way.  
  
New example **OpenWeatherMap**   
--
Shows the temperature on the sensehat LED matrix.  
Supports scrolling for long figures (-10 and colder) and color coding, becomes bluer as it gets colder, and more red as it's hotter.  
You want it to be nice and green :-).  
It currently takes the temperature value from a small ESP8266 project I have (will github it soon) that simply reads the temperature upon request to it's webserver, but there's also a provider interface, just edit the code and use a different REST API provider to fetch the weather on your own.  
A darksky API sample is available, but you can't sign up for a new API key, after they were bought by Apple, but it should get you started on using a different provider.  
Just use your own URL and change the code that fetches the data from the JSON response.  
...oh yeh, the temp is in celsius, I live in europe.  
To switch to Fahrenheit, use `F = C * 1.8 + 32`.  
You may want to do it after it figures out the color coding, as the color coding thresholds are set to celsius.  
To install it as a systemd daeomn:
1. `make && sudo make install`
2. Inside `examples`, run `make`
3. Copy `build/openweathermap` to `/usr/local/bin/openweatheramp`.
4. Copy `openweathermap/openweathermap.service` to `/etc/systemd/system/`
5. Install the service: `sudo systemctl enable openweathermap`
6. Run it (it will start on boot otherwise): `sudo service openweathermap start`  

Why not use the built in sensehat temp sensor(s)? Because there's a board, LEDs and a CPU right next to it, and those generate a lot of heat.  
It may work reasonably in space, but not on earth :-)

<br/>
## C++
Writing shared libraries in C is easy.  
The rules for when to bump up the so name is simple and clear.  
When dealing with C++ - that's not the case.  
Sorry for all the warnings about specifying exception type, should port it to modern C++ at some point.   
The list of things that break binary compatibility is very long, and it's super easy to break.  
Since the C++ code is just a wrapper - I decided to include it as an inline header, that way the shared library doesn't have any C++ issues.<br/>

## Contributing to libsense

Yes please! see the [guideline](CONTRIBUTING.md) first.
