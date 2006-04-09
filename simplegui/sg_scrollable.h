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

#ifndef SG_SCROLLABLE_H
#define SG_SCROLLABLE_H

#include "sg_alignment.h"
#include "sg_ranger2d.h"

#include <vector>
using namespace std;

class SG_Table;
class SG_TextArea;

class SG_Scrollable : public SG_Alignment, public SG_Ranger2D {
public:
  SG_Scrollable(float xspn, float yspn, float xoff = 0.0, float yoff = 0.0,
	float xmin = 0.0, float ymin = 0.0, float xmax = 1.0, float ymax = 1.0);
  virtual ~SG_Scrollable();
  virtual int HandleEvent(SDL_Event *event, float x, float y);
  virtual bool HandEventTo(SG_Widget *targ, SDL_Event *event,
		float x, float y);
//  virtual bool SetDefaultCursor(GL_MODEL *cur);

  virtual bool AddWidget(SG_Widget *wid);
  virtual bool AddWidget(SG_Table *tab);	//Special Handler for Tables
  virtual bool AddWidget(SG_TextArea *text);	//Special Handler for TextAreas

  virtual void SetAspectRatio(const double asp);

  virtual void RangerChanged();

protected:
  virtual bool RenderSelf(unsigned long cur_time);

  void CalcGeometry();
  bool subwidget_handles;

//  static GL_MODEL Default_Mouse_Cursor;
  };

#endif // SG_SCROLLABLE_H

