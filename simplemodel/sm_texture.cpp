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

#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>

#include "sm_texture.h"

static int nextpoweroftwo(int x) {
  if(x <= 2) return 2;

  --x;          //Hitch it down in case it's exactly a power of 2
  int p = 1;
  for(; x != 1; ++p, x>>=1);
  x <<= p;
  return x;
  }

SM_Texture::SM_Texture(SDL_Surface *tex) {
  type = SM_TEXTURE_DEFINED;
  texture = 0;
  cur = NULL;
  src = tex;
  xfact = 1.0;
  yfact = 1.0;
  dirty = 1;
  }

SM_Texture::SM_Texture(const string &filenm) {
  filename = filenm;
  type = SM_TEXTURE_DEFINED;
  texture = 0;
  cur = NULL;
  src = IMG_Load(filenm.c_str());
  if(!src) {
    // Judging by many of the models I've see, this is not an error.
    type = SM_TEXTURE_NONE;
    dirty = 0;
    return;
    }
  xfact = 1.0;
  yfact = 1.0;
  dirty = 1;
  }

SM_Texture::~SM_Texture() {
  if(trans_cache.count(src) > 0) {
    trans_cache[src].erase(this);
    if(trans_cache[src].size() < 1) {
      trans_cache.erase(src);
      if(texture != 0) glDeleteTextures(1, &texture);
      if(cur) {
	glFinish();	// Don't yank the rug out from under GL
	SDL_FreeSurface(cur);
	cur = NULL;
	}
      }
    }
  else if(def_cache.count(src) > 0) {
    def_cache[src].erase(this);
    if(def_cache[src].size() < 1) {
      def_cache.erase(src);
      if(texture != 0) glDeleteTextures(1, &texture);
      if(cur) {
	glFinish();	// Don't yank the rug out from under GL
	SDL_FreeSurface(cur);
	cur = NULL;
	}
      }
    }
  else {
    if(texture != 0) glDeleteTextures(1, &texture);
    if(cur) {
      glFinish();	// Don't yank the rug out from under GL
      SDL_FreeSurface(cur);
      cur = NULL;
      }
    }
  }

bool SM_Texture::CheckCache() {
  if(texture != 0) {
    return false; // Temporary - don't currently handle this case with cache
    }

  if(type == SM_TEXTURE_TRANS) {
    if(trans_cache.count(src) > 0) {
      (*this) = *(*(trans_cache[src].begin()));
      UpdateCache();
      return true;
      }
    else {
      return false;
      }
    }
  else if(type == SM_TEXTURE_DEFINED) {
    if(def_cache.count(src) > 0) {
      (*this) = *(*(def_cache[src].begin()));
      UpdateCache();
      return true;
      }
    else {
      return false;
      }
    }
  else {
    return false;
    }
  }

void SM_Texture::Update() {
  if(type == SM_TEXTURE_DEFINED) {
    int xsize = nextpoweroftwo(src->w);
    int ysize = nextpoweroftwo(src->h);
    cur = SDL_CreateRGBSurface(0, xsize, ysize, 32, SM_SDL_RGBA_COLFIELDS);
    memset(cur->pixels, 0, xsize*ysize*4);
    SDL_SetAlpha(src, 0, SDL_ALPHA_OPAQUE);
    SDL_BlitSurface(src, NULL, cur, NULL);

    if(texture == 0) glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);
//    glTexImage2D(GL_TEXTURE_2D, 0, 4,
//	cur->w, cur->h, 0, GL_RGBA,
//	GL_UNSIGNED_BYTE, cur->pixels );

    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, cur->w, cur->h, GL_RGBA, GL_UNSIGNED_BYTE, cur->pixels);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glBindTexture(GL_TEXTURE_2D, 0);

    dirty = 0;
    }
  UpdateCache();
  }

void SM_Texture::UpdateCache() {
  if(type == SM_TEXTURE_TRANS) {
    trans_cache[src].insert(this);
    }
  else if(type == SM_TEXTURE_DEFINED) {
    def_cache[src].insert(this);
    }
  else {
    }
  }

map<SDL_Surface *, set<SM_Texture *> > SM_Texture::trans_cache;
map<SDL_Surface *, set<SM_Texture *> > SM_Texture::def_cache;

unsigned char sm_col_u32b1[4] = { 0xFF, 0x00, 0x00, 0x00 };
unsigned char sm_col_u32b2[4] = { 0x00, 0xFF, 0x00, 0x00 };
unsigned char sm_col_u32b3[4] = { 0x00, 0x00, 0xFF, 0x00 };
unsigned char sm_col_u32b4[4] = { 0x00, 0x00, 0x00, 0xFF };
