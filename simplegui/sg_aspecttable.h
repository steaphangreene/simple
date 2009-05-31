// *************************************************************************
//  This file is part of the SimpleGUI Example Module by Steaphan Greene
//
//  Copyright 2005-2008 Steaphan Greene <stea@cs.binghamton.edu>
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

#ifndef SG_ASPECTTABLE_H
#define SG_ASPECTTABLE_H

#include "sg_table.h"

#define SG_CENTER	0x00
#define SG_LEFT		0x01
#define SG_RIGHT	0x02
#define SG_UP		0x10
#define SG_UP_LEFT	0x11
#define SG_UP_RIGHT	0x12
#define SG_DOWN		0x20
#define SG_DOWN_LEFT	0x21
#define SG_DOWN_RIGHT	0x22

class SG_AspectTable : public SG_Table {
public:
  SG_AspectTable(float aspect, int xsz, int ysz, float xbor = 0.0, float ybor = 0.0);
  virtual ~SG_AspectTable();
  virtual int HandleEvent(SDL_Event *event, float x, float y);
  virtual bool HandEventTo(SG_Widget *targ, SDL_Event *event,
		float x, float y);
  virtual bool AddWidget(SG_Widget *wid, int x1, int y1, int grav=SG_CENTER);
  virtual bool AddWidget(SG_Widget *wid, int x1, int y1, int xs, int ys,
	int grav=SG_UP_LEFT);
//  virtual bool SetDefaultCursor(GL_MODEL *cur);
  virtual bool RenderSelf(unsigned long cur_time);
  
protected:
//  static GL_MODEL Default_Mouse_Cursor;
  vector<SG_TableGeometry> wgrav;
  float fixed_aspect;
  };

#endif // SG_ASPECTTABLE_H

