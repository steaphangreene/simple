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

#include <GL/gl.h>

#include "sg_filebrowser.h"
#include "sg_panel.h"
#include "sg_button.h"
#include "sg_events.h"

SG_FileBrowser::SG_FileBrowser(const string &filt)
	: SG_Compound(8, 5, 0.1, 0.1) {
  filter = filt;
  background = new SG_Panel(SG_COL_FG);
  openb = new SG_Button("Open", SG_COL_RAISED, SG_COL_LOW);
  AddWidget(openb, 6, 4, 2, 1);
  nameb = new SG_TextArea("file.txt", SG_COL_LOW);
  AddWidget(nameb, 1, 2, 6, 1);
  }

SG_FileBrowser::~SG_FileBrowser() {	//Even Need this?
  vector<SG_Widget *> tmp = widgets;
  vector<SG_Widget *>::iterator itr = tmp.begin();
  widgets.clear();
  wgeom.clear();
  for(; itr != tmp.end(); ++itr) {
    if(*itr) delete (*itr);
    }
  }

int SG_FileBrowser::ChildEvent(SDL_Event *event) {
  if(event->user.code == SG_EVENT_BUTTONPRESS) {
    if(event->user.data1 == (void *)(openb)) {
      event->user.code = SG_EVENT_FILEOPEN;
      event->user.data1 = (void*)this;
      event->user.data2 = (void*)"file.txt";
      return 1;
      }
    }
  return 0; // Silence children doing other things
  }

//  bool SG_FileBrowser::SetDefaultCursor(GL_MODEL *cur);
  
//  static GL_MODEL SG_FileBrowser::Default_Mouse_Cursor = NULL;
