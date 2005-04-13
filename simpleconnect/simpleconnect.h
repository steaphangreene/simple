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

// This file was created from (or actually IS) a basic compound widget
// def, so it's not defined and is really just a place-holder for now.

#ifndef SIMPLECONNECT_H
#define SIMPLECONNECT_H

#include <string>
#include <vector>
using namespace std;

#include "../simplegui/sg_compound.h"

class SG_Button;
class SG_TextArea;

enum SC_SlotType {
  SC_SLOT_NONE = 0,
  SC_SLOT_SPEC,
  SC_SLOT_LOCAL,
  SC_SLOT_PLAYER,
  SC_SLOT_AI,
  SC_SLOT_MAX
  };

class SimpleConnect : public SG_Compound {
public:
  SimpleConnect(const vector<SC_SlotType> &slts = vector<SC_SlotType>());
  virtual ~SimpleConnect();
  //  virtual bool SetDefaultCursor(GL_MODEL *cur);
  virtual bool ChildEvent(SDL_Event *event);

  void EnableNetwork(const string &tag);
  
protected:
//  static GL_MODEL Default_Mouse_Cursor;
  SG_Button *okb;
  vector<SC_SlotType> slots;

  bool network;
  string nettag;
  };

#endif // SIMPLECONNECT_H
