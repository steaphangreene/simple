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

#ifndef SG_TABLE_H
#define SG_TABLE_H

#include "sg_alignment.h"

#include <vector>
using namespace std;

struct SG_TableGeometry {
  int xpos, ypos;	//Starting X/Y Position (Cell)
  int xsize, ysize;	//X/Y Size (in Cells)
  };

class SG_Table : public SG_Alignment {
public:
  SG_Table(int cls, int rws, float xbor = 0.0, float ybor = 0.0);
  virtual ~SG_Table();
  virtual bool HandleEvent(SDL_Event *event, float x, float y);
  virtual bool HandMouseEventTo(SG_Widget *targ, SDL_Event *event,
		float x, float y);
  virtual bool Render(unsigned long cur_time);
//  virtual bool SetDefaultCursor(GL_MODEL *cur);
  virtual bool AddWidget(SG_Widget *wid, int c1, int r1, int cs=1, int rs=1);
  virtual bool AddWidget(SG_Widget *wid);
  virtual void RemoveWidget(SG_Widget *wid);
  
protected:
  void CalcGeometry(const vector<SG_TableGeometry>::iterator &geom);

//  static GL_MODEL Default_Mouse_Cursor;
  int xsize, ysize;	//Geometry of Table
  int xpos, ypos;		//Current X/Y Position
  vector<SG_TableGeometry> wgeom;
  };

#endif // SG_TABLE_H

