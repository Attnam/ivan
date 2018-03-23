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

#include "libxbrzscale.h"

#include <SDL2/SDL_endian.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_surface.h>
#include <cstdio>

#include "xbrz/xbrz.h"

//#include <cstdio>
//#include <cstdint>
//#include "SDL.h"
//#include "SDL_image.h"
//#include "xbrz/xbrz.h"

bool libxbrzscale::bEnableOutput=false;

Uint32 libxbrzscale::SDL_GetPixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}

void libxbrzscale::SDL_PutPixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}


SDL_Surface* libxbrzscale::createARGBSurface(int w, int h) {
  return SDL_CreateRGBSurface(0, w, h, 32, 0xff0000U, 0xff00U, 0xffU, 0xff000000U);
}

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

uint32_t* libxbrzscale::surfaceToUint32(SDL_Surface* img){
  uint32_t *data = new uint32_t[img->w * img->h];

  int x, y, offset=0;
  Uint8 r, g, b, a;
  for(y = 0; y < img->h; y++) {
    for(x = 0; x < img->w; x++) {
      uint32_t c = SDL_GetPixel(img, x, y);
      SDL_GetRGBA(c, img->format, &r, &g, &b, &a);
      //fprintf(stdout, "%02x%02x%02x%02x ", a, r, g, b);
      data[offset] = (
            (uint32_t(a) << 24)
            | (uint32_t(r) << 16)
            | (uint32_t(g) << 8)
            | (uint32_t(b))
          );
//      fprintf(stdout, "%08x ", in_data[offset]);
      offset++;
    }
//    fprintf(stdout, "\n");
  }

  return data;
}

void libxbrzscale::uint32toSurface(uint32_t* ui32src, SDL_Surface* dst_img){
  int x, y, offset=0;
  Uint8 r, g, b, a;
  for(y = 0; y < dst_img->h; y++) {
    for(x = 0; x < dst_img->w; x++) {
      a = ((ui32src[offset] >> 24) & 0xff);
      r = (ui32src[offset] >> 16) & 0xff;
      g = (ui32src[offset] >> 8) & 0xff;
      b = (ui32src[offset]) & 0xff;
      uint32_t c = SDL_MapRGBA(dst_img->format, r, g, b, a);
//      fprintf(stdout, "%02x%02x%02x%02x ", a, r, g, b);
//      fprintf(stdout, "%08x ", c);
//      fprintf(stdout, "%08x ", dest[offset]);
      SDL_PutPixel(dst_img, x, y, c);
//      SDL_PutPixel(dst_img, x, y, (a || r || g || b) ? 0xffffffffU : 0);
      offset++;
    }
  }
}

SDL_Surface* libxbrzscale::scale(SDL_Surface* src_img, int scale){
  int src_width = src_img->w;
  int src_height = src_img->h;
  int dst_width = src_width * scale;
  int dst_height = src_height * scale;

  uint32_t *in_data = surfaceToUint32(src_img);
  SDL_FreeSurface(src_img);

  if(bEnableOutput)printf("Scaling image...\n");
  uint32_t* dest = new uint32_t[dst_width * dst_height];

  xbrz::scale(scale, in_data, dest, src_width, src_height, xbrz::ColorFormat::ARGB);
  delete [] in_data;

  if(bEnableOutput)printf("Saving image...\n");
  SDL_Surface* dst_img = createARGBSurface(dst_width, dst_height);
  if (!dst_img) {
    delete [] dest;
    if(bEnableOutput)fprintf(stderr, "Failed to create SDL surface: %s\n", SDL_GetError());
    return NULL;
  }

  uint32toSurface(dest,dst_img);

  return dst_img;
}
