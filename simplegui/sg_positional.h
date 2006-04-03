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

#ifndef SG_POSITIONAL_H
#define SG_POSITIONAL_H

#include "sg_alignment.h"
#include "sg_ranger2d.h"

#include <vector>
using namespace std;

class SG_Positional : public SG_Alignment {
public:
  SG_Positional();
  virtual ~SG_Positional();
  virtual int HandleEvent(SDL_Event *event, float x, float y);
  virtual bool HandEventTo(SG_Widget *targ, SDL_Event *event,
		float x, float y);
  virtual bool Render(unsigned long cur_time);
//  virtual bool SetDefaultCursor(GL_MODEL *cur);

  virtual bool AddWidget(SG_Widget *wid,
	const float xoff = 0.0, const float yoff = 0.0,
	const float xsize = 0.5, const float ysize = 0.5,
	bool modal = false);

  virtual void RemoveWidget(SG_Widget *wid);

  virtual void SetAspectRatio(const double asp);

protected:
  vector<SG_AlignmentGeometry> wgeom;
  void CalcGeometry(const vector<SG_AlignmentGeometry>::iterator &geom);

  SG_Ranger2D ranger;

//  static GL_MODEL Default_Mouse_Cursor;
  };

#endif // SG_POSITIONAL_H
