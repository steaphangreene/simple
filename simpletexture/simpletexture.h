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
#include <list>
#include <vector>
#include <string>
using namespace std;

#include "SDL.h"

#define ST_ALIGN_LEFT   0
#define ST_ALIGN_CENTER 1
#define ST_ALIGN_RIGHT  2

#define ST_AUTOSIZE     -1

struct _TTF_Font;
typedef struct _TTF_Font TTF_Font;

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
  SimpleTexture(int col_index);
  SimpleTexture(const SimpleTexture &in);
  ~SimpleTexture();

  const SimpleTexture &operator = (const SimpleTexture &in);
  void SetTexture(const SimpleTexture &in);

  bool CheckCache();
  void Update();

  void SetText(const string txt,
	const float ylines=-1.0, const float xlines=-1.0,
	const float yoff=0.0, const float xoff=0.0);
  void SetMargins(const float xmar, const float ymar);
  void SetFontSize(const int sz);

  GLuint GLTexture();

  SimpleTextureType type;
  SDL_Surface *cur;	//Current texture buffer
  SDL_Surface *src;	//Only for SIMPLETEXTURE_DEFINED/SIMPLETEXTURE_FILE
  SDL_Color col;	//Only for SIMPLETEXTURE_COLOR
  SDL_Color fg;		//Text Color
  float xfact, yfact;	//Portion of texture actually shown
  bool dirty; //Does the system need to rebuild this texture?

  //Color Code (originally from SimpleGUI)
  static float Red(const int c);
  static float Green(const int c);
  static float Blue(const int c);
  static float TextRed(const int c);
  static float TextGreen(const int c);
  static float TextBlue(const int c);
  static const SDL_Color *BGColor(const int c);
  static const SDL_Color *TextColor(const int c);
  static void SetColor(const int c, const SDL_Color &col,
	const SDL_Color &text = default_text_color);
  static void SetColor(const int c, const float r, const float g, const float b,
	const float tr = -1.0, const float tg = -1.0, const float tb = -1.0);
  static int NewColor();
  static int NewColor(const float r, const float g, const float b,
	const float tr = -1.0, const float tg = -1.0, const float tb = -1.0);
  static void SetDefaultTextColor(
	const float tr, const float tg, const float tb
	);
  static const SDL_Color *DefaultTextColor();

  //Font Code (originally from SimpleGUI)
  static void LoadFont(const char *fontfn, int pxsz = 20);
  static void SetDefaultFontSize(const int pxsz);
  static void SetFont(TTF_Font *f);
  static void UnsetFont() { SetFont(NULL); };
  static TTF_Font *Font(const int pxsz = -1);

  //For Internal Use Only!
  static void EmptyTrash();

protected:
  struct TextData;
  TextData *text;

  void Init(const SimpleTextureType tp);
  static void InitStatic();

  void BuildTexture();
  void BuildBlankTexture();
  void BuildTextTexture();

  void AttachTextData(TextData *in = NULL);
  void DetachTextData();

  void Clear();
  void CopyFrom(const SimpleTexture &in);

  GLuint texture;	//Current texture (when active)

  void UpdateCache();

  string filename;

  static map<SDL_Surface *, set<SimpleTexture *> > trans_cache;
  static map<SDL_Surface *, set<SimpleTexture *> > def_cache;

  static list<SDL_Surface *> trash_surf;
  static list<GLuint> trash_tex;

  static vector<SDL_Color> color;
  static SDL_Color default_text_color;
  static map<int, TTF_Font *> cur_font;
  static int default_pxsize;
  static float fontyratio;
  static char *fontfile;
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

#endif // SIMPLETEXTURE_H
