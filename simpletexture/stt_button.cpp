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

#define COL_SCALOR      (12.0 / 16.0)

#include "stt_button.h"

SDL_Surface *STT_Button::BuildTexture(SDL_Surface *surf,
	const Uint32 xsize, const Uint32 ysize, const SDL_Color &col
	) {
  fprintf(stderr, "WARNING: Abstract class STT_Button called!\n");
  return NULL;
  }

SDL_Color STT_Button::base_col(const SDL_Color &c) {
  SDL_Color ret = {0, 0, 0, 0};
  ret.r = base_col(c.r);
  ret.g = base_col(c.g);
  ret.b = base_col(c.b);
  return ret;
  }

SDL_Color STT_Button::light_col(const SDL_Color &c) {
  SDL_Color ret = {0, 0, 0, 0};
  ret.r = light_col(c.r);
  ret.g = light_col(c.g);
  ret.b = light_col(c.b);
  return ret;
  }

SDL_Color STT_Button::dark_col(const SDL_Color &c) {
  SDL_Color ret = {0, 0, 0, 0};
  ret.r = dark_col(c.r);
  ret.g = dark_col(c.g);
  ret.b = dark_col(c.b);
  return ret;
  }

Uint8 STT_Button::base_col(const Uint8 c) {
  float col = (float)c;
  col -= 127.5;
  col *= COL_SCALOR;
  col += 127.5 + 0.5;
  return Uint8(col);
  }

Uint8 STT_Button::light_col(const Uint8 c) {
  float col = (float)c;
  col -= 255.0;
  col *= COL_SCALOR;
  col += 255.0 + 0.5;
  return Uint8(col);
  }

Uint8 STT_Button::dark_col(const Uint8 c) {
  float col = (float)c;
  col *= COL_SCALOR;
  col += 0.5;
  return Uint8(col);
  }

SDL_Surface *STT_Button::BuildInternal(SDL_Surface *surf,
	const Uint32 xsize, const Uint32 ysize,
	const SDL_Color &col, const SDL_Color &tlcol, const SDL_Color &brcol) {

  SDL_FillRect(surf, NULL, SDL_MapRGB(surf->format, col.r, col.g, col.b));

  Uint32 width = ysize/8;
  if(width > xsize/8) width = xsize/8;

  SDL_Rect lft = { 0, 0, width, surf->h };
  SDL_Rect top = { 0, 0, surf->w, width };
  SDL_FillRect(surf, &lft, SDL_MapRGB(surf->format, tlcol.r, tlcol.g, tlcol.b));
  SDL_FillRect(surf, &top, SDL_MapRGB(surf->format, tlcol.r, tlcol.g, tlcol.b));

  SDL_Rect rgt, btm;
  rgt.y = width;
  rgt.h = surf->h - width;
  rgt.x = xsize - width;
  rgt.w = 1;

  btm.x = width;
  btm.w = surf->w - width;
  btm.y = ysize - width;
  btm.h = 1;

  for(Uint32 x = 0; x < width; ++x) {
    SDL_FillRect(surf, &rgt, SDL_MapRGB(surf->format, brcol.r, brcol.g, brcol.b));
    SDL_FillRect(surf, &btm, SDL_MapRGB(surf->format, brcol.r, brcol.g, brcol.b));
    --rgt.y;  --btm.x;
    SDL_Rect tmpb = btm, tmpr = rgt;
    ++rgt.x;  ++rgt.h;  ++btm.y;  ++btm.w;
    tmpb.w = 1;  tmpb.h = 1;  tmpr.w = 1;  tmpr.h = 1;
    SDL_FillRect(surf, &tmpb, SDL_MapRGB(surf->format, col.r, col.g, col.b));
    SDL_FillRect(surf, &tmpr, SDL_MapRGB(surf->format, col.r, col.g, col.b));
    }

  return surf;
  }
