/*
 * Copyright (c) 2014 Przemysław Grzywacz <nexather@gmail.com>
 * This file is part of xbrzscale.
 *
 * xbrzscale is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_surface.h>
#include <cstdio>
#include <cstdlib>

#include "libxbrzscale.h"

//#include <cstdio>
//#include <cstdint>
//#include "SDL.h"
//#include "SDL_image.h"
//#include "xbrz/xbrz.h"

/*
void displayImage(SDL_Surface* surface, const char* message) {
	fprintf(stdout, "Display: %s\n", message);
	SDL_Surface* video = SDL_SetVideoMode(640, 480, 0, 0);
	if (!video) {
		fprintf(stderr, "Failed to open display: %s\n", SDL_GetError());
		return;
	}

	SDL_Event e;
	bool done = false;
	bool expose = true;
	while(!done) {
		while(SDL_PollEvent(&e)) {
			if (e.type == SDL_KEYUP) {
				done = true;
			} else if (e.type == SDL_VIDEOEXPOSE) {
				expose = true;
			}
		}

		if (expose) {
			printf("expose\n");
			expose = false;
			SDL_Rect dest;

			dest.x = 640 / 2 - surface->w / 2;
			dest.y = 480 / 2 - surface->h / 2;
			SDL_BlitSurface(surface, NULL, video, &dest);
			SDL_Flip(video);
		}
	}
}
*/

int main(int argc, char* argv[]) {
	if (argc != 4) {
		fprintf(stderr, "usage: xbrzscale scale_factor input_image output_image\n");
		fprintf(stderr, "scale_factor can be between 2 and 6\n");
		return 1;
	}
	
	int scale = atoi(argv[1]);
	char* in_file = argv[2];
	char* out_file = argv[3];
	
	if (scale < 2 || scale > 6) {
		fprintf(stderr, "scale_factor must be between 2 and 6 (inclusive), got %i\n", scale);
		return 1;
	}
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
		return 1;
	}
	
	SDL_Surface* src_img = IMG_Load(in_file);
  if (!src_img) {
    fprintf(stderr, "Failed to load source image '%s': %s\n", in_file, IMG_GetError());
    return 1;
  }

//  displayImage(src_img, "Source image");

  libxbrzscale::setEnableOutput(true);
	SDL_Surface* dst_img = libxbrzscale::scale(src_img,scale);
	if(!dst_img)return 1;

	//  displayImage(dst_img, "Image after color conversion");

  IMG_SavePNG(dst_img,out_file);

  SDL_FreeSurface(dst_img);

  SDL_Quit();
  return 0;
}
