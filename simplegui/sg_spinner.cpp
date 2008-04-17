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

#include "SDL_opengl.h"

#include "sg_events.h"
#include "sg_globals.h"
#include "sg_menu.h"
#include "sg_spinner.h"
#include "sg_editable.h"
#include "sg_stickybutton.h"

const int binvpro = 8; //FIXME: Changable?

SG_Spinner::SG_Spinner(bool edit) : SG_Compound(binvpro, 2, 0.0, 0.0) {

  upb = new SG_Button("^");
  upb->SetAlignment(SG_ALIGN_CENTER);
  AddWidget(upb, binvpro-1, 0, 1, 1);
  downb = new SG_Button("v");
  downb->SetAlignment(SG_ALIGN_CENTER);
  AddWidget(downb, binvpro-1, 1, 1, 1);

  if(edit) text = new SG_Editable("0");
  else text = new SG_TextArea("0");
  AddWidget(text, 0, 0, binvpro-1, 2);
  }

SG_Spinner::~SG_Spinner() {
  }

bool SG_Spinner::ChildEvent(SDL_Event *event) {
  if(event->user.code == SG_EVENT_BUTTONCLICK) {
    //FIXME: Up/Down
    event->user.code = SG_EVENT_NEEDTORENDER;
    event->user.data1 = NULL;
    event->user.data2 = NULL;
    return 1;
    }
  return 0; // Silence children doing other things
  }

//  bool SG_Spinner::SetDefaultCursor(GL_MODEL *cur);
  
//  static GL_MODEL SG_Spinner::Default_Mouse_Cursor = NULL;

const string &SG_Spinner::Text() {
  return text->Text();
  }

void SG_Spinner::SetText(const string &itms) {
  fprintf(stderr, "WARNING: Tried to set text of SG_Spinner - use SetValue\n");
  }
