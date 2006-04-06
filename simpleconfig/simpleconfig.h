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

#ifndef SIMPLECONFIG_H
#define SIMPLECONFIG_H

#include <string>
#include <vector>
using namespace std;

#include "../simplegui/sg_multitab.h"

class SG_Button;
class SG_TextArea;
class SG_Editable;

class SimpleConfig : public SG_MultiTab {
public:
  SimpleConfig();
  virtual ~SimpleConfig();

  virtual bool Render(unsigned long cur_time);
  //  virtual bool SetDefaultCursor(GL_MODEL *cur);
  virtual bool ChildEvent(SDL_Event *event);

protected:
//  static GL_MODEL Default_Mouse_Cursor;
  };

#endif // SIMPLECONFIG_H
