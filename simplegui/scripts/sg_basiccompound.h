// *************************************************************************
//  This file is part of the SimpleGUI Example Module by Steaphan Greene
//
//  Copyright 2005-2015 Steaphan Greene <steaphan@gmail.com>
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

#ifndef SG_BASICCOMPOUND_H
#define SG_BASICCOMPOUND_H

#include "sg_compound.h"

class SG_Button;
class SG_TextArea;

class SG_BasicCompound : public SG_Compound {
 public:
  SG_BasicCompound();
  virtual ~SG_BasicCompound();
  //  virtual bool SetDefaultCursor(GL_MODEL *cur);
  virtual bool ChildEvent(SDL_Event *event);

 protected:
  //  static GL_MODEL Default_Mouse_Cursor;
  SG_Button *okb;
};

#endif  // SG_BASICCOMPOUND_H
