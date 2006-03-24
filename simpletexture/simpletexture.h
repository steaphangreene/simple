// *************************************************************************
//  This file is part of the SimpleTexture Example Module by Steaphan Greene
//
//  Copyright 2005 Steaphan Greene <stea@cs.binghamton.edu>
//
//  SimpleTexture is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  SimpleTexture is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SimpleTexture (see the file named "COPYING");
//  if not, write to the the Free Software Foundation, Inc.,
//  59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
// *************************************************************************

#ifndef SIMPLETEXTURE_H
#define SIMPLETEXTURE_H

#include <map>
#include <set>
#include <string>
using namespace std;

#include "SDL.h"

enum SimpleTextureType {
   SIMPLETEXTURE_NONE = 0,
   SIMPLETEXTURE_COLOR,
   SIMPLETEXTURE_TRANSCOLOR,
   SIMPLETEXTURE_DEFINED,
   SIMPLETEXTURE_TRANS,
   SIMPLETEXTURE_FILE,
   SIMPLETEXTURE_TRANSFILE,
   SIMPLETEXTURE_MAX
   };

class SimpleTexture {
public:
  SimpleTexture(SDL_Surface *tex);
  SimpleTexture(const string &filenm);
  ~SimpleTexture();

  bool CheckCache();
  void Update();

  GLuint GLTexture() { return texture; };

  SimpleTextureType type;
  SDL_Surface *cur;	//Current texture buffer
  SDL_Surface *src;	//Only for SIMPLETEXTURE_DEFINED/SIMPLETEXTURE_FILE
  SDL_Color col;	//Only for SIMPLETEXTURE_COLOR
  float xfact, yfact;	//Portion of texture actually shown
  bool dirty; //Does the system need to rebuild this texture?

  string filename;

protected:
  GLuint texture;	//Current texture (when active)

  void UpdateCache();
  static map<SDL_Surface *, set<SimpleTexture *> > trans_cache;
  static map<SDL_Surface *, set<SimpleTexture *> > def_cache;
  };

extern unsigned char sm_col_u32b1[4];
extern unsigned char sm_col_u32b2[4];
extern unsigned char sm_col_u32b3[4];
extern unsigned char sm_col_u32b4[4];

#define SM_COL_U32B1 (*((unsigned long *)(sm_col_u32b1)))
#define SM_COL_U32B2 (*((unsigned long *)(sm_col_u32b2)))
#define SM_COL_U32B3 (*((unsigned long *)(sm_col_u32b3)))
#define SM_COL_U32B4 (*((unsigned long *)(sm_col_u32b4)))

#define SM_SDL_RGBA_COLFIELDS SM_COL_U32B1, SM_COL_U32B2, SM_COL_U32B3, SM_COL_U32B4

#endif // SIMPLETEXTURE_H
