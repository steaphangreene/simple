// *************************************************************************
//  This file is part of the SimpleTexture Example Module by Steaphan Greene
//
//  Copyright 2005-2008 Steaphan Greene <stea@cs.binghamton.edu>
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
#include "SDL_ttf.h"
#include "SDL_opengl.h"

#include "st.h"
#include "stt_default.h"
#include "stt_invisible.h"
#include "stt_buttonup.h"
#include "stt_buttondown.h"
#include "saferead.h"

#define	ST_NUM_SYSTEM_COLORS	32

static int nextpoweroftwo(int x) {
  if(x <= 2) return 2;

  --x;		//Hitch it down in case it's exactly a power of 2
  int p = 1;
  for(; x != 1; ++p, x>>=1);
  x <<= p;
  return x;
  }

struct SimpleTexture::TextData {
  TextData() {
    rendered_text = NULL;
    text_xsize = 0;
    text_ysize = 0;
    visible_xlines = ST_KEEPASPECT;
    visible_ylines = ST_AUTOSIZE;
    visible_xoffset = 0.0;
    visible_yoffset = 0.0;
    xmargin = 0.0;
    ymargin = 0.0;
    font_size = ST_AUTOSIZE;
    alignment = ST_ALIGN_LEFT;
    aspect_ratio = 0.0;
    dirty = 0; //Not yet ready to be "cleaned"
    };

  ~TextData() {
    if(rendered_text) SDL_FreeSurface(rendered_text);
    };

  //FIXME: This is the font stuff ported from SimpleGUI - needs cleanup!
  SDL_Surface *rendered_text;
  int text_xsize, text_ysize;

  //FIXME: This is the font stuff ported from SimpleGUI - needs cleanup!
  string message;
  vector<string> lines;
  float xmargin, ymargin;
  float visible_xlines, visible_ylines;
  float visible_xoffset, visible_yoffset;
  double aspect_ratio;
  int font_size;
  int alignment;
  int dirty;
  set<SimpleTexture *> clients;
  };


void SimpleTexture::InitStatic() {
  if(color.size() < (ST_NUM_SYSTEM_COLORS * 2)) {
    color.resize(ST_NUM_SYSTEM_COLORS * 2);
    }
  if(!default_texturator) default_texturator = new STT_Default;
  if(!invisible_texturator) invisible_texturator = new STT_Invisible;
  }

void SimpleTexture::Init(const SimpleTextureType tp) {
  SimpleTexture::InitStatic();
  type = tp;
  cur = NULL;
  src = NULL;
  xfact = 1.0;
  yfact = 1.0;
  texture = 0;
  text = NULL;
  texturator = default_texturator;
  dirty = 0;	//Since it's not ready to be "cleaned" yet.
  all_textures.insert(this);	// So I can be invalidated!
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
  { string::iterator chr = filename.begin();
    for(; chr < filename.end(); ++chr) {
      if((*chr) == '\\') (*chr) = '/';
      }
    }

  SDL_RWops *file = SDL_RWFromZZIP(filename.c_str(), "rb");
  if(file && tolower(filename[filename.length()-2]) == 'g'
	&& tolower(filename[filename.length()-3]) == 't'
	&& tolower(filename[filename.length()-1]) == 'a') {
    src = IMG_LoadTyped_RW(file, true, "TGA");
    }

  if(!file) {
    // Judging by many of the models I've see, this is not an error.
    file = SDL_RWFromZZIP((filename + ".png").c_str(), "rb");
    }
  if(!file) {
    // Judging by many of the models I've see, this is not an error.
    file = SDL_RWFromZZIP((filename + ".PNG").c_str(), "rb");
    }
  if(!file) {
    // Judging by many of the models I've see, this is not an error.
    file = SDL_RWFromZZIP((filename + ".bmp").c_str(), "rb");
    }
  if(!file) {
    // Judging by many of the models I've see, this is not an error.
    file = SDL_RWFromZZIP((filename + ".BMP").c_str(), "rb");
    }
  if(!file) {
    // Judging by many of the models I've see, this is not an error.
    file = SDL_RWFromZZIP((filename + ".tga").c_str(), "rb");
    if(file) src = IMG_LoadTyped_RW(file, true, "TGA");
    }
  if(!file) {
    // Judging by many of the models I've see, this is not an error.
    file = SDL_RWFromZZIP((filename + ".TGA").c_str(), "rb");
    if(file) src = IMG_LoadTyped_RW(file, true, "TGA");
    }
  if(!file) {
    // Judging by many of the models I've see, this is not an error.
    file = SDL_RWFromZZIP((filename + ".jpg").c_str(), "rb");
    }
  if(!file) {
    // Judging by many of the models I've see, this is not an error.
    file = SDL_RWFromZZIP((filename + ".JPG").c_str(), "rb");
    }

  if(!file) {
    // Judging by many of the models I've see, even this is not an error.
    //fprintf(stderr, "WARNING: Unable to open '%s'\n", filename.c_str());
    type = SIMPLETEXTURE_NONE;
    dirty = 0;
    }
  else {
    if(!src) {
      Uint8 head[5] = {0};
      SDL_RWread(file, (char*)head, 4, 1);
      if(!strncmp((char*)head, "BLP1", 4)) {
	Uint32 uncomp, n2, xs, ys, n5, n6;
	Uint32 off[16], siz[16];
	int ret = 0;
	ret += freadLE(uncomp, file);
	ret += freadLE(n2, file);	//NumMipmaps?  Nope.  What is this?
	ret += freadLE(xs, file);
	ret += freadLE(ys, file);
	ret += freadLE(n5, file);	//Type
	ret += freadLE(n6, file);	//SubType
	for(int i=0; i<16; ++i) ret += freadLE(off[i], file);
	for(int i=0; i<16; ++i) ret += freadLE(siz[i], file);
	if(ret != 38) {
	  fprintf(stderr, "ERROR: Failed to read BLP header (%d) on '%s'!\n",
		ret, filename.c_str());
	  type = SIMPLETEXTURE_NONE;
	  dirty = 0;
	  return;
	  }

	if(!uncomp) {
	//  fprintf(stderr, "It seems to be compressed\n");
	//  fprintf(stderr, "Size: %dx%d\n", xs, ys);
	//  fprintf(stderr, "With: %d %d %d\n", n2, n5, n6);
	//  for(int i=0; i<16; ++i) {
	//    fprintf(stderr, "Offset: %X  Size: %X\n", off[i], siz[i]);
	//    }
	  Uint32 jsize;
	  SDL_RWseek(file, 0x9C, SEEK_SET);
	  freadLE(jsize, file);
	  Uint8 *data = new Uint8[jsize + siz[0]];
	  SDL_RWread(file, (char *)data, 1, jsize);
	  SDL_RWseek(file, off[0], SEEK_SET);
	  SDL_RWread(file, ((char *)data)+jsize, 1, siz[0]);
	//  fprintf(stderr, "Sizes: (%d + %d) %d\n", jsize, siz[0], jsize+siz[0]);
	//  FILE *tmpf = fopen("debug.jpg", "wb");
	//  fwrite(data, 1, jsize+siz[0], tmpf);
	//  fclose(tmpf);
	  src = IMG_Load_RW(SDL_RWFromMem(data, jsize+siz[0]), true);
	  if(!src) {
	    fprintf(stderr, "WARNING[JPG]: %s\n", IMG_GetError());
	    fprintf(stderr, "Sorry, BLP '%s' failed to load!\n",
		filename.c_str());
	    fprintf(stderr, "Perhaps you haven't patched SDL_image?\n");
	    fprintf(stderr, "WARNING: File \"%s\"ignored!\n", filename.c_str());
	    }
	  delete [] data;
	  }
	else {
	  SDL_RWseek(file, 0x9C, SEEK_SET);
	  src = SDL_CreateRGBSurface(SDL_SWSURFACE, xs, ys, 32, ST_SDL_RGBA_COLFIELDS);
	  vector<SDL_Color> pal;
	  for(Uint32 ent=0; ent < 256; ++ent) {
	    pal.push_back(SDL_Color());
	    freadLE(pal.back().b, file);
	    freadLE(pal.back().g, file);
	    freadLE(pal.back().r, file);
	    SDL_RWseek(file, 1, SEEK_CUR); //Skip Unused Alpha
	    }
	  SDL_Rect pt = { 0, 0, 1, 1 };
	  Uint8 index[xs*ys], alpha[xs*ys];
	  int ret = SDL_RWread(file, index, 1, xs*ys);
	  ret += SDL_RWread(file, alpha, 1, xs*ys);
	  if(ret != int(xs*ys*2)) {
	    fprintf(stderr, "ERROR: Can't read pixel data from file '%s'\n",
		filename.c_str());
	    type = SIMPLETEXTURE_NONE;
	    dirty = 0;
	    return;
	    }
	  for(Uint32 pix=0; pix < xs*ys; ++pix) {
	    pt.x = pix%xs;
	    pt.y = pix/xs;
	    SDL_FillRect(src, &pt, SDL_MapRGBA(src->format, 
		pal[index[pix]].r, pal[index[pix]].g, pal[index[pix]].b,
		alpha[pix]));
	    }
	  }
	}
      else {
	SDL_RWseek(file, 0, SEEK_SET);
	}
      }
    if(!src) {
      src = IMG_Load_RW(file, true);
      }
    if(!src) {
      type = SIMPLETEXTURE_NONE;
      dirty = 0;
      }
    xfact = 1.0;
    yfact = 1.0;
    dirty = 1;
//    SDL_RWclose(file);	//Why does this generate a double-free???
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
  all_textures.erase(this);	// So I can no longer be invalidated!
  Clear();

//FIXME: This is static, need a ref counter.
//  if(fontfile) {                //Only close if WE opened
//    delete [] fontfile;
//    fontfile = NULL;
//    map<int, TTF_Font *>::iterator itrf = cur_font.begin();
//    for(; itrf != cur_font.end(); ++itrf) {
//      TTF_CloseFont(itrf->second);
//      }
//    }
//  cur_font.clear();
  }

void SimpleTexture::AttachTextData(TextData *in) {
  if(text) DetachTextData();
  if(!in) {
    text = new TextData;
    }
  else {
    text = in;
    }
  text->clients.insert(this);
  };

void SimpleTexture::DetachTextData() {
  if(text) {
    text->clients.erase(this);
    if(text->clients.size() <= 0) delete text;
    text = NULL;
    }
  }

void SimpleTexture::SetTexture(const SimpleTexture &in) {
  TextData *tmptxt = text;
  text = NULL;
  Clear();
  CopyFrom(in);
  if(text) DetachTextData();
  text = tmptxt;
  dirty = 1;
  }

void SimpleTexture::CopyFrom(const SimpleTexture &in) {
  Init(in.type);

  texturator = in.texturator;

  //WARNING! Shallow Copy
  cur = in.cur;
  src = in.src;
  col = in.col;
  fg = in.fg;
  xfact = in.xfact;
  yfact = in.yfact;
  dirty = in.dirty;

  //WARNING! Shallow Copy
  texture = in.texture;

  filename = in.filename;

  if(in.text) AttachTextData(in.text);

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

  if(text) DetachTextData();
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

void SimpleTexture::BuildTexture() {
  if((!text) || text->message.length() <= 0) {
    BuildBlankTexture();
    }
  else if(Font() == NULL) {
    fprintf(stderr, "WARNING: No Font Loaded!\n");
    BuildBlankTexture();
    }
  else {
    BuildTextTexture();
    }
  }

void SimpleTexture::BuildBlankTexture() {
  int xsize = 16, ysize = 16;

  if(CheckCache()) {
    return;
    }

  if(cur) {
    glFinish();
    SDL_FreeSurface(cur);
    }

  if(type == SIMPLETEXTURE_COLOR) {
    cur = SDL_CreateRGBSurface(SDL_SWSURFACE,
	xsize, ysize, 32, ST_SDL_RGBA_COLFIELDS);
    cur = texturator->Generate(cur, xsize, ysize, col);
    }
  else if(type == SIMPLETEXTURE_TRANSCOLOR) {
    //A TRANSCOLOR widget with no text is totally invisible.
    cur = SDL_CreateRGBSurface(SDL_SWSURFACE,
	xsize, ysize, 32, ST_SDL_RGBA_COLFIELDS);
    cur = invisible_texturator->Generate(cur, xsize, ysize, col);
    }
  else if(type == SIMPLETEXTURE_DEFINED) {
    xsize = nextpoweroftwo(src->w);
    ysize = nextpoweroftwo(src->h);
    cur = SDL_CreateRGBSurface(SDL_SWSURFACE,
	xsize, ysize, 32, ST_SDL_RGBA_COLFIELDS);
    memset(cur->pixels, 0, xsize*ysize*4);
    SDL_SetAlpha(src, 0, SDL_ALPHA_OPAQUE);
    SDL_BlitSurface(src, NULL, cur, NULL);
    xfact = (float)(src->w) / (float)(xsize);
    yfact = (float)(src->h) / (float)(ysize);
    }
  else if(type == SIMPLETEXTURE_TRANS) {
    xsize = nextpoweroftwo(src->w);
    ysize = nextpoweroftwo(src->h);
    cur = SDL_CreateRGBSurface(SDL_SWSURFACE,
	xsize, ysize, 32, ST_SDL_RGBA_COLFIELDS);
    SDL_SetAlpha(src, 0, SDL_ALPHA_TRANSPARENT);
    SDL_BlitSurface(src, NULL, cur, NULL);
    xfact = (float)(src->w) / (float)(xsize);
    yfact = (float)(src->h) / (float)(ysize);
    }
  }

void SimpleTexture::BuildTextTexture() {
  int bxsize = 0, bysize = 0, xsize = 0, ysize = 0, xoff = 0, yoff = 0;

  double ylines = text->visible_ylines;
  double xlines = text->visible_xlines;
  double fsz = (double)(TTF_FontLineSkip(Font(text->font_size)));

  if(ylines <= 0.0) ylines = text->lines.size();
  if(xlines == 0.0 && text->aspect_ratio != 0.0) {
    xlines = ylines * text->aspect_ratio;
    }
  else if(xlines <= 0.0) {
    xlines = double(text->text_xsize) / fsz;
    }

  if(type == SIMPLETEXTURE_DEFINED) {
    bxsize = src->w;
    bysize = src->h;

			//Used temporarilly - not final values
    xsize = int((float)(bxsize) * (1.0f - text->xmargin * 2.0f) + 0.5f);
    ysize = int((float)(bysize) * (1.0f - text->ymargin * 2.0f) + 0.5f);

    if(text->font_size != (int)((double)(ysize) / ylines + 0.5)) {
      SetTextFontSize((int)((double)(ysize) / ylines + 0.5));
      }

    if(text->text_xsize < xsize) {
      text->text_xsize = xsize;  //Needed for non-left alignments to work!
      }

    if(Font(text->font_size) == NULL) {
      fprintf(stderr, "WARNING: Couldn't resize font to ptsize %d\n", text->font_size);
      BuildBlankTexture();
      return;
      }

    xoff = (bxsize-xsize)/2;
    yoff = (bysize-ysize)/2;

    //OpenGL Needs a power of two size - grow to next
    xsize = nextpoweroftwo(bxsize);	// Final values
    ysize = nextpoweroftwo(bysize);

    xfact = (float)(bxsize) / (float)(xsize);
    yfact = (float)(bysize) / (float)(ysize);
    }
  else {
    bxsize = (int)(fsz * xlines + 0.5);
    bysize = (int)(fsz * ylines + 0.5);

    xsize = bxsize;	//Used temporarilly - not final values
    ysize = bysize;
    bxsize = int((float)(bxsize) / (1.0f - text->xmargin * 2.0f) + 0.5f);
    bysize = int((float)(bysize) / (1.0f - text->ymargin * 2.0f) + 0.5f);
    xoff = (bxsize-xsize)/2;
    yoff = (bysize-ysize)/2;

    //OpenGL Needs a power of two size - grow to next
    xsize = nextpoweroftwo(bxsize);	// Final values
    ysize = nextpoweroftwo(bysize);

    int max;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);

    xfact = (float)(bxsize) / (float)(xsize);
    yfact = (float)(bysize) / (float)(ysize);
    }

  if(cur) SDL_FreeSurface(cur);
  cur = NULL;

  if(type == SIMPLETEXTURE_COLOR) {
    cur = SDL_CreateRGBSurface(SDL_SWSURFACE,
	xsize, ysize, 32, ST_SDL_RGBA_COLFIELDS);
    cur = texturator->Generate(cur,
	Uint32(xsize*xfact), Uint32(ysize*yfact), col);
    }
  else if(type == SIMPLETEXTURE_TRANSCOLOR) {
    cur = SDL_CreateRGBSurface(SDL_SWSURFACE,
	xsize, ysize, 32, ST_SDL_RGBA_COLFIELDS);
    cur = invisible_texturator->Generate(cur,
	Uint32(xsize*xfact), Uint32(ysize*yfact), col);
    }
  else if(type == SIMPLETEXTURE_DEFINED) {
    cur = SDL_CreateRGBSurface(SDL_SWSURFACE,
	xsize, ysize, 32, ST_SDL_RGBA_COLFIELDS);
    SDL_SetAlpha(src, 0, SDL_ALPHA_OPAQUE);
    SDL_BlitSurface(src, NULL, cur, NULL);
    }
  else if(type == SIMPLETEXTURE_TRANS) {
    cur = SDL_CreateRGBSurface(SDL_SWSURFACE,
	xsize, ysize, 32, ST_SDL_RGBA_COLFIELDS);
    SDL_SetAlpha(src, 0, SDL_ALPHA_TRANSPARENT);
    SDL_BlitSurface(src, NULL, cur, NULL);
    }

  if(text->rendered_text == NULL) {	// Create persistent text surface
    SDL_Rect drec = { 0, 0, 0, 0 };
    text->rendered_text = SDL_CreateRGBSurface(SDL_SWSURFACE,
	text->text_xsize, text->text_ysize, 32, ST_SDL_RGBA_COLFIELDS);
    for(int ln = 0; ln < int(text->lines.size()); ++ln) {
      if(text->lines[ln].length() > 0) {
	SDL_Surface *tmp_text = TTF_RenderText_Blended(
		Font(text->font_size), text->lines[ln].c_str(), fg);
	if(!tmp_text) {
	  fprintf(stderr, "ERROR: Failed to render font: %s\n", TTF_GetError());
	  exit(1);
	  }

	if(text->alignment == ST_ALIGN_RIGHT) {
	  int xs = 0, ys = 0;
	  TTF_SizeText(Font(text->font_size), text->lines[ln].c_str(), &xs, &ys);
	  drec.x = text->text_xsize - xs;
	  }
	else if(text->alignment == ST_ALIGN_CENTER) {
	  int xs = 0, ys = 0;
	  TTF_SizeText(Font(text->font_size), text->lines[ln].c_str(), &xs, &ys);
	  drec.x = (text->text_xsize - xs)/2;
	  }

	SDL_SetAlpha(tmp_text, 0, SDL_ALPHA_TRANSPARENT);
	SDL_BlitSurface(tmp_text, NULL, text->rendered_text, &drec);
	SDL_FreeSurface(tmp_text);
	}
      drec.y += TTF_FontLineSkip(Font(text->font_size));
      }
    text->dirty = 0;
    set<SimpleTexture *>::iterator itr = text->clients.begin();
    for(; itr != text->clients.end(); ++itr) {
      (*itr)->dirty = 1;
      }
    }

  { SDL_Rect srec = { 0, 0, 0, 0}, drec = { xoff, yoff, 0, 0 };
    srec.x = (int)(fsz * text->visible_xoffset + 0.5);
    srec.w = (int)(fsz * xlines + 0.5);

    if(text->visible_yoffset >= 0.0) {
      srec.y = (int)(fsz * text->visible_yoffset + 0.5);
      srec.h = (int)(fsz * ylines + 0.5);
      }
    else {
      drec.y += (int)(fsz * -text->visible_yoffset + 0.5);
      srec.h = (int)(fsz * (ylines + ylines) + 0.5);
      }

    if(type == SIMPLETEXTURE_TRANS
		|| type == SIMPLETEXTURE_TRANSCOLOR) {
      SDL_SetAlpha(text->rendered_text, 0, SDL_ALPHA_TRANSPARENT);
      }

    if(srec.w < text->text_xsize) {
      if(text->alignment == ST_ALIGN_CENTER) {
	srec.x += ((text->text_xsize - srec.w) / 2);
	}
      else if(text->alignment == ST_ALIGN_RIGHT) {
	srec.x += (text->text_xsize - srec.w);
	}
      }
    else if(srec.w > text->text_xsize) {
      if(text->alignment == ST_ALIGN_CENTER) {
	drec.x -= ((text->text_xsize - srec.w) / 2);
	}
      else if(text->alignment == ST_ALIGN_RIGHT) {
	drec.x -= (text->text_xsize - srec.w);
	}
      }

    SDL_BlitSurface(text->rendered_text, &srec, cur, &drec);
    }
  }

void SimpleTexture::Update() {
  if(type != SIMPLETEXTURE_NONE) {
    BuildTexture();
    int xsize, ysize;
    if(src && (!cur)) {
      xsize = nextpoweroftwo(src->w);
      ysize = nextpoweroftwo(src->h);
      cur = SDL_CreateRGBSurface(SDL_SWSURFACE,
	xsize, ysize, 32, ST_SDL_RGBA_COLFIELDS);
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
    GLint res = 0;
    glGetTexLevelParameteriv(GL_PROXY_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &res);
    while(res == 0) {
      if(xsize < 1 || ysize < 1) {
	fprintf(stderr, "ERROR: Shrank image to %dx%d\n", xsize, ysize);
	exit(1);
	}
      if(xsize >= ysize) xsize /= 2;
      else ysize /= 2;
      glTexImage2D(GL_PROXY_TEXTURE_2D, 0, 4, xsize, ysize, 0, GL_RGBA,
	GL_UNSIGNED_BYTE, cur->pixels );
      glGetTexLevelParameteriv(GL_PROXY_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &res);
      }
    if(xsize != cur->w || ysize != cur->h) {
      SDL_Surface *tmp = cur;
      cur = SDL_CreateRGBSurface(SDL_SWSURFACE,
	xsize, ysize, 32, ST_SDL_RGBA_COLFIELDS);
      gluScaleImage(GL_RGBA, tmp->w, tmp->h, GL_UNSIGNED_BYTE, tmp->pixels,
	cur->w, cur->h, GL_UNSIGNED_BYTE, cur->pixels);
      SDL_FreeSurface(tmp);
      }

    if(texture == 0) glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);

    //FIXME: Don't use GLU, already resized and fitted to GL implementation!
    //FIXME: Not every texture needs an alpha!
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, cur->w, cur->h, 0,
//	GL_RGBA, GL_UNSIGNED_BYTE, cur->pixels);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, cur->w, cur->h,
	GL_RGBA, GL_UNSIGNED_BYTE, cur->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
	GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
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

void SimpleTexture::SetTextMargins(const float xmar, const float ymar) {
  if(!text) AttachTextData();
  text->xmargin = xmar;
  text->ymargin = ymar;

  if(text->rendered_text != NULL) SDL_FreeSurface(text->rendered_text);
  text->rendered_text = NULL;
  text->dirty = 1;
  }

void SimpleTexture::SetTextFontSize(const int sz) {
  if(!text) AttachTextData();
  text->font_size = sz;

  SetText(text->message);
  }

void SimpleTexture::SetText(const string txt) {
  if(!text) AttachTextData();

  text->message = txt;
  text->lines.clear();

  text->text_xsize = 0;
  int pos = 0, lpos = 0;
  while(lpos < (int)(text->message.length())) {
    pos = text->message.find('\n', lpos);
    if(pos < lpos) pos = text->message.length();
    text->lines.push_back(text->message.substr(lpos, pos - lpos));
    int xs = 0, ys = 0;
    TTF_SizeText(Font(text->font_size), text->lines.back().c_str(), &xs, &ys);
    if(text->text_xsize < xs) text->text_xsize = xs;
    lpos = pos+1;
    }
  text->text_ysize = TTF_FontLineSkip(Font(text->font_size)) * text->lines.size();

  if(text->rendered_text != NULL) SDL_FreeSurface(text->rendered_text);
  text->rendered_text = NULL;
  text->dirty = 1;
  }

void SimpleTexture::SetTextVisibleSize(const float ylines, const float xlines) {
  if(!text) AttachTextData();

  text->visible_ylines = ylines;
  text->visible_xlines = xlines;

  if(text->rendered_text != NULL) SDL_FreeSurface(text->rendered_text);
  text->rendered_text = NULL;
  text->dirty = 1;
  }

void SimpleTexture::SetTextAspectRatio(const double asp) {
  if(!text) AttachTextData();
  text->aspect_ratio = asp;

  if(text->rendered_text != NULL) SDL_FreeSurface(text->rendered_text);
  text->rendered_text = NULL;
  text->dirty = 1;
  }

void SimpleTexture::SetTextPosition(const float yoff, const float xoff) {
  if(!text) AttachTextData();

  text->visible_xoffset = xoff;
  text->visible_yoffset = yoff;

  if(text->rendered_text != NULL) SDL_FreeSurface(text->rendered_text);
  text->rendered_text = NULL;
  text->dirty = 1;
  }

void SimpleTexture::SetTextAlignment(int align) {
  if(!text) AttachTextData();

  text->alignment = align;

  if(text->rendered_text != NULL) SDL_FreeSurface(text->rendered_text);
  text->rendered_text = NULL;
  text->dirty = 1;
  }

GLuint SimpleTexture::GLTexture() {
  if(dirty || (text && text->dirty)) Update();
  return texture;
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

void SimpleTexture::SetFont(TTF_Font *font) {
  if(!TTF_WasInit()) {
    if(TTF_Init()) {
      fprintf(stderr, "ERROR: Unable to load font '%s' - %s\n",
	fontfile, TTF_GetError());
      exit(1);
      }
    atexit(TTF_Quit);
    }

  if(fontfile) delete [] fontfile;
  fontfile = NULL;
  cur_font[0] = font;
  fontyratio = 1.0;
  default_pxsize = 0;
  }

void SimpleTexture::SetDefaultFontSize(const int pxsz) {
  default_pxsize = pxsz;
  }

void SimpleTexture::LoadFont(const char *fontfn, const int pxsz) {
  if(fontfile) delete [] fontfile;
  fontfile = new char[strlen(fontfn)+1];
  strcpy(fontfile, fontfn);

  fontyratio = 1.0;
  int load_pxsz = 100;
  Font(load_pxsz); //Initialize font
  int act_height = TTF_FontLineSkip(cur_font[load_pxsz]);
  if(act_height != load_pxsz) { //Non-true-pixel font
    cur_font[act_height] = cur_font[load_pxsz];
    cur_font.erase(load_pxsz);
    fontyratio = (float)(load_pxsz) / (float)(act_height);
    Font(load_pxsz); //Create the REAL default font size
//    fprintf(stderr, "FontYRatio = %f\n", fontyratio);
    }

  SetDefaultFontSize(pxsz);
  }

TTF_Font *SimpleTexture::CurrentFont() {
  if(text) return Font(text->font_size);
  else return Font(default_pxsize);
  }

TTF_Font *SimpleTexture::Font(int pxsz) {
  if(pxsz < 1) pxsz = default_pxsize;
  if(cur_font.count(pxsz)) return cur_font[pxsz];
  else if(!fontfile) {
//    fprintf(stderr, "WARNING: attempt to resize font size with no loaded font!\n");
    return NULL;
    }

  if(!TTF_WasInit()) {
    if(TTF_Init()) {
      fprintf(stderr, "ERROR: Unable to load font '%s' - %s\n",
	fontfile, TTF_GetError());
      exit(1);
      }
    atexit(TTF_Quit);
    }

  int ptsz = (int)((float)(pxsz) * fontyratio + 0.5);   //Scale to real ptsize

  cur_font[pxsz] = TTF_OpenFont(fontfile, ptsz);
  if(!cur_font[pxsz]) {
    fprintf(stderr, "ERROR: Unable to load font '%s'!\n", fontfile);
    exit(1);
    }

  return cur_font[pxsz];
  }

map<SDL_Surface *, set<SimpleTexture *> > SimpleTexture::trans_cache;
map<SDL_Surface *, set<SimpleTexture *> > SimpleTexture::def_cache;

list<SDL_Surface *> SimpleTexture::trash_surf;
list<GLuint> SimpleTexture::trash_tex;

vector<SDL_Color> SimpleTexture::color;
SDL_Color SimpleTexture::default_text_color = {0, 0, 0, 0};
map<int, TTF_Font *> SimpleTexture::cur_font;
int SimpleTexture::default_pxsize;
float SimpleTexture::fontyratio = 1.0;
char *SimpleTexture::fontfile = NULL;

ST_Texturator *SimpleTexture::default_texturator = NULL;
ST_Texturator *SimpleTexture::invisible_texturator = NULL;

TextGeometry *SimpleTexture::GetTextGeometry() {	//FIXME: Temporary!
  static TextGeometry ret;
  if(text) {
    ret.visible_xlines = text->visible_xlines;
    ret.visible_ylines = text->visible_ylines;
    ret.aspect_ratio = text->aspect_ratio;
    ret.text_xsize = text->text_xsize;
    }
  else {
    ret.visible_xlines = ST_KEEPASPECT;
    ret.visible_ylines = ST_AUTOSIZE;
    ret.aspect_ratio = 0.0;
    ret.text_xsize = 0;
    }
  return &ret;
  }

void SimpleTexture::LinkTextTo(SimpleTexture *other) {
  if(!text) AttachTextData();
  other->AttachTextData(text);
  }

void SimpleTexture::LinkTextFrom(SimpleTexture *other) {
  if(!other->text) other->AttachTextData();
  AttachTextData(other->text);
  }

void SimpleTexture::SetTexturator(ST_Texturator *ttr) {
  if(ttr == NULL) texturator = default_texturator;
  else texturator = ttr;
  }

void SimpleTexture::NeedToReaquireContext(const int xsize, const int ysize) {
  new_xsize = xsize;
  new_ysize = ysize;
  need_to_reaquire = true;
  }

bool SimpleTexture::ReaquireNeeded(int &xsize, int &ysize) {
  if(need_to_reaquire) {
    xsize = new_xsize;
    ysize = new_ysize;
    return true;
    }
  return false;
  }

void SimpleTexture::ReaquireContext() {
  need_to_reaquire = false;
  set<SimpleTexture*>::iterator itr = all_textures.begin();
  for(; itr != all_textures.end(); ++itr) {
    (*itr)->dirty = 1;
    }
  }

float SimpleTexture::ScaleX(float base) {
  return base*xfact;
  }

float SimpleTexture::ScaleY(float base) {
  return base*yfact;
  }

set<SimpleTexture *> SimpleTexture::all_textures;
bool SimpleTexture::need_to_reaquire;
int SimpleTexture::new_xsize;
int SimpleTexture::new_ysize;


//All of the below is adapted from the zzliplib site:
//FILE: http://www.kekkai.org/roger/sdl/rwops/SDL_rwops_zzip.c
//Copyright (c) 2001 Guido Draheim <guidod@gmx.de>

#include "zzip/zzip.h"

static int _zzip_seek(SDL_RWops *context, int offset, int whence) {
  return zzip_seek((ZZIP_FILE*)(context->hidden.unknown.data1), offset, whence);
  }

static int _zzip_read(SDL_RWops *context, void *ptr, int size, int maxnum) {
  return (zzip_read((ZZIP_FILE*)(context->hidden.unknown.data1), (char*)ptr, size*maxnum) / size);
  }

static int _zzip_write(SDL_RWops *context, const void *ptr, int size, int num) {
  return 0; /* ignored */
  }

static int _zzip_close(SDL_RWops *context) {
  zzip_close ((ZZIP_FILE*)(context->hidden.unknown.data1));
  if(context) SDL_FreeRW(context);
  return 0;
  }

#ifndef O_BINARY
#define O_BINARY 0
#endif

SDL_RWops *SDL_RWFromZZIP(const char* file, const char* mode) {
  int mo = ZZIP_CASEINSENSITIVE;
  SDL_RWops *rwops;
  ZZIP_FILE *zzip_file;

  for (; *mode; ++mode) {
    switch (*mode) {
      case 'r': mo |= O_RDONLY;  break;
      case 'b': mo |= O_BINARY; break;
      case 'w': /* ouch! */  return 0;
      /* default, 't', 'a', etc, just ignore */
      }
    }

  zzip_file = zzip_open (file, mo);
  if(!zzip_file) return NULL;

  rwops = SDL_AllocRW();
  if(!rwops) { zzip_close(zzip_file); return NULL; }

  rwops->hidden.unknown.data1 = (void*)zzip_file;
  rwops->read = _zzip_read;
  rwops->write = _zzip_write;
  rwops->seek = _zzip_seek;
  rwops->close = _zzip_close;
  return rwops;
  }
