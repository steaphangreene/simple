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

#include "sg_alignment.h"

SG_Widget::SG_Widget() {
  flags = 0;
  }

SG_Widget::~SG_Widget() {
  }

bool SG_Widget::HandleKeyboardEvent(SDL_Event *event) {
  return 1;
  }

bool SG_Widget::HandleMouseEvent(SDL_Event *event, float x, float y) {
  return 1;
  }

bool SG_Widget::HandMouseEventTo(SG_Widget *targ, SDL_Event *event,
		float x, float y) {
  if(targ == this) return HandleMouseEvent(event, x, y);
  return 1;
  }

bool SG_Widget::HasWidget(SG_Widget *targ) {
  return this == targ;
  }

bool SG_Widget::Render(unsigned long cur_time) {
  return 1;
  }


//  bool SG_Widget::SetDefaultCursor(GL_MODEL *cur);
//  bool SG_Widget::SetCursor(GL_MODEL *cur)
//  bool SG_Widget::UnsetCursor()
  
//  static GL_MODEL SG_Widget::Default_Mouse_Cursor = NULL;

