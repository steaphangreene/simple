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

#ifndef SG_COMPOUND_H
#define SG_COMPOUND_H

#include "sg_table.h"

class SG_Compound : public SG_Table {
public:
  SG_Compound(int xsz, int ysz, float xbor = 0.0, float ybor = 0.0);
  virtual ~SG_Compound();
  virtual bool HandleEvent(SDL_Event *event, float x, float y);
  virtual bool HandEventTo(SG_Widget *targ, SDL_Event *event,
		float x, float y);
//  virtual bool SetDefaultCursor(GL_MODEL *cur);
  virtual bool ChildEvent(SDL_Event *event);
  
protected:
//  static GL_MODEL Default_Mouse_Cursor;
  };

#endif // SG_COMPOUND_H

