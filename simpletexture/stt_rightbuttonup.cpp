// *************************************************************************
//  This file is part of the SimpleTexture Example Module by Steaphan Greene
//
//  Copyright 2005-2008 Steaphan Greene <stea@cs.binghamton.edu>
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

#include "stt_rightbuttonup.h"

STT_RightButton_Up::STT_RightButton_Up(const SDL_Color &txtcol) {
  textcol = txtcol;
  }

SDL_Surface *STT_RightButton_Up::BuildTexture(SDL_Surface *surf,
	const Uint32 xsize, const Uint32 ysize, const SDL_Color &col) {
  SDL_Surface *ret = BuildInternal(surf, xsize, ysize,
	base_col(col), light_col(col), dark_col(col));

  Uint32 width = xsize/16;
  if(width > ysize/16) width = ysize/16;
  if(width < 1) width = 1;

  SDL_Rect point = { width * 2, width * 2, width, width };

  for(Uint32 pos = ysize/4; pos <= ysize / 2; ++pos) {
    point.x = xsize/2;
    point.y = pos;
    point.w = (xsize * pos / ysize) - ysize/4;
    SDL_FillRect(ret, &point, SDL_MapRGB(
	ret->format, textcol.r, textcol.g, textcol.b
	));
    point.y = ysize - pos;
    SDL_FillRect(ret, &point, SDL_MapRGB(
	ret->format, textcol.r, textcol.g, textcol.b
	));
    }

  return ret;
  }
