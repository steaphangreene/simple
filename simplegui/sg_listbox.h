// *************************************************************************
//  This file is part of the SimpleGUI Example Module by Steaphan Greene
//
//  Copyright 2005-2008 Steaphan Greene <stea@cs.binghamton.edu>
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
#include "simpletexture.h"
#include "sg_panel.h"
#include "sg_stickybutton.h"

class SG_Button;
class SG_TextArea;

class SG_ListBox : public SG_Compound {
public:
  SG_ListBox(const vector<SG_Widget*>& items, SimpleTexture desel, 
		  SimpleTexture sel, SimpleTexture click, SimpleTexture disable, 
		  unsigned int min=1, unsigned int max=1,
		  bool vert=true, float border=0.1);
  virtual ~SG_ListBox();

  bool SetSelection(const set<int>& toselect);

  void AddItem( SG_Widget* w, int at=-1 );
  //Adds the passed widget pointer to the list
  //If at == -1 or at >= number of items in the list, it is added to the end of the list
  //	(NOTE passing a value larger then the number of items will NOT expand the list
  //	 accomodate the passed index, it will expand the list item count by ONE)
  //If at >= 0 && a < number of items it will be inserted at that the passed position
  //	and will push the former occupier and all the follow one position forward
  
  bool RemoveItem( unsigned int item );
  //Removes item number from list
  //Will succeed only if with this item the minimum selection count can still be satisified
  //If selected and at minimum selection the minimum will be satisified starting with the 
  //first item and proceeding down the list until the minimum is reached
  
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
  float alignborder;
  SimpleTexture texdesel, texsel, texclick, texdisable; 
  deque<int> selhistory;
  vector<SG_StickyButton*> stickies;
  vector<SG_Alignment*> aligns;
  map<SG_StickyButton*, int> ptr2pos; 
  };

#endif // SG_LISTBOX_H
