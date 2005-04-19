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

#ifndef SG_TEXTAREA_H
#define SG_TEXTAREA_H

#include "sg_panel.h"
#include "sg_text.h"

#include <string>

using namespace std;

class SG_TextArea : public SG_Panel, public SG_Text {
public:
  SG_TextArea(string mes, SG_Texture tex = SG_COL_BG,
	SG_Texture dis_tex = SG_COL_BG, float mx = 0.125, float my = 0.125);
  virtual ~SG_TextArea();
  virtual int HandleEvent(SDL_Event *event, float x, float y);
//  virtual bool SetDefaultCursor(GL_MODEL *cur);
  void SetMargins(float xmar, float ymar);
  void SetVisibleLines(int numlns);
  void SetFontSize(int sz);	// Ignored for defined-texture widgets
  virtual void Disable();
  virtual void Enable();

  virtual void SetText(const string &txt);
  virtual const string &Text();
  
protected:
//  static GL_MODEL Default_Mouse_Cursor;

  virtual void BuildTexture(int st);

  float xmargin, ymargin;
  int visible_lines;
  string message;
  int font_size;
  };

#endif // SG_TEXTAREA_H
