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

#ifndef	SG_H
#define	SG_H

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include <map>
#include <vector>
using namespace std;

#define ASPECT_NO_SUPPORT	0
#define ASPECT_EXPANDING_X	1
#define ASPECT_EXPANDING_Y	2
#define ASPECT_FIXED_X		4
#define ASPECT_FIXED_Y		8

#include "sg_colors.h"

class SG_Widget;
class SG_Alignment;
class SG_Editable;

class SimpleGUI {
public:
  SimpleGUI(int aspmeth, float asp);
  ~SimpleGUI();
  bool RenderStart(unsigned long cur_time);
  bool RenderFinish(unsigned long cur_time);
  bool Render(unsigned long cur_time);
  bool ProcessEvent(SDL_Event *event);

  SG_Alignment *MasterWidget() { return mWid; };

  //Popup Widgets MUST have solid backgrounds!
  SG_Alignment *PopupWidget() { return mWid; };
  void SetPopupWidget(SG_Alignment *wid, float px = 0.5, float py = 0.5);
  void UnsetPopupWidget() { popWid = NULL; };

  void SetCurrentWidget(SG_Widget *wid) { current_widget = wid; };
  void UnsetCurrentWidget() { SetCurrentWidget(NULL); };
  SG_Widget *CurrentWidget() { return current_widget; };

  void SetFocusWidget(SG_Widget *wid) { focus_widget = wid; };
  void UnsetFocusWidget() { SetFocusWidget(NULL); };
  SG_Widget *FocusWidget() { return focus_widget; };

  void LoadFont(const char *fontfn, int pxsz = 20);
  void SetFont(TTF_Font *f);
  void UnsetFont() { SetFont(NULL); };
  TTF_Font *Font(int pxsz = -1);

  float Red(int c);
  float Green(int c);
  float Blue(int c);
  float TextRed(int c);
  float TextGreen(int c);
  float TextBlue(int c);
  const SDL_Color *BGColor(int c);
  const SDL_Color *TextColor(int c);
  void SetColor(int c, float r, float g, float b,
	float tr = -1.0, float tg = -1.0, float tb = -1.0);
  int NewColor();
  int NewColor(float r, float g, float b,
	float tr = -1.0, float tg = -1.0, float tb = -1.0);
  void SetDefaultTextColor(float tr, float tg, float tb);
  const SDL_Color *DefaultTextColor();

  void GetMousePos(float &mx, float &my) { mx = mousex; my = mousey; };

protected:
  SG_Alignment *mWid, *popWid;
  float popx, popy;
  int aspect_method;
  float aspect;
  float aspect_actual;
  SG_Widget *current_widget;
//  GL_MODEL *mouse_cursor;
  SG_Widget *focus_widget;

  int ScreenToRelative(float &x, float &y);
  int xunused, yunused;	//Current Screen Geometry
  int xsize, ysize;	//Current Screen Geometry

  float mousex, mousey;	//Current Mouse Position

  unsigned long mb_state;	// Bit Vector of Mouse Button States
				// Can't handle more than 32 Buttons
  map<int, TTF_Font *> cur_font;
  int default_pxsize;
  float fontyratio;
  char *fontfile;

  vector<SDL_Color> col;
  SDL_Color text_col;
  };

#endif	//SG_H

