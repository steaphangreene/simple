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

#ifndef STT_H
#define STT_H

#include "SDL.h"

class ST_Texturator {
public:
  ST_Texturator();
  virtual ~ST_Texturator();

  SDL_Surface *Generate(SDL_Surface *surf,
	const Uint32 xsize, const Uint32 ysize, const SDL_Color &col);

protected:
  virtual SDL_Surface *BuildTexture(SDL_Surface *surf,
	const Uint32 xsize, const Uint32 ysize, const SDL_Color &col);
  };

extern unsigned char st_col_u32b1[4];
extern unsigned char st_col_u32b2[4];
extern unsigned char st_col_u32b3[4];
extern unsigned char st_col_u32b4[4];

#define ST_COL_U32B1 (*((unsigned long *)(st_col_u32b1)))
#define ST_COL_U32B2 (*((unsigned long *)(st_col_u32b2)))
#define ST_COL_U32B3 (*((unsigned long *)(st_col_u32b3)))
#define ST_COL_U32B4 (*((unsigned long *)(st_col_u32b4)))

#define ST_SDL_RGBA_COLFIELDS ST_COL_U32B1, ST_COL_U32B2, ST_COL_U32B3, ST_COL_U32B4

#endif // STT_H
