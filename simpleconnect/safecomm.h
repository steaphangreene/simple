// *************************************************************************
//  This file is part of the SimpleConnect Example Module by Steaphan Greene
//
//  Copyright 2005-2006 Steaphan Greene <stea@cs.binghamton.edu>
//
//  SimpleConnect is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  SimpleConnect is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SimpleConnect (see the file named "COPYING");
//  if not, write to the the Free Software Foundation, Inc.,
//  59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
// *************************************************************************

#include "SDL.h"

template <class Tp>
static void ReadNBO(Tp &var, void *buf) {
  var = *((Tp*)(buf));
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
static void WriteNBO(Tp &var, void *buf) {
  *((Tp*)(buf)) = var;
#if SDL_BYTEORDER == SDL_LITTLE_ENDIAN
  union DTp { Tp v; Uint8 b[sizeof(Tp)]; } data;
  int i1=0, i2=sizeof(Tp)-1;
  while(i1 < i2) {
    swap(((DTp *)(buf))->b[i1], ((DTp *)(buf))->b[i2]);
    ++i1; --i2;
    }
#endif
  };
