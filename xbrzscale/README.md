xBRZ upscaling commandline tool
===============================

Copyright (c) 2014 Przemysław Grzywacz <nexather@gmail.com>

This file is part of xbrzscale.

xbrzscale is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.



Overview
--------

This tool allows you to scale your graphics with xBRZ algorithm, see http://en.wikipedia.org/wiki/Image_scaling#xbr_family

This is an example of what xBRZ can do:

![Example of xBRZ](https://upload.wikimedia.org/wikipedia/commons/6/6c/HQx-xBRZ-comparison.png)


External code
-------------

The following external code is included in this repository:

* https://sourceforge.net/projects/xbrz/files/xBRZ/ - xBRZ implementation

Dependencies
------------

The following dependencies are needed to compile xbrzscale:

* libsdl2-dev
* libsdl2-image-dev

Under OSX they can be installed using macports

* port install libsdl2_image
* port install libsdl2

Some additional libraries are needed. I'm sure you'll figure it out.

If you need SDL1.2 support, check sdl1.2 git branch.


Compiling
---------

Just run `make` and you should end up with a binary called `xbrzscale`.

The makefile will probably work only on linux and OSX, but it is simple enough to be ported to other platforms.


Usage
-----

	`xbrztool scale_factor input_image output_image`

* `scale_factor` - Controls how much your image should be scaled. It should be an integer between 2 and 5 (inclusive).
* `input_image` - Input image is the filename of the image you want to scale. Image format can be anything that SDL_image supports.
* `output_image` - Filename where the scaled image should be saved. The only supported format is PNG!

Please note I only tested the scaling on 32bit RGBA PNGs, I have no idea if this will work with 8bit indexed images.




