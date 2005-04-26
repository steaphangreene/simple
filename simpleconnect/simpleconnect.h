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
  SC_SLOT_PLAYER,
  SC_SLOT_LOCALONLY,
  SC_SLOT_AIONLY,
  SC_SLOT_SPEC,
  SC_SLOT_MAX
  };

enum SC_PlayerType {
  SC_PLAYER_NONE = 0,
  SC_PLAYER_SPEC,
  SC_PLAYER_LOCAL,
  SC_PLAYER_AI,
  SC_PLAYER_REMOTE,
  SC_PLAYER_MAX
  };

enum SC_Mode {
  SC_MODE_NONE = 0,
  SC_MODE_HOST,
  SC_MODE_SEARCH,
  SC_MODE_SLAVE,
  SC_MODE_CONFIG,
  SC_MODE_MAX
  };

#define SC_FLAG_COLOR
#define SC_FLAG_TEAM

class SimpleConnect : public SG_Compound {
public:
  SimpleConnect();
  virtual ~SimpleConnect();
  //  virtual bool SetDefaultCursor(GL_MODEL *cur);
  virtual bool ChildEvent(SDL_Event *event);

  void Host(const string &tag, const vector<SC_SlotType> &slts);
  void Search(const string &tag);
  void Config(const vector<SC_SlotType> &slts);
  
protected:
//  static GL_MODEL Default_Mouse_Cursor;
  SG_Button *okb;
  vector<SC_SlotType> slots;

  string nettag;
  SC_Mode mode;
  unsigned int prop_flags, change_flags;
  };

#endif // SIMPLECONNECT_H
