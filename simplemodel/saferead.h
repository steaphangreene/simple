// *************************************************************************
//  This file is part of the SimpleModel Example Module by Steaphan Greene
//
//  Copyright 2005 Steaphan Greene <stea@cs.binghamton.edu>
//
//  SimpleModel is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  SimpleModel is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SimpleModel (see the file named "COPYING");
//  if not, write to the the Free Software Foundation, Inc.,
//  59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
// *************************************************************************

#include "SDL.h"

template <class Tp>
static void freadLE(Tp &var, FILE *fl) {
  union { Tp v; char c[sizeof(Tp)]; } data;
  fread(&data.v, 1, sizeof(Tp), fl);
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  int i1=0, i2=sizeof(Tp)-1;
  while(i1 < i2) {
    swap(data.c[i1], data.c[i2]);
    ++i1; --i2;
    }
#endif
  var = data.v;
  };

template <class Tp>
static void freadBE(Tp &var, FILE *fl) {
  union { Tp v; char c[sizeof(Tp)]; } data;
  fread(&data.v, 1, sizeof(Tp), fl);
#if SDL_BYTEORDER == SDL_LITTLE_ENDIAN
  int i1=0, i2=sizeof(Tp)-1;
  while(i1 < i2) {
    swap(data.c[i1], data.c[i2]);
    ++i1; --i2;
    }
#endif
  var = data.v;
  };
