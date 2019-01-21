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

#ifndef XBRZLIB_RELATIVEPATHSDL
#include <SDL_stdinc.h>
#else
#include "SDL_stdinc.h"
#endif

struct SDL_Surface;

class libxbrzscale
{
 public:
  static Uint32 SDL_GetPixel(SDL_Surface *surface, int x, int y);
  static void SDL_PutPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
  static SDL_Surface* createARGBSurface(int w, int h);
  static SDL_Surface* scale(SDL_Surface* src_img,int scale);
  static SDL_Surface* scale(SDL_Surface* dst_imgCache,SDL_Surface* src_img,int scale);
  static void setEnableOutput(bool b){bEnableOutput=b;};
  static void setDebugMsg(bool b){bDbgMsg=b;};
  static void setFreeSurfaceAfterScale(bool bInputSurface,bool bOutputSurface){
    bFreeInputSurfaceAfterScale=bInputSurface;
    bFreeOutputSurfaceAfterScale=bOutputSurface;
  };
  static void setUseCache(bool b){bUseCache=b;};
  static uint32_t* surfaceToUint32(bool bIn, SDL_Surface* img);
  static void uint32toSurface(uint32_t* dest, SDL_Surface* dst_img);
  static bool isDbgMsg(){return bDbgMsg;}
 private:
  static bool bEnableOutput;
  static bool bDbgMsg;
  static bool bUseCache;
  static bool bFreeInputSurfaceAfterScale;
  static bool bFreeOutputSurfaceAfterScale;
};
