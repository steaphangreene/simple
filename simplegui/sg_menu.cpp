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

#include "SDL_opengl.h"

#include "sg_menu.h"
#include "sg_globals.h"
#include "sg_events.h"

SG_Menu::SG_Menu(const vector<string> &itms,
	SG_Texture tex, SG_Texture dis_tex, SG_Texture click_tex)
	: SG_Button("", tex, dis_tex, click_tex) {
  SetItems(itms);
  menu_id = 0;
  }

SG_Menu::~SG_Menu() {
  }

//  bool SG_Menu::SetDefaultCursor(GL_MODEL *cur);
  
//  static GL_MODEL SG_Menu::Default_Mouse_Cursor = NULL;

int SG_Menu::HandleEvent(SDL_Event *event, float x, float y) {
  if(flags & SG_WIDGET_FLAGS_IGNORE) return -1; //Ignore all events
  if(flags & SG_WIDGET_FLAGS_DISABLED) return 0; //Eat all events

  static SG_Event_DataType event_data; //Pointer to this is put in event struct

  if(event->type == SDL_MOUSEBUTTONUP) {
    current_sg->UnsetCurrentWidget();
    y -= 1.0;
    if(y > 0.0 || y < -(double)(items.size()) * 2.0) {
      return 0;	//Missed Menu Options
      }
    else {
      event_data.i[0] = (int)(-y)/2;
      current_sg->UnsetCurrentWidget();
      flags &= (~SG_WIDGET_FLAGS_PRESSED);
      state = 0;
      event->type = SDL_SG_EVENT;
      event->user.code = SG_EVENT_MENU + menu_id;
      event->user.data1 = (void*)this;
      event->user.data2 = (void*)&event_data;
      return 1;
      }
    }  

  return 1;
  }

bool SG_Menu::Render(unsigned long cur_time) {
  if(current_sg->CurrentWidget() != this) return true;

  glPushMatrix();
  while(texture.size() < items.size() * 3) {
    texture.push_back(texture[0]);
    texture.back().dirty = true;
    texture.push_back(texture[1]);
    texture.back().dirty = true;
    texture.push_back(texture[2]);
    texture.back().dirty = true;
    }
  for(unsigned int item = 0; item < items.size(); ++item) {
    state = (state % 3) + item * 3;
    if(texture[state].dirty) message = items[item];
    if(!SG_Button::Render(cur_time)) {
      glPopMatrix();
      return false;
      }
    if(item != items.size() - 1) glTranslatef(0.0, -2.0, 0.0);
    }
  state = (state % 3);
  glPopMatrix();
  return true;
  }

void SG_Menu::SetItems(const vector<string> &itms) {
  items = itms;
  }

void SG_Menu::SetID(int id) {
  menu_id = id;
  if(menu_id < 0) menu_id = 0;
  if(menu_id >= SG_EVENT_MENUMAX - SG_EVENT_MENU)
	menu_id = SG_EVENT_MENUMAX - SG_EVENT_MENU - 1;
  }

