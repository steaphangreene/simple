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

// This file was created from (or actually IS) a basic compound widget
// def, so it's not defined and is really just a place-holder for now.

#ifndef SG_SCROLLINGAREA_H
#define SG_SCROLLINGAREA_H

#include "sg_compound.h"
#include "sg_ranger2d.h"
#include "sg_scrollable.h"
#include "sg_sliderbar.h"

#ifndef SG_AUTOSIZE
#define SG_AUTOSIZE -1
#endif

class SG_Button;
class SG_TextArea;

class SG_ScrollingArea : public SG_Compound, public SG_Ranger2D {
 public:
  SG_ScrollingArea(float xvs = SG_AUTOSIZE, float yvs = SG_AUTOSIZE,
                   float xsz = SG_AUTOSIZE, float ysz = SG_AUTOSIZE);
  virtual ~SG_ScrollingArea();
  //  virtual bool SetDefaultCursor(GL_MODEL *cur);
  virtual int HandleEvent(SDL_Event *event, float x, float y);
  virtual bool ChildEvent(SDL_Event *event);

  virtual bool AddWidget(SG_Widget *wid);
  virtual void RemoveWidget(SG_Widget *wid);
  virtual bool AddWidget(SG_Table *tab);      // Special Handler for Tables
  virtual bool AddWidget(SG_TextArea *text);  // Special Handler for TextAreas

 protected:
  //  static GL_MODEL Default_Mouse_Cursor;
  SG_Scrollable *scroll;
  SG_SliderBar *vert, *horiz;
};

#endif  // SG_SCROLLINGAREA_H
