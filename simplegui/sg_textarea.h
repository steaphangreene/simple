// *************************************************************************
//  This file is part of the SimpleGUI Example Module by Steaphan Greene
//
//  Copyright 2005-2014 Steaphan Greene <steaphan@gmail.com>
//
//  SimpleGUI is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 3 of the License, or
//  (at your option) any later version.
//
//  SimpleGUI is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SimpleGUI (see the file named "COPYING");
//  If not, see <http://www.gnu.org/licenses/>.
//
// *************************************************************************

#ifndef SG_TEXTAREA_H
#define SG_TEXTAREA_H

#include "sg_panel.h"
#include "sg_text.h"
#include "sg_ranger2d.h"

#include <vector>
#include <string>
using namespace std;

#ifndef SG_AUTOSIZE
#define SG_AUTOSIZE	ST_AUTOSIZE
#endif

#ifndef SG_KEEPASPECT
#define SG_KEEPASPECT	ST_KEEPASPECT
#endif

using namespace std;

class SG_TextArea : public SG_Panel, public SG_Text, public SG_Ranger2D {
public:
  SG_TextArea(const string mes = "", SimpleTexture tex = SG_COL_BG,
	SimpleTexture dis_tex = SG_COL_BG, float mx = 0.03125, float my = 0.125);
  virtual ~SG_TextArea();
  virtual int HandleEvent(SDL_Event *event, float x, float y);
//  virtual bool SetDefaultCursor(GL_MODEL *cur);
  void SetMargins(float xmar, float ymar);
  void SetFontSize(int sz);	// Ignored for defined-texture widgets
  virtual void Disable();
  virtual void Enable();

  void SetVisibleSize(float xs, float ys);
  void SetVisibleLines(int numlns);	//Depricated!

  virtual void SetAlignment(int align);

  virtual void SetText(const string &txt);
  virtual const string &Text();

  unsigned int NumLines();

  virtual void RangerChanged();

protected:
//  static GL_MODEL Default_Mouse_Cursor;

  void UpdateRange();

  string message;
  };

#endif // SG_TEXTAREA_H
