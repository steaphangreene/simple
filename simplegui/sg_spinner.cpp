// *************************************************************************
//  This file is part of the SimpleGUI Example Module by Steaphan Greene
//
//  Copyright 2005-2015 Steaphan Greene <steaphan@gmail.com>
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

const int binvpro = 8;  // FIXME: Changable?

SG_Spinner::SG_Spinner(bool edit) : SG_Compound(binvpro, 2, 0.0, 0.0) {
  upb = new SG_Button();
  upb->SetAlignment(SG_ALIGN_CENTER);
  upb->SetTexturator(stt_upbutt_up, 0);
  upb->SetTexturator(stt_upbutt_dn, 2);
  AddWidget(upb, binvpro - 1, 0, 1, 1);
  downb = new SG_Button();
  downb->SetAlignment(SG_ALIGN_CENTER);
  downb->SetTexturator(stt_dnbutt_up, 0);
  downb->SetTexturator(stt_dnbutt_dn, 2);
  AddWidget(downb, binvpro - 1, 1, 1, 1);

  if (edit)
    text = new SG_Editable("0");
  else
    text = new SG_TextArea("0");
  text->SetAlignment(SG_ALIGN_RIGHT);
  text->SetVisibleSize(SG_KEEPASPECT, 1);
  AddWidget(text, 0, 0, binvpro - 1, 2);

  SetFixedDisplayPrecision(false);
}

SG_Spinner::~SG_Spinner() {}

void SG_Spinner::SetFixedDisplayPrecision(bool f) {
  fixed = f;
  RangerChanged();
}

static SG_Event_DataType event_data;

int SG_Spinner::HandleEvent(SDL_Event *event, float x, float y) {
  if (flags & SG_WIDGET_FLAGS_IGNORE) return -1;   // Ignore all events
  if (flags & SG_WIDGET_FLAGS_DISABLED) return 0;  // Eat all events

  if (event->type == SDL_MOUSEWHEEL && event->wheel.y < 1) {
    Decrement();
    event->type = SDL_SG_EVENT;
    event->user.code = SG_EVENT_NEEDTORENDER;
    event->user.data1 = (void *)(SG_Widget *)this;
    event->user.data2 = NULL;
    return 1;
  } else if (event->type == SDL_MOUSEWHEEL && event->wheel.y > 1) {
    Increment();
    event->type = SDL_SG_EVENT;
    event->user.code = SG_EVENT_NEEDTORENDER;
    event->user.data1 = (void *)(SG_Widget *)this;
    event->user.data2 = NULL;
    return 1;
  }
  return SG_Compound::HandleEvent(event, x, y);
}

bool SG_Spinner::ChildEvent(SDL_Event *event) {
  if (event->user.code == SG_EVENT_BUTTONCLICK) {
    if ((SG_Widget *)(event->user.data1) == (SG_Widget *)(upb)) {
      Increment();
    } else {
      Decrement();
    }
    event->user.code = SG_EVENT_MOVE;
    event->user.data1 = (void *)(SG_Ranger *)this;
    event_data.f[0] = Value();
    event->user.data2 = (void *)&event_data;
    return 1;
  } else if (event->user.code == SG_EVENT_NEWTEXT) {
    float v = strtof(text->Text().c_str(), NULL);
    v -= Min();
    v /= (float)(inc);
    v += 0.5;
    v = (float)((int)(v));
    v *= (float)(inc);
    v += Min();
    SetValue(v);
    event->user.code = SG_EVENT_MOVE;
    event->user.data1 = (void *)(SG_Ranger *)this;
    event_data.f[0] = Value();
    event->user.data2 = (void *)&event_data;
    return 1;
  }
  return 0;  // Silence children doing other things
}

//  bool SG_Spinner::SetDefaultCursor(GL_MODEL *cur);

//  static GL_MODEL SG_Spinner::Default_Mouse_Cursor = NULL;

void SG_Spinner::RangerChanged() {
  char buf[64];
  char format[64];

  snprintf(format, 64, "%%.%df%%c%c",
           getSignificantDigits((fixed ? inc : Value())), 0);
  snprintf(buf, 64, format, Value(), 0);

  text->SetText(buf);
}

int SG_Spinner::getSignificantDigits(float f) {
  float fB = f - ((int)f);
  int p = 0;
  while (fB != 0) {
    fB *= 10;
    fB = fB - ((int)fB);
    p++;
  }
  return p;
}
