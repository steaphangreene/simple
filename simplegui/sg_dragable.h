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

#ifndef SG_DRAGABLE_H
#define SG_DRAGABLE_H

#include "sg_panel.h"
#include "sg_ranger2d.h"

class SG_Dragable : public SG_Panel, public SG_Ranger2D {
public:
  SG_Dragable(SimpleTexture tex = SG_COL_RAISED);
  virtual ~SG_Dragable();
  virtual int HandleEvent(SDL_Event *event, float x, float y);
//  virtual bool SetDefaultCursor(GL_MODEL *cur);

  void SetDisplayLimits(float mnx, float mny, float mxx, float mxy);
  void SetXDisplayLimits(float mnx, float mxx);
  void SetYDisplayLimits(float mny, float mxy);

  void SetLabel(SG_Widget *lab);

  virtual void SetAspectRatio(double asp);

	//Internal Use Only!  For self-reconfiguring widgets.
  virtual void AdjustGeometry(SG_AlignmentGeometry *geom);
  
protected:
  virtual bool RenderSelf(unsigned long cur_time);

//  static GL_MODEL Default_Mouse_Cursor;
  float base_x, base_y, off_x, off_y, start_x, start_y;

  float min_dx, min_dy, max_dx, max_dy;	//Display Limits
  void Disp2Limits(float &x, float &y);
  void Limits2Disp(float &x, float &y);

  SG_Widget *label;
  };

#endif // SG_DRAGABLE_H
