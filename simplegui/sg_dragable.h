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

#ifndef SG_DRAGABLE_H
#define SG_DRAGABLE_H

#include "sg_panel.h"

class SG_Dragable : public SG_Panel {
public:
  SG_Dragable(SG_Texture tex = SG_COL_RAISED);
  virtual ~SG_Dragable();
  void SetLimits(float mnx, float mny, float mxx, float mxy);
  virtual bool HandleEvent(SDL_Event *event, float x, float y);
  virtual bool Render(unsigned long cur_time);
//  virtual bool SetDefaultCursor(GL_MODEL *cur);
  
protected:
//  static GL_MODEL Default_Mouse_Cursor;
  float base_x, base_y;
  float off_x, off_y;
  float max_x, min_x, max_y, min_y;
  };

#endif // SG_DRAGABLE_H

