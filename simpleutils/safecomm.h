// *************************************************************************
//  This file is part of the SimpleUtils Example Module by Steaphan Greene
//
//  Copyright 2005-2014 Steaphan Greene <steaphan@gmail.com>
//
//  SimpleUtils is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 3 of the License, or
//  (at your option) any later version.
//
//  SimpleUtils is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SimpleUtils (see the file named "COPYING");
//  If not, see <http://www.gnu.org/licenses/>.
// *************************************************************************

#ifndef SAFECOMM_H
#define SAFECOMM_H

#include <cstring>

#include "SDL.h"

template <class Tp>
static void ReadNBO(Tp &var, const void *buf) {
  std::memcpy(&var, buf, sizeof(Tp));
#if SDL_BYTEORDER == SDL_LITTLE_ENDIAN
  union DTp { Tp v; Uint8 b[sizeof(Tp)]; } data;
  int i1=0, i2=sizeof(Tp)-1;
  while(i1 < i2) {
    swap(((DTp *)(&var))->b[i1], ((DTp *)(&var))->b[i2]);
    ++i1; --i2;
    }
#endif
  };

template <class Tp>
static void WriteNBO(const Tp &var, void *buf) {
  std::memcpy(buf, &var, sizeof(Tp));
#if SDL_BYTEORDER == SDL_LITTLE_ENDIAN
  union DTp { Tp v; Uint8 b[sizeof(Tp)]; } data;
  int i1=0, i2=sizeof(Tp)-1;
  while(i1 < i2) {
    swap(((DTp *)(buf))->b[i1], ((DTp *)(buf))->b[i2]);
    ++i1; --i2;
    }
#endif
  };

#endif // SAFECOMM_H
