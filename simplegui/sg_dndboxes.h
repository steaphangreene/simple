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

#include <vector>
using namespace std;

class SG_TransLabel;

class SG_DNDBoxes : public SG_Compound {
public:
  SG_DNDBoxes(int xsz, int ysz);
  virtual ~SG_DNDBoxes();
//  virtual bool SetDefaultCursor(GL_MODEL *cur);
  virtual bool Render(unsigned long cur_time);
  virtual bool ChildEvent(SDL_Event *event);
  void Exclude(int xp, int yp, int xsz = 1, int ysz = 1);
  void Include(int xp, int yp, int xsz = 1, int ysz = 1);
  bool AddItem(SG_TransLabel *wid, int x1, int y1, int xs=1, int ys=1);

  //Handle all these to disable them!
  virtual bool AddWidget(SG_Widget *wid, int x1, int y1, int xs=1, int ys=1);
  virtual bool AddWidget(SG_Widget *wid);
  virtual void RemoveWidget(SG_Widget *wid);
  
protected:
//  static GL_MODEL Default_Mouse_Cursor;
  vector<bool> present;
  };

#endif // SG_DNDBOXES_H
