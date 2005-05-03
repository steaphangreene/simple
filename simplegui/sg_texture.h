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

#ifndef SG_TEXTURE_H
#define SG_TEXTURE_H

#include <map>
#include <set>
using namespace std;

#include "SDL.h"

enum SG_TextureType {
   SG_TEXTURE_NONE = 0,
   SG_TEXTURE_COLOR,
   SG_TEXTURE_TRANSCOLOR,
   SG_TEXTURE_DEFINED,
   SG_TEXTURE_TRANS,
   SG_TEXTURE_MAX
   };

class SG_Texture {
public:
  SG_Texture(SDL_Surface *tex);
  SG_Texture(int sg_cols);
  SG_Texture(const SG_Texture &in);
  ~SG_Texture();

  bool CheckCache();
  void Update();

  const SG_Texture &operator = (const SG_Texture &in);

  GLuint GLTexture() { return texture; };

  SG_TextureType type;
  SDL_Surface *cur;	//Current texture buffer
  SDL_Surface *src;	//Only for SG_TEXTURE_DEFINED
  SDL_Color col;	//Only for SG_TEXTURE_COLOR
  SDL_Color fg;		//Only used by children (font color)
  float xfact, yfact;	//Portion of texture actually shown
  bool dirty; //Does the system need to rebuild this texture?

protected:
  void Clear();
  void CopyFrom(const SG_Texture &in);

  GLuint texture;	//Current texture (when active)

  void UpdateCache();
  static map<SDL_Surface *, set<SG_Texture *> > trans_cache;
  static map<SDL_Surface *, set<SG_Texture *> > def_cache;
  };

extern unsigned char sg_col_u32b1[4];
extern unsigned char sg_col_u32b2[4];
extern unsigned char sg_col_u32b3[4];
extern unsigned char sg_col_u32b4[4];

#define SG_COL_U32B1 (*((unsigned long *)(sg_col_u32b1)))
#define SG_COL_U32B2 (*((unsigned long *)(sg_col_u32b2)))
#define SG_COL_U32B3 (*((unsigned long *)(sg_col_u32b3)))
#define SG_COL_U32B4 (*((unsigned long *)(sg_col_u32b4)))

#define SG_SDL_RGBA_COLFIELDS SG_COL_U32B1, SG_COL_U32B2, SG_COL_U32B3, SG_COL_U32B4

#endif // SG_TEXTURE_H
