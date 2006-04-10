// *************************************************************************
//  This file is part of the SimpleTexture Example Module by Steaphan Greene
//
//  Copyright 2005-2006 Steaphan Greene <stea@cs.binghamton.edu>
//
//  SimpleTexture is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  SimpleTexture is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SimpleTexture (see the file named "COPYING");
//  if not, write to the the Free Software Foundation, Inc.,
//  59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
// *************************************************************************

#ifndef	SAFEREAD_H
#define	SAFEREAD_H

#include "SDL.h"

template <class Tp>
static void freadLE(Tp &var, SDL_RWops *fl) {
  union { Tp v; Uint8 b[sizeof(Tp)]; } data;
  SDL_RWread(fl, &data.v, 1, sizeof(Tp));
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  int i1=0, i2=sizeof(Tp)-1;
  while(i1 < i2) {
    swap(data.b[i1], data.b[i2]);
    ++i1; --i2;
    }
#endif
  var = data.v;
  };

template <class Tp>
static void freadBE(Tp &var, SDL_RWops *fl) {
  union { Tp v; Uint8 b[sizeof(Tp)]; } data;
  SDL_RWread(fl, &data.v, 1, sizeof(Tp));
#if SDL_BYTEORDER == SDL_LITTLE_ENDIAN
  int i1=0, i2=sizeof(Tp)-1;
  while(i1 < i2) {
    swap(data.b[i1], data.b[i2]);
    ++i1; --i2;
    }
#endif
  var = data.v;
  };

#endif	//SAFEREAD_H

