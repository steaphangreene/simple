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

// This file was created from (or actually IS) a basic compound widget
// def, so it's not defined and is really just a place-holder for now.

#ifndef SG_SPINNER_H
#define SG_SPINNER_H

#include <vector>
using namespace std;

#include "sg_compound.h"
#include "simpletexture.h"
#include "sg_colors.h"
#include "sg_text.h"
#include "sg_ranger.h"

class SG_Button;
class SG_TextArea;

class SG_Spinner : public SG_Compound, public SG_Ranger {
public:
  SG_Spinner(bool edit = false);
  virtual ~SG_Spinner();
//  virtual bool SetDefaultCursor(GL_MODEL *cur);
  virtual int HandleEvent(SDL_Event *event, float x, float y);
  virtual bool ChildEvent(SDL_Event *event);

  virtual void RangerChanged();

protected:
//  static GL_MODEL Default_Mouse_Cursor;
  SG_TextArea *text;
  SG_Button *upb, *downb;
  };

#endif // SG_SPINNER_H
