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

#ifndef SM_TEXTURE_H
#define SM_TEXTURE_H

#include <map>
#include <set>
#include <string>
using namespace std;

#include <SDL/SDL.h>

enum SM_TextureType {
   SM_TEXTURE_NONE = 0,
   SM_TEXTURE_COLOR,
   SM_TEXTURE_TRANSCOLOR,
   SM_TEXTURE_DEFINED,
   SM_TEXTURE_TRANS,
   SM_TEXTURE_FILE,
   SM_TEXTURE_TRANSFILE,
   SM_TEXTURE_MAX
   };

class SM_Texture {
public:
  SM_Texture(SDL_Surface *tex);
  SM_Texture(const string &filenm);
  ~SM_Texture();

  bool CheckCache();
  void Update();

  GLuint GLTexture() { return texture; };

  SM_TextureType type;
  SDL_Surface *cur;	//Current texture buffer
  SDL_Surface *src;	//Only for SM_TEXTURE_DEFINED/SM_TEXTURE_FILE
  SDL_Color col;	//Only for SM_TEXTURE_COLOR
  float xfact, yfact;	//Portion of texture actually shown
  bool dirty; //Does the system need to rebuild this texture?

  string filename;

protected:
  GLuint texture;	//Current texture (when active)

  void UpdateCache();
  static map<SDL_Surface *, set<SM_Texture *> > trans_cache;
  static map<SDL_Surface *, set<SM_Texture *> > def_cache;
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

#endif // SM_TEXTURE_H
