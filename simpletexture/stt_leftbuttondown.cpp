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

#include "stt_leftbuttondown.h"

STT_LeftButton_Down::STT_LeftButton_Down(const SDL_Color &txtcol) {
  textcol = txtcol;
  }

SDL_Surface *STT_LeftButton_Down::BuildTexture(SDL_Surface *surf,
	const Uint32 xsize, const Uint32 ysize, const SDL_Color &col) {
  SDL_Surface *ret = BuildInternal(surf, xsize, ysize,
	base_col(col), dark_col(col), light_col(col));

  int width = xsize/16;
  if(width > int(ysize/16)) width = ysize/16;
  if(width < 1) width = 1;

  SDL_Rect point = { width * 2, width * 2, width, width };

  for(Uint32 pos = ysize/4; pos <= ysize / 2; ++pos) {
    point.x = ((xsize * 3) / 4) - (xsize * pos / ysize);
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
