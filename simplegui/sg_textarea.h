// *************************************************************************
//  This file is part of the SimpleGUI Example Module by Steaphan Greene
//
//  Copyright 2005-2006 Steaphan Greene <stea@cs.binghamton.edu>
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
#include "sg_ranger2d.h"

#include <vector>
#include <string>
using namespace std;

#define SG_ALIGN_LEFT	ST_ALIGN_LEFT
#define SG_ALIGN_CENTER	ST_ALIGN_CENTER
#define SG_ALIGN_RIGHT	ST_ALIGN_RIGHT

#ifndef SG_AUTOSIZE
#define SG_AUTOSIZE	-1
#endif

using namespace std;

class SG_TextArea : public SG_Panel, public SG_Text, public SG_Ranger2D {
public:
  SG_TextArea(string mes, SimpleTexture tex = SG_COL_BG,
	SimpleTexture dis_tex = SG_COL_BG, float mx = 0.125, float my = 0.125);
  virtual ~SG_TextArea();
  virtual int HandleEvent(SDL_Event *event, float x, float y);
//  virtual bool SetDefaultCursor(GL_MODEL *cur);
  void SetMargins(float xmar, float ymar);
  void SetFontSize(int sz);	// Ignored for defined-texture widgets
  virtual void Disable();
  virtual void Enable();

  void SetVisibleSize(double xs, double ys);
  void SetVisibleLines(int numlns);	//Depricated!

  void SetAlignment(int align);

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
