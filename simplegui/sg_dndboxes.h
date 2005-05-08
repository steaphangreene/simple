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

#ifndef SG_DNDBOXES_H
#define SG_DNDBOXES_H

#include "sg_compound.h"
#include "sg_dragable.h"

#include <map>
#include <vector>
using namespace std;

class SG_DNDBoxes : public SG_Compound {
public:
  SG_DNDBoxes(int xsz, int ysz);
  virtual ~SG_DNDBoxes();
//  virtual bool SetDefaultCursor(GL_MODEL *cur);
  virtual bool Render(unsigned long cur_time);
  virtual bool ChildEvent(SDL_Event *event);
  void Exclude(int xp, int yp, int xs = 1, int ys = 1);
  void Include(int xp, int yp, int xs = 1, int ys = 1, int xcs = 1, int ycs = 1,
	Uint32 id = 0, Uint32 inv = 0);
  bool AddItem(SDL_Surface *icon, int x1, int y1, int xs=1, int ys=1,
	Uint32 id = 0, Uint32 tps = 0);
  void RemoveItem(int x1, int y1);

  //Handle all these to disable them!
  virtual bool AddWidget(SG_Widget *wid, int x1, int y1, int xs=1, int ys=1);
  virtual bool AddWidget(SG_Widget *wid);
  virtual void RemoveWidget(SG_Widget *wid);
  
protected:
  bool CanFit(int x1, int y1, int xs=1, int ys=1, Uint32 tps = 0);
  void UnconfigDrag(SG_Dragable *drag, int x1, int y1, int xs=1, int ys=1);
  void ConfigDrag(SG_Dragable *drag, int x1, int y1, int xs=1, int ys=1);

//  static GL_MODEL Default_Mouse_Cursor;
  vector<bool> present;		// Is this potential cell present?
  vector<bool> occupied;	// Is this cell full?
  vector<bool> basecell;	// Is this the cell a base cell?
  vector<Uint32> invalids;	// Bitvector of invalid types
  vector<Uint32> cellids;	// (Non-Unique) ID of cell area

  // Maps items to [item id, item types bitvec]
  struct ItemInfo { Uint32 id; Uint32 types; };
  map<SG_Widget *, ItemInfo> itemmap;
  };

#endif // SG_DNDBOXES_H
