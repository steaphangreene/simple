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

// Rob W. Brooks (rob@atomicpenguin.net) is to be blamed for any problems
// with this module in its mature state.

#ifndef SG_LISTBOX_H
#define SG_LISTBOX_H

#include <set>
#include <deque>
#include <map>
using namespace std;

#include "sg_compound.h"
#include "sg_texture.h"
#include "sg_panel.h"
#include "sg_stickybutton.h"

class SG_Button;
class SG_TextArea;

class SG_ListBox : public SG_Compound {
public:
  SG_ListBox(const vector<SG_Widget*>& items, SG_Texture desel, 
		  SG_Texture sel, SG_Texture click, SG_Texture disable, 
		  unsigned int min=1, unsigned int max=1,
		  bool vert=true, float border=0.1);
  virtual ~SG_ListBox();

  bool SetSelection(const set<int>& toselect);
  
//  virtual bool SetDefaultCursor(GL_MODEL *cur);
  virtual bool ChildEvent(SDL_Event *event);

  deque<int> Which() { return selhistory; }
  
 
protected:
  void Select(int ind);
  bool Deselect(bool override=false, int ind=-1);

//  static GL_MODEL Default_Mouse_Cursor;
  bool vertical;
  unsigned int listsize;
  unsigned int minsel,maxsel;
  deque<int> selhistory;
  vector<SG_StickyButton*> stickies;
  vector<SG_Alignment*> aligns;
  map<SG_StickyButton*, int> ptr2pos; 
  };

#endif // SG_LISTBOX_H
