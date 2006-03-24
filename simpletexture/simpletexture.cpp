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

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_opengl.h"

#include "simpletexture.h"

static int nextpoweroftwo(int x) {
  if(x <= 2) return 2;

  --x;          //Hitch it down in case it's exactly a power of 2
  int p = 1;
  for(; x != 1; ++p, x>>=1);
  x <<= p;
  return x;
  }

SimpleTexture::SimpleTexture(SDL_Surface *tex) {
  type = SIMPLETEXTURE_DEFINED;
  texture = 0;
  cur = NULL;
  src = tex;
  xfact = 1.0;
  yfact = 1.0;
  dirty = 1;
  }

SimpleTexture::SimpleTexture(const string &filenm) {
  filename = filenm;
  type = SIMPLETEXTURE_DEFINED;
  texture = 0;
  cur = NULL;
  src = NULL;

  SDL_RWops *file = SDL_RWFromFile(filenm.c_str(), "rb");
  if(file && tolower(filenm[filenm.length()-2]) == 'g'
	&& tolower(filenm[filenm.length()-3]) == 't'
	&& tolower(filenm[filenm.length()-1]) == 'a') {
    src = IMG_LoadTyped_RW(file, true, "TGA");
    }

  if(!file) {
    // Judging by many of the models I've see, this is not an error.
    file = SDL_RWFromFile((filenm + ".png").c_str(), "rb");
    }
  if(!file) {
    // Judging by many of the models I've see, this is not an error.
    file = SDL_RWFromFile((filenm + ".PNG").c_str(), "rb");
    }
  if(!file) {
    // Judging by many of the models I've see, this is not an error.
    file = SDL_RWFromFile((filenm + ".bmp").c_str(), "rb");
    }
  if(!file) {
    // Judging by many of the models I've see, this is not an error.
    file = SDL_RWFromFile((filenm + ".BMP").c_str(), "rb");
    }
  if(!file) {
    // Judging by many of the models I've see, this is not an error.
    file = SDL_RWFromFile((filenm + ".tga").c_str(), "rb");
    if(file) src = IMG_LoadTyped_RW(file, true, "TGA");
    }
  if(!file) {
    // Judging by many of the models I've see, this is not an error.
    file = SDL_RWFromFile((filenm + ".TGA").c_str(), "rb");
    if(file) src = IMG_LoadTyped_RW(file, true, "TGA");
    }

  if(!file) {
    // Judging by many of the models I've see, even this is not an error.
    type = SIMPLETEXTURE_NONE;
    dirty = 0;
    }
  else {
    if(!src) src = IMG_Load_RW(file, true);
    xfact = 1.0;
    yfact = 1.0;
    dirty = 1;
//    if(!src) {
//      //FIXME: Error Handling!
//      }
    }
  }

SimpleTexture::~SimpleTexture() {
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

bool SimpleTexture::CheckCache() {
  if(texture != 0) {
    return false; // Temporary - don't currently handle this case with cache
    }

  if(type == SIMPLETEXTURE_TRANS) {
    if(trans_cache.count(src) > 0) {
      (*this) = *(*(trans_cache[src].begin()));
      UpdateCache();
      return true;
      }
    else {
      return false;
      }
    }
  else if(type == SIMPLETEXTURE_DEFINED) {
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

void SimpleTexture::Update() {
  if(type == SIMPLETEXTURE_DEFINED) {
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

void SimpleTexture::UpdateCache() {
  if(type == SIMPLETEXTURE_TRANS) {
    trans_cache[src].insert(this);
    }
  else if(type == SIMPLETEXTURE_DEFINED) {
    def_cache[src].insert(this);
    }
  else {
    }
  }

map<SDL_Surface *, set<SimpleTexture *> > SimpleTexture::trans_cache;
map<SDL_Surface *, set<SimpleTexture *> > SimpleTexture::def_cache;

unsigned char sm_col_u32b1[4] = { 0xFF, 0x00, 0x00, 0x00 };
unsigned char sm_col_u32b2[4] = { 0x00, 0xFF, 0x00, 0x00 };
unsigned char sm_col_u32b3[4] = { 0x00, 0x00, 0xFF, 0x00 };
unsigned char sm_col_u32b4[4] = { 0x00, 0x00, 0x00, 0xFF };
