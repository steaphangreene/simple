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

#include "SDL_opengl.h"

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
  type = SG_TEXTURE_COLOR;
  col = *(current_sg->BGColor(sg_cols));
  fg = *(current_sg->TextColor(sg_cols));
  texture = 0;
  cur = NULL;
  src = NULL;
  xfact = 1.0;
  yfact = 1.0;
  dirty = 1;
  }

SG_Texture::~SG_Texture() {
  Clear();
  }

SG_Texture::SG_Texture(const SG_Texture &in) {
  CopyFrom(in);
  }

const SG_Texture &SG_Texture::operator = (const SG_Texture &in) {
  Clear();
  CopyFrom(in);
  return *this;
  }

void SG_Texture::CopyFrom(const SG_Texture &in) {
  memcpy(this, &in, sizeof(SG_Texture));	//WARNING! Shallow Copy
  if(trans_cache.count(src) > 0) {
    UpdateCache();			//Use cache to make it deep
    }
  else if(def_cache.count(src) > 0) {
    UpdateCache();			//Use cache to make it deep
    }
  }

void SG_Texture::Clear() {
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

bool SG_Texture::CheckCache() {
  if(texture != 0) {
    return false; // Temporary - don't currently handle this case with cache
    }

  if(type == SG_TEXTURE_TRANS) {
    if(trans_cache.count(src) > 0) {
      (*this) = *(*(trans_cache[src].begin()));
      UpdateCache();
      return true;
      }
    else {
      return false;
      }
    }
  else if(type == SG_TEXTURE_DEFINED) {
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

void SG_Texture::Update() {
  if(texture == 0) glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, 4,
	cur->w, cur->h, 0, GL_RGBA,
	GL_UNSIGNED_BYTE, cur->pixels );
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glBindTexture(GL_TEXTURE_2D, 0);

  dirty = 0;
  UpdateCache();
  }

void SG_Texture::UpdateCache() {
  if(type == SG_TEXTURE_TRANS) {
    trans_cache[src].insert(this);
    }
  else if(type == SG_TEXTURE_DEFINED) {
    def_cache[src].insert(this);
    }
  else {
    // Temporary - don't currently handle this case with cache
    }
  }

map<SDL_Surface *, set<SG_Texture *> > SG_Texture::trans_cache;
map<SDL_Surface *, set<SG_Texture *> > SG_Texture::def_cache;

unsigned char sg_col_u32b1[4] = { 0xFF, 0x00, 0x00, 0x00 };
unsigned char sg_col_u32b2[4] = { 0x00, 0xFF, 0x00, 0x00 };
unsigned char sg_col_u32b3[4] = { 0x00, 0x00, 0xFF, 0x00 };
unsigned char sg_col_u32b4[4] = { 0x00, 0x00, 0x00, 0xFF };
