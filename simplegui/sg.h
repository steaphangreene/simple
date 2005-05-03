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

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_thread.h"

#include <map>
#include <vector>
using namespace std;

#define ASPECT_NO_SUPPORT	0
#define ASPECT_EXPANDING_X	1
#define ASPECT_EXPANDING_Y	2
#define ASPECT_FIXED_X		4
#define ASPECT_FIXED_Y		8

//Flags
#define SIMPLEGUI_NOMOUSE	1

#include "sg_colors.h"

class SG_Widget;
class SG_Alignment;
class SG_Editable;
class SG_Texture;

class SimpleGUI {
public:
  SimpleGUI(int aspmeth, float asp);
  ~SimpleGUI();

  static SimpleGUI *Current();
  static SimpleGUI *CurrentGUI() { return Current(); };	// Depricated

  bool RenderStart(unsigned long cur_time, bool ts = false);
  bool RenderFinish(unsigned long cur_time, bool ts = false);
  bool PollEvent(SDL_Event *event, bool ts = false);
  bool WaitEvent(SDL_Event *event, bool ts = false);

  SG_Alignment *MasterWidget() { return mWid; };

  //Popup Widgets MUST have solid backgrounds!
  SG_Alignment *PopupWidget() { return mWid; };
  void SetPopupWidget(SG_Alignment *wid, float px = 0.5, float py = 0.5,
	float posx = 0.0, float posy = 0.0);
  void SetModalPopupWidget(SG_Alignment *wid, float px = 0.5, float py = 0.5,
	float posx = 0.0, float posy = 0.0);
  void UnsetPopupWidget() { SetPopupWidget(NULL); };

  void SetCurrentWidget(SG_Widget *wid) { current_widget = wid; };
  void UnsetCurrentWidget() { SetCurrentWidget(NULL); };
  SG_Widget *CurrentWidget() { return current_widget; };

  void SetFocusWidget(SG_Widget *wid) { focus_widget = wid; };
  void UnsetFocusWidget() { SetFocusWidget(NULL); };
  SG_Widget *FocusWidget() { return focus_widget; };

  void LoadFont(const char *fontfn, int pxsz = 20);
  void SetDefaultFontSize(int pxsz);
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

  void SetMouseCursor(SDL_Surface *cur, float xsc = 0.0625, float ysc = 0.0625);

  void Lock() { SDL_mutexP(mutex); };		//For multithreaded access
  void Unlock() { SDL_mutexV(mutex); };		//For multithreaded access

  bool Render(unsigned long cur_time);		//Depricated
  bool ProcessEvent(SDL_Event *event);		//Depricated for external use!

  void DisableMouse() { flags |= SIMPLEGUI_NOMOUSE; };
  void EnableMouse() { flags &= (~SIMPLEGUI_NOMOUSE); };

protected:
  SG_Alignment *mWid, *popWid;
  float popx, popy;
  float popxpos, popypos;
  bool pop_modal;

  SG_Texture *mouse_cursor;
  float mouse_xscale, mouse_yscale;

  SG_Widget *current_widget;
  SG_Widget *focus_widget;

  int aspect_method;
  float aspect, aspect_actual;
  float newaspect_actual;

  int ScreenToRelative(float &x, float &y);
  int xunused, yunused;	//Current Screen Geometry
  int xsize, ysize;	//Current Screen Geometry
  int newxunused, newyunused;	//Next Screen Geometry
  int newxsize, newysize;	//Next Screen Geometry

  Uint32 flags;

  float mousex, mousey;	//Current Mouse Position

  unsigned long mb_state;	// Bit Vector of Mouse Button States
				// Can't handle more than 32 Buttons
  map<int, TTF_Font *> cur_font;
  int default_pxsize;
  float fontyratio;
  char *fontfile;

  vector<SDL_Color> col;
  SDL_Color text_col;

  SDL_mutex *Mutex();
  SDL_mutex *mutex;
  };

#endif	//SG_H
