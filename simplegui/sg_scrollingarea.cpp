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

#include <GL/gl.h>

#include "sg_scrollingarea.h"
#include "sg_panel.h"
#include "sg_button.h"
#include "sg_events.h"

SG_ScrollingArea::SG_ScrollingArea()
	: SG_Compound(8, 5, 0.1, 0.1) {
  background = new SG_Panel(SG_COL_FG);
  okb = new SG_Button("Ok", SG_COL_RAISED, SG_COL_LOW);
  AddWidget(okb, 7, 4, 1, 1);
  SG_Widget *labelb =
	new SG_TextArea("SG_ScrollingArea", SG_COL_LOW);
  AddWidget(labelb, 1, 2, 6, 1);
  }

SG_ScrollingArea::~SG_ScrollingArea() {	//Even Need this?
  vector<SG_Widget *> tmp = widgets;
  vector<SG_Widget *>::iterator itr = tmp.begin();
  widgets.clear();
  wgeom.clear();
  for(; itr != tmp.end(); ++itr) {
    if(*itr) delete (*itr);
    }
  }

int SG_ScrollingArea::ChildEvent(SDL_Event *event) {
  if(event->user.code == SG_EVENT_BUTTONPRESS) {
    if(event->user.data1 == (void *)(okb)) {
      event->user.code = SG_EVENT_OK;
      event->user.data1 = (void*)this;
      return 1;
      }
    }
  return 0; // Silence children doing other things
  }

//  bool SG_ScrollingArea::SetDefaultCursor(GL_MODEL *cur);
  
//  static GL_MODEL SG_ScrollingArea::Default_Mouse_Cursor = NULL;
