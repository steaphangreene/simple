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

#ifndef SG_EDITABLE_H
#define SG_EDITABLE_H

#include "sg_textarea.h"

class SG_Editable : public SG_TextArea {
public:
  SG_Editable(string mes, SG_Texture c = SG_COL_LOW, 
	SG_Texture dc = SG_COL_BG, SG_Texture fc = SG_COL_HIGH);
  virtual ~SG_Editable();
  virtual bool HandleKeyboardEvent(SDL_Event *event);
  virtual bool HandleMouseEvent(SDL_Event *event, float x, float y);
  virtual bool Render(unsigned long cur_time);
//  virtual bool SetDefaultCursor(GL_MODEL *cur);
  
protected:
//  static GL_MODEL Default_Mouse_Cursor;
  };

#endif // SG_EDITABLE_H

