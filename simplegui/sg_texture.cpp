// *************************************************************************
//  This file is part of the SimpleGUI Example Module by Steaphan Greene
//
//  Copyright 2005 Steaphan Greene <stea@cs.binghamton.edu>
//
//  SimpleGUI is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  SimpleGUI is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SimpleGUI (see the file named "COPYING");
//  if not, write to the the Free Software Foundation, Inc.,
//  59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
// *************************************************************************

#include <GL/gl.h>

#include "sg_texture.h"
#include "sg_globals.h"

SG_Texture::SG_Texture(SDL_Surface *tex) {
  type = SG_TEXTURE_DEFINED;
  fg = *(current_sg->DefaultTextColor());
  texture = 0;
  cur = NULL;
  src = tex;
  xfact = 1.0;
  yfact = 1.0;
  dirty = 1;
  }

SG_Texture::SG_Texture(int sg_cols) {
  if(sg_cols > 0) {
    type = SG_TEXTURE_COLOR;
    col = *(current_sg->BGColor(sg_cols));
    fg = *(current_sg->TextColor(sg_cols));
    }
  else {
    type = SG_TEXTURE_TRANS;
    }
  texture = 0;
  cur = NULL;
  src = NULL;
  xfact = 1.0;
  yfact = 1.0;
  dirty = 1;
  }

unsigned char sg_col_u32b1[4] = { 0xFF, 0x00, 0x00, 0x00 };
unsigned char sg_col_u32b2[4] = { 0x00, 0xFF, 0x00, 0x00 };
unsigned char sg_col_u32b3[4] = { 0x00, 0x00, 0xFF, 0x00 };
unsigned char sg_col_u32b4[4] = { 0x00, 0x00, 0x00, 0xFF };
