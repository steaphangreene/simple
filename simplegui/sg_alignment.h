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

#ifndef SG_ALIGNMENT_H
#define SG_ALIGNMENT_H

#include "sg_widget.h"

#include <vector>
using namespace std;

struct SG_AlignmentGeometry {
  float xp, yp; //Relative Position (Center)
  float xs, ys; //Relative Size (From Center)
  };

class SG_Alignment : public SG_Widget {
public:
  SG_Alignment(float xbor = 0.0, float ybor = 0.0);
  virtual ~SG_Alignment();
  void SetBorder(float xbor, float ybor);
  virtual bool HandleEvent(SDL_Event *event, float x, float y);
  virtual bool HandEventTo(SG_Widget *targ, SDL_Event *event,
		float x, float y);
  virtual bool HasWidget(SG_Widget *targ);
  virtual bool Render(unsigned long cur_time);
//  virtual bool SetDefaultCursor(GL_MODEL *cur);
  virtual bool AddWidget(SG_Widget *wid);
  virtual void RemoveWidget(SG_Widget *wid);
  void SetBackground(SG_Widget *wid) { background = wid; };
  void UnsetBackground() { background = NULL; };
  
protected:
  void CalcGeometry();
  SG_AlignmentGeometry cur_geom;

//  static GL_MODEL Default_Mouse_Cursor;
  vector<SG_Widget *> widgets;
  SG_Widget *background;
  float xborder, yborder;
  };

#endif // SG_ALIGNMENT_H

