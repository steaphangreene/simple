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

#include "stt.h"

ST_Texturator::ST_Texturator() {
  }

ST_Texturator::~ST_Texturator() {
  }

SDL_Surface *ST_Texturator::Generate(SDL_Surface *surf,
	const Uint32 xsize, const Uint32 ysize, const SDL_Color &col
	) {
  return BuildTexture(surf, xsize, ysize, col);
  }

SDL_Surface *ST_Texturator::BuildTexture(SDL_Surface *surf,
	const Uint32 xsize, const Uint32 ysize, const SDL_Color &col
	) {
  fprintf(stderr, "WARNING: Abstract class ST_Texturator called!\n");
  return NULL;
  }

Uint8_32 st_col_u32b1 = {{ 0xFF, 0x00, 0x00, 0x00 }};
Uint8_32 st_col_u32b2 = {{ 0x00, 0xFF, 0x00, 0x00 }};
Uint8_32 st_col_u32b3 = {{ 0x00, 0x00, 0xFF, 0x00 }};
Uint8_32 st_col_u32b4 = {{ 0x00, 0x00, 0x00, 0xFF }};
