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

#ifndef SG_EDITABLE_H
#define SG_EDITABLE_H

#include "sg_textarea.h"

class SG_Editable : public SG_TextArea {
public:
  SG_Editable(const string mes = "", SimpleTexture c = SG_COL_LOW,
	SimpleTexture dc = SG_COL_BG, SimpleTexture fc = SG_COL_HIGH);
  virtual ~SG_Editable();
  virtual int HandleEvent(SDL_Event *event, float x, float y);
//  virtual bool SetDefaultCursor(GL_MODEL *cur);

protected:
  virtual bool RenderSelf(unsigned long cur_time);

//  static GL_MODEL Default_Mouse_Cursor;
  };

#endif // SG_EDITABLE_H

