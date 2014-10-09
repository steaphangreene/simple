// *************************************************************************
//  This file is part of the SimpleGUI Example Module by Steaphan Greene
//
//  Copyright 2005-2014 Steaphan Greene <steaphan@gmail.com>
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

#include "SDL_opengl.h"

#include "sg_editable.h"
#include "sg_globals.h"
#include "sg_events.h"

SG_Editable::SG_Editable(string mes, SimpleTexture c, SimpleTexture dc,
                         SimpleTexture fc)
    : SG_TextArea(mes, c, dc) {
  fc.LinkTextFrom(&texture[0]);
  fc.EnableCursor();
  texture.push_back(fc);
}

SG_Editable::~SG_Editable() {}

int SG_Editable::HandleEvent(SDL_Event *event, float x, float y) {
  //  if(event->type == SDL_MOUSEBUTTONDOWN)
  //    fprintf(stderr, "Editable/Handle: Button Down at (%f,%f)\n", x, y);

  if (flags & SG_WIDGET_FLAGS_IGNORE) return -1;   // Ignore all events
  if (flags & SG_WIDGET_FLAGS_DISABLED) return 0;  // Eat all events

  if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == 1) {
    current_sg->SetCurrentWidget(this);
    current_sg->SetFocusWidget(this);
    state = 2;
    return 0;
  } else if (event->type == SDL_MOUSEBUTTONDOWN &&
             (event->button.button == 4 || event->button.button == 5)) {
    // Allow mousewheel events to pass through
    return -1;
  } else if (event->type == SDL_MOUSEBUTTONDOWN) {  // Eat other buttons
    return 0;
  } else if (event->type == SDL_MOUSEBUTTONUP) {
    current_sg->UnsetCurrentWidget();
    return 0;
  } else if (event->type == SDL_TEXTINPUT) {
    if (event->text.text[0] == '\e') {  // Revert?
      current_sg->UnsetFocusWidget();
      event->type = SDL_SG_EVENT;
      event->user.code = SG_EVENT_EDITABORT;
      event->user.data1 = (void *)(SG_Text *)this;
      event->user.data2 = NULL;
      return 1;
    } else if (event->text.text[0] == '\b') {
      if (message.length() > 0) {
        string newmes = message.substr(0, message.length() - 1);
        SetText(newmes);
        event->type = SDL_SG_EVENT;
        event->user.code = SG_EVENT_EDIT;
        event->user.data1 = (void *)(SG_Text *)this;
        event->user.data2 = NULL;
        return 1;
      }
      return 0;
    } else if (event->text.text[0] == '\n' || event->text.text[0] == '\r') {
      if (texture[0].GetTextGeometry()->visible_ylines == 1.0) {
        current_sg->UnsetFocusWidget();
        event->type = SDL_SG_EVENT;
        event->user.code = SG_EVENT_NEWTEXT;
        event->user.data1 = (void *)(SG_Text *)this;
        event->user.data2 = NULL;
        return 1;
      } else {
        string newmes = message.append(1, '\n');
        SetText(newmes);
        event->type = SDL_SG_EVENT;
        event->user.code = SG_EVENT_EDIT;
        event->user.data1 = (void *)(SG_Text *)this;
        event->user.data2 = NULL;
        return 1;
      }
    } else if (event->text.text[0] < 0x80) {
      if (isgraph(event->text.text[0]) || (event->text.text[0] == 0x20)) {
        string newmes = message.append(1, (char)(event->text.text[0]));
        SetText(newmes);
        event->type = SDL_SG_EVENT;
        event->user.code = SG_EVENT_EDIT;
        event->user.data1 = (void *)(SG_Text *)this;
        event->user.data2 = NULL;
        return 1;
      }
    }
    return 0;  // Eat keyboard events I don't handle.
  } else if (event->type == SDL_KEYUP) {
    return 0;  // Eat keyboard events I don't handle.
  }

  return 1;
}

bool SG_Editable::RenderSelf(unsigned long cur_time) {
  if (state == 2 && current_sg->FocusWidget() != this) state = 0;
  return SG_Panel::RenderSelf(cur_time);
}

//  bool SG_Editable::SetDefaultCursor(GL_MODEL *cur);

//  static GL_MODEL SG_Editable::Default_Mouse_Cursor = NULL;
