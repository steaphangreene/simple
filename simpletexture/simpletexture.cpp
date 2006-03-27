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

#define	ST_NUM_SYSTEM_COLORS	32

static int nextpoweroftwo(int x) {
  if(x <= 2) return 2;

  --x;          //Hitch it down in case it's exactly a power of 2
  int p = 1;
  for(; x != 1; ++p, x>>=1);
  x <<= p;
  return x;
  }

void SimpleTexture::InitStatic() {
  if(color.size() < (ST_NUM_SYSTEM_COLORS * 2)) {
    color.resize(ST_NUM_SYSTEM_COLORS * 2);
    }
  }

void SimpleTexture::Init(const SimpleTextureType tp) {
  SimpleTexture::InitStatic();
  type = tp;
  cur = NULL;
  src = NULL;
  xfact = 1.0;
  yfact = 1.0;
  texture = 0;
  dirty = 0;	//Since it's not ready to be "cleaned" yet.
  }


SimpleTexture::SimpleTexture(SDL_Surface *tex) {
  Init(SIMPLETEXTURE_DEFINED);
  fg = default_text_color;
  src = tex;
  dirty = 1;
  }

SimpleTexture::SimpleTexture(const int col_index) {
  Init(SIMPLETEXTURE_COLOR);
  if((int)(color.size()) > (col_index*2 + 1)) {
    col = color[col_index*2];
    fg = color[col_index*2 + 1];
    }
  else {
    fprintf(stderr, "ERROR: Color index '%d' out of range!\n", col_index);
    exit(1);
    }
  dirty = 1;
  }

SimpleTexture::SimpleTexture(const string &filenm) {
  Init(SIMPLETEXTURE_DEFINED);
  filename = filenm;

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

SimpleTexture::SimpleTexture(const SimpleTexture &in) {
  CopyFrom(in);
  }

const SimpleTexture &SimpleTexture::operator = (const SimpleTexture &in) {
  Clear();
  CopyFrom(in);
  return *this;
  }

SimpleTexture::~SimpleTexture() {
  Clear();
  }

void SimpleTexture::CopyFrom(const SimpleTexture &in) {
  memcpy(this, &in, sizeof(SimpleTexture));	//WARNING! Shallow Copy
  if(trans_cache.count(src) > 0) {
    UpdateCache();			//Use cache to make it deep
    }
  else if(def_cache.count(src) > 0) {
    UpdateCache();			//Use cache to make it deep
    }
  else {
    cur = NULL;			//Don't use cache, make it recreate itself
    texture = 0;
    xfact = 1.0;
    yfact = 1.0;
    dirty = 1;
    }
  }

void SimpleTexture::Clear() {
  if(trans_cache.count(src) > 0) {
    trans_cache[src].erase(this);
    if(trans_cache[src].size() < 1) {
      trans_cache.erase(src);
      if(texture != 0) trash_tex.push_back(texture);
      if(cur) {
	trash_surf.push_back(cur);
	cur = NULL;
	}
      }
    }
  else if(def_cache.count(src) > 0) {
    def_cache[src].erase(this);
    if(def_cache[src].size() < 1) {
      def_cache.erase(src);
      if(texture != 0) trash_tex.push_back(texture);
      if(cur) {
	trash_surf.push_back(cur);
	cur = NULL;
	}
      }
    }
  else {
    if(texture != 0) trash_tex.push_back(texture);
    if(cur) {
      trash_surf.push_back(cur);
      cur = NULL;
      }
    }
  }

void SimpleTexture::EmptyTrash() {
  if(trash_tex.size() > 0 || trash_surf.size() > 0) {
    list<GLuint>::iterator itri = trash_tex.begin();
    for(; itri != trash_tex.end(); ++itri) {
      glDeleteTextures(1, &(*itri));
      }
    trash_tex.clear();

    glFinish();	// Don't yank the rug out from under GL

    list<SDL_Surface *>::iterator itrs = trash_surf.begin();
    for(; itrs != trash_surf.end(); ++itrs) {
      SDL_FreeSurface(*itrs);
      }
    trash_surf.clear();
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
  if(type != SIMPLETEXTURE_NONE) {
    int xsize, ysize;
    if(src) {
      xsize = nextpoweroftwo(src->w);
      ysize = nextpoweroftwo(src->h);
      if(cur) SDL_FreeSurface(cur); //FIXME!
      cur = SDL_CreateRGBSurface(0, xsize, ysize, 32, ST_SDL_RGBA_COLFIELDS);
      memset(cur->pixels, 0, xsize*ysize*4);
      SDL_SetAlpha(src, 0, SDL_ALPHA_OPAQUE);
      SDL_BlitSurface(src, NULL, cur, NULL);
      }
    else {
      xsize = nextpoweroftwo(cur->w);
      ysize = nextpoweroftwo(cur->h);
      }

    //This is the automatic downsizing code - originally from SimpleGUI
    glTexImage2D(GL_PROXY_TEXTURE_2D, 0, 4, xsize, ysize, 0, GL_RGBA,
	GL_UNSIGNED_BYTE, cur->pixels );
    GLint res;
    glGetTexLevelParameteriv(GL_PROXY_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &res);
    while(res == 0) {
      if(xsize >= ysize) xsize /= 2;
      else ysize /= 2;
      glTexImage2D(GL_PROXY_TEXTURE_2D, 0, 4, xsize, ysize, 0, GL_RGBA,
	GL_UNSIGNED_BYTE, cur->pixels );
      glGetTexLevelParameteriv(GL_PROXY_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &res);
      }
    if(xsize != cur->w || ysize != cur->h) {
      SDL_Surface *tmp = cur;
      cur = SDL_CreateRGBSurface(0, xsize, ysize, 32, ST_SDL_RGBA_COLFIELDS);
      gluScaleImage(GL_RGBA, tmp->w, tmp->h, GL_UNSIGNED_BYTE, tmp->pixels,
	cur->w, cur->h, GL_UNSIGNED_BYTE, cur->pixels);
      SDL_FreeSurface(tmp);
      }

    if(texture == 0) glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);

    //This is how SimpleGUI did it (for 2-D usage).
    glTexImage2D(GL_TEXTURE_2D, 0, 4, cur->w, cur->h, 0, GL_RGBA,
	GL_UNSIGNED_BYTE, cur->pixels );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //This is how SimpleModel did it (for 3-D usage).
//    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, cur->w, cur->h, GL_RGBA, GL_UNSIGNED_BYTE, cur->pixels);
//    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
//    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

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
    // Temporary - don't currently handle this case with cache
    }
  }

float SimpleTexture::Red(const int col_index) {
  SimpleTexture::InitStatic();
  if((int)(color.size()) <= (col_index*2 + 1)) {
    fprintf(stderr, "ERROR: Color index '%d' out of range!\n", col_index);
    exit(1);
    }
  return color[col_index*2 + 0].r / 255.0f;
  }

float SimpleTexture::Green(const int col_index) {
  SimpleTexture::InitStatic();
  if((int)(color.size()) <= (col_index*2 + 1)) {
    fprintf(stderr, "ERROR: Color index '%d' out of range!\n", col_index);
    exit(1);
    }
  return color[col_index*2 + 0].g / 255.0f;
  }

float SimpleTexture::Blue(const int col_index) {
  SimpleTexture::InitStatic();
  if((int)(color.size()) <= (col_index*2 + 1)) {
    fprintf(stderr, "ERROR: Color index '%d' out of range!\n", col_index);
    exit(1);
    }
  return color[col_index*2 + 0].b / 255.0f;
  }

float SimpleTexture::TextRed(const int col_index) {
  SimpleTexture::InitStatic();
  if((int)(color.size()) <= (col_index*2 + 1)) {
    fprintf(stderr, "ERROR: Color index '%d' out of range!\n", col_index);
    exit(1);
    }
  return color[col_index*2 + 1].r / 255.0f;
  }

float SimpleTexture::TextGreen(const int col_index) {
  SimpleTexture::InitStatic();
  if((int)(color.size()) <= (col_index*2 + 1)) {
    fprintf(stderr, "ERROR: Color index '%d' out of range!\n", col_index);
    exit(1);
    }
  return color[col_index*2 + 1].g / 255.0f;
  }

float SimpleTexture::TextBlue(const int col_index) {
  SimpleTexture::InitStatic();
  if((int)(color.size()) <= (col_index*2 + 1)) {
    fprintf(stderr, "ERROR: Color index '%d' out of range!\n", col_index);
    exit(1);
    }
  return color[col_index*2 + 1].b / 255.0f;
  }

const SDL_Color *SimpleTexture::BGColor(const int col_index) {
  SimpleTexture::InitStatic();
  if((int)(color.size()) <= (col_index*2 + 1)) {
    fprintf(stderr, "ERROR: Color index '%d' out of range!\n", col_index);
    exit(1);
    }
  return &color[col_index*2 + 0];
  }

const SDL_Color *SimpleTexture::TextColor(const int col_index) {
  SimpleTexture::InitStatic();
  if((int)(color.size()) <= (col_index*2 + 1)) {
    fprintf(stderr, "ERROR: Color index '%d' out of range!\n", col_index);
    exit(1);
    }
  return &color[col_index*2 + 1];
  }

void SimpleTexture::SetColor(const int col_index, const SDL_Color &col,
	const SDL_Color &text) {
  SimpleTexture::InitStatic();
  if((int)(color.size()) <= (col_index*2 + 1)) {
    fprintf(stderr, "ERROR: Color index '%d' out of range!\n", col_index);
    exit(1);
    }
  color[col_index*2 + 0] = col;
  color[col_index*2 + 1] = text;
  }

void SimpleTexture::SetColor(const int col_index,
	const float r, const float g, const float b,
	const float tr, const float tg, const float tb) {
  SimpleTexture::InitStatic();
  if((int)(color.size()) <= (col_index*2 + 1)) {
    fprintf(stderr, "ERROR: Color index '%d' out of range!\n", col_index);
    exit(1);
    }
  color[col_index*2 + 0].r = (Uint8)(r*255.0f);
  color[col_index*2 + 0].g = (Uint8)(g*255.0f);
  color[col_index*2 + 0].b = (Uint8)(b*255.0f);
  if(r == -1.0) {
    color[col_index*2 + 1] = default_text_color;
    }
  else {
    color[col_index*2 + 1].r = (Uint8)(tr*255.0f);
    color[col_index*2 + 1].g = (Uint8)(tg*255.0f);
    color[col_index*2 + 1].b = (Uint8)(tb*255.0f);
    }
  }

int SimpleTexture::NewColor() {
  SimpleTexture::InitStatic();
  int ret = color.size();
  color.resize(ret+2);
  return ret/2;
  }

int SimpleTexture::NewColor(const float r, const float g, const float b,
        const float tr, const float tg, const float tb) {
  SimpleTexture::InitStatic();
  int ret = NewColor();
  SetColor(ret, r, g, b, tr, tg, tb);
  return ret;
  }

void SimpleTexture::SetDefaultTextColor(
	const float tr, const float tg, const float tb) {
  default_text_color.r = (Uint8)(tr*255.0f);
  default_text_color.g = (Uint8)(tg*255.0f);
  default_text_color.b = (Uint8)(tb*255.0f);
  }

const SDL_Color *SimpleTexture::DefaultTextColor() {
  return &default_text_color;
  }

map<SDL_Surface *, set<SimpleTexture *> > SimpleTexture::trans_cache;
map<SDL_Surface *, set<SimpleTexture *> > SimpleTexture::def_cache;

unsigned char st_col_u32b1[4] = { 0xFF, 0x00, 0x00, 0x00 };
unsigned char st_col_u32b2[4] = { 0x00, 0xFF, 0x00, 0x00 };
unsigned char st_col_u32b3[4] = { 0x00, 0x00, 0xFF, 0x00 };
unsigned char st_col_u32b4[4] = { 0x00, 0x00, 0x00, 0xFF };

list<SDL_Surface *> SimpleTexture::trash_surf;
list<GLuint> SimpleTexture::trash_tex;

vector<SDL_Color> SimpleTexture::color;
SDL_Color SimpleTexture::default_text_color = {0, 0, 0, 0};
