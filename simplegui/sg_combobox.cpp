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

#include "SDL_opengl.h"

#include "sg_events.h"
#include "sg_globals.h"
#include "sg_menu.h"
#include "sg_combobox.h"
#include "sg_editable.h"

const int binvpro = 8; //FIXME: Changable?

SG_ComboBox::SG_ComboBox(const vector<string> &options, bool edit,
        SG_Texture btex, SG_Texture btex_dis, SG_Texture btex_click,
        SG_Texture ttex, SG_Texture ttex_dis, SG_Texture ttex_fg,
	SG_Texture mtex, SG_Texture mtex_dis, SG_Texture mtex_sel)
	: SG_Compound(binvpro, 1, 0.0, 0.0) {

  opb = new SG_Button("v", btex, btex_dis, btex_click);
  AddWidget(opb, binvpro-1, 0, 1, 1);

  string message;
  if(options.size() > 0) message = options[0];
  if(edit) text = new SG_Editable(message, ttex, ttex_dis, ttex_fg);
  else text = new SG_TextArea(message, ttex, ttex_dis);
  AddWidget(text, 0, 0, binvpro-1, 1);

  menu = new SG_Menu(options, mtex, mtex_dis, mtex_sel);
  AddWidget(menu, 0, 0, binvpro, 1);
  wgeom[2].ypos = 1;
  wgeom[2].ysize = 1;
  }

SG_ComboBox::~SG_ComboBox() {
  }

bool SG_ComboBox::ChildEvent(SDL_Event *event) {
  if(event->user.code == SG_EVENT_BUTTONPRESS) {
    current_sg->SetCurrentWidget(menu);
    event->user.code = SG_EVENT_NEEDTORENDER;
    event->user.data1 = NULL;
    event->user.data2 = NULL;
    return 1;
    }
  if(current_sg->CurrentWidget() != (SG_Widget*)menu) {
    if(opb->State() == 2) opb->SetState(0);
    }
  if(event->user.data1 == (void*)(SG_MultiText*)menu) {
    if(event->user.code == SG_EVENT_MENU) {
      text->SetText(menu->Item(((int*)(event->user.data2))[0]));
      event->user.code = SG_EVENT_NEWTEXT;
      event->user.data1 = (void*)(SG_Text*)this;
      event->user.data2 = NULL;
      return 1;   // This event is now mine
      }
    event->user.code = SG_EVENT_NEEDTORENDER;
    event->user.data1 = NULL;
    event->user.data2 = NULL;
    return 1;   // Eat other menu events but request re-render
    }
  return 0; // Silence children doing other things
  }

//  bool SG_ComboBox::SetDefaultCursor(GL_MODEL *cur);
  
//  static GL_MODEL SG_ComboBox::Default_Mouse_Cursor = NULL;

const string &SG_ComboBox::Text() {
  return text->Text();
  }

void SG_ComboBox::SetText(const string &itms) {
  text->SetText(itms);
  }

const string &SG_ComboBox::Item(int opt) {
  return menu->Item(opt);
  }

void SG_ComboBox::SetItems(const vector<string> &itms) {
  menu->SetItems(itms);
  if(itms.size() > 0) text->SetText(itms[0]);
  }
