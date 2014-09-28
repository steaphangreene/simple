// *************************************************************************
//  This file is part of the SimpleTexture Example Module by Steaphan Greene
//
//  Copyright 2005-2014 Steaphan Greene <steaphan@gmail.com>
//
//  SimpleTexture is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 3 of the License, or
//  (at your option) any later version.
//
//  SimpleTexture is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SimpleTexture (see the file named "COPYING");
//  If not, see <http://www.gnu.org/licenses/>.
//
// *************************************************************************

#ifndef	SAFEREAD_H
#define	SAFEREAD_H

#include "SDL.h"

template <class Tp>
static int freadLE(Tp &var, SDL_RWops *fl) {
  union { Tp v; Uint8 b[sizeof(Tp)]; } data;
  int ret=SDL_RWread(fl, &data.v, sizeof(Tp), 1);
  if(ret == 1) {
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    int i1=0, i2=sizeof(Tp)-1;
    while(i1 < i2) {
      swap(data.b[i1], data.b[i2]);
      ++i1; --i2;
      }
#endif
    var = data.v;
    }
  return ret;
  };

template <class Tp>
static int freadBE(Tp &var, SDL_RWops *fl) {
  union { Tp v; Uint8 b[sizeof(Tp)]; } data;
  int ret=SDL_RWread(fl, &data.v, sizeof(Tp), 1);
  if(ret == 1) {
#if SDL_BYTEORDER == SDL_LITTLE_ENDIAN
    int i1=0, i2=sizeof(Tp)-1;
    while(i1 < i2) {
      swap(data.b[i1], data.b[i2]);
      ++i1; --i2;
      }
#endif
    var = data.v;
    }
  return ret;
  };

#endif	//SAFEREAD_H

