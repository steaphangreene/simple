// *************************************************************************
//  This file is part of the SimpleTexture Example Module by Steaphan Greene
//
//  Copyright 2005-2006 Steaphan Greene <stea@cs.binghamton.edu>
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

#ifndef ST_H
#define ST_H

#include <map>
#include <set>
#include <list>
#include <vector>
#include <string>
using namespace std;

#include "SDL.h"

#include "stt.h"

#define ST_ALIGN_LEFT	0
#define ST_ALIGN_CENTER	1
#define ST_ALIGN_RIGHT	2

#define ST_AUTOSIZE	-1
#define ST_KEEPASPECT	0

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

struct TextGeometry {	//FIXME: Only needed temporarily!
  double visible_xlines;
  double visible_ylines;
  double aspect_ratio;
  int text_xsize;
  };

class SimpleTexture {
public:
  SimpleTexture(SDL_Surface *tex);
  SimpleTexture(const string &filenm);
  SimpleTexture(int col_index);
  SimpleTexture(const SimpleTexture &in);
  ~SimpleTexture();

// Tell us to prep to renew ourselves
  static void NeedToReaquireContext(const int xsize, const int ysize);
// Are we preparing?
  static bool ReaquireNeeded(int &xsize, int &ysize);
// Go ahead and do it!
  static void ReaquireContext();

  const SimpleTexture &operator = (const SimpleTexture &in);
  void SetTexture(const SimpleTexture &in);

  bool CheckCache();
  void Update();

  TextGeometry *GetTextGeometry();

  void SetText(const string txt);
  void SetTextAspectRatio(const double asp);
  void SetTextVisibleSize(const float ylines=-1.0, const float xlines=-1.0);
  void SetTextPosition(const float yoff=0.0, const float xoff=0.0);
  void SetTextMargins(const float xmar, const float ymar);
  void SetTextAlignment(int align);
  void SetTextFontSize(const int sz);
  TTF_Font *CurrentFont();	// FIXME: Make this const somehow?

  void LinkTextTo(SimpleTexture *other);
  void LinkTextFrom(SimpleTexture *other);

  GLuint GLTexture();

  float ScaleX(float base);
  float ScaleY(float base);
  float xfact, yfact;	// Portion of texture actually shown
			// Using these publically is depricated!
			// You should use ScaleX and ScaleY instead!

  //FIXME: All this should be protected!
  SimpleTextureType type;
  SDL_Surface *cur;	//Current texture buffer
  SDL_Surface *src;	//Only for SIMPLETEXTURE_DEFINED/SIMPLETEXTURE_FILE
  SDL_Color col;	//Only for SIMPLETEXTURE_COLOR
  SDL_Color fg;		//Text Color
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

  void SetTexturator(ST_Texturator *ttr);

  //For Internal Use Only!
  static void EmptyTrash();

protected:
  struct TextData;
  TextData *text;

  ST_Texturator *texturator;
  static ST_Texturator *default_texturator;
  static ST_Texturator *invisible_texturator;

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

  static set<SimpleTexture *> all_textures;	//For invalidation
  static bool need_to_reaquire;
  static int new_xsize, new_ysize;
  };

//Exported Utility Functions
SDL_RWops *SDL_RWFromZZIP(const char* file, const char* mode);

#endif // ST_H
