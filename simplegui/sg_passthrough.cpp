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

#include "SDL_opengl.h"

#include "sg_passthrough.h"
#include "sg_textarea.h" // FIXME: Menu eventaully
#include "sg_events.h"
#include "sg_globals.h"

SG_PassThrough::SG_PassThrough(int lact, int mact, int ract)
	: SG_Alignment(0.0, 0.0) {
  SetBehavior(lact, mact, ract);
  button_menu[0] = NULL;
  button_menu[1] = NULL;
  button_menu[2] = NULL;
  cur_action = SG_PT_IGNORE;
  cur_button = 0;
  send_motion = false;
  }

void SG_PassThrough::SetBehavior(int lact, int mact, int ract) {
  button_action[0] = lact;
  button_action[1] = mact;
  button_action[2] = ract;
  }

SG_PassThrough::~SG_PassThrough() {
  }

bool SG_PassThrough::HandEventTo(SG_Widget *targ, SDL_Event *event,
		float x, float y) {
  bool ret = HandleEdgeEvent(event, x, y);
  if(ret) return ret;

//  if(event->type == SDL_MOUSEBUTTONUP)
//    fprintf(stderr, "Align/Hand: Button Up at (%f,%f)\n", x, y);

  if(targ == this) return HandleEvent(event, x, y);

  if(widgets.size() >= 1 && widgets[0]) {
    if(widgets[0]->HasWidget(targ)) {
      SG_AlignmentGeometry geom;
      CalcGeometry(geom);
      x -= geom.xp; //Scale the coordinates to widget's relative coords
      y -= geom.yp;
      x /= geom.xs;
      y /= geom.ys;
      ret = widgets[0]->HandEventTo(targ, event, x, y);
      }
    }

  if(background && background->HasWidget(targ)) {
    ret = background->HandEventTo(targ, event, x, y);
    }

  //Clean up after menu completion
  if(cur_action == SG_PT_MENU && widgets.size() > 0
	&& current_sg->CurrentWidget() != widgets[0]) {
    if(widgets.size() > 0) RemoveWidget(widgets[0]);
    cur_action = SG_PT_IGNORE;
    cur_button = 0;
    }

  return ret;
  }

int SG_PassThrough::HandleEvent(SDL_Event *event, float x, float y) {
//  if(event->type == SDL_MOUSEBUTTONDOWN)
//    fprintf(stderr, "Align/Handle: Button Down at (%f,%f)\n", x, y);

  if(flags & SG_WIDGET_FLAGS_IGNORE) return -1; //Ignore all events
  if(flags & SG_WIDGET_FLAGS_DISABLED) return 0; //Eat all events

  int ret = 1;

  if(widgets.size() >= 1 && widgets[0]) {
    SG_AlignmentGeometry geom;
    CalcGeometry(geom);
    SG_AlignmentGeometry adj_geom = geom;
    widgets[0]->AdjustGeometry(&adj_geom);
    if(x >= adj_geom.xp-adj_geom.xs && x <= adj_geom.xp+adj_geom.xs
        && y >= adj_geom.yp-adj_geom.ys && y <= adj_geom.yp+adj_geom.ys) {
      float back_x = x, back_y = y;

      x -= geom.xp; //Scale the coordinates to widget's relative coords
      y -= geom.yp;
      x /= geom.xs;
      y /= geom.ys;

      ret = widgets[0]->HandleEvent(event, x, y);
      if(ret != -1) return ret;

      x = back_x; y = back_y;
      }
    }

  static SG_Event_DataType event_data; //Pointer to this is put in event struct

  event_data.f[0] = x;
  event_data.f[1] = y;

  if(event->type == SDL_MOUSEBUTTONDOWN) {
    current_sg->SetCurrentWidget(this);
    if(event->button.button > 3		//We only handle left/mid/right buttons
	|| button_action[event->button.button - 1] == SG_PT_CLICK) {
      event->user.code = SG_EVENT_CLICK + event->button.button;
      event->type = SDL_SG_EVENT;
      event->user.data1 = (void*)(SG_PassThrough*)this;
      event->user.data2 = (void*)&event_data;
      return 1;
      }
    else if(button_action[event->button.button - 1] == SG_PT_IGNORE) {
      return -1;
      }
    else if(button_action[event->button.button - 1] == SG_PT_BLOCK) {
      return 0;
      }
    else if(button_action[event->button.button - 1] == SG_PT_BOX) {
      cur_action = SG_PT_BOX;
      cur_button = event->button.button;
      act_x = x;
      act_y = y;
      cur_x = x;
      cur_y = y;
      return 0;
      }
    else if(button_action[event->button.button - 1] == SG_PT_MENU
	&& button_menu[event->button.button - 1]) {
      cur_action = SG_PT_MENU;
      cur_button = event->button.button;
      act_x = x;
      act_y = y;
      if(button_menu[event->button.button - 1]) {
	if(widgets.size() > 0) RemoveWidget(widgets[0]);
	AddWidget(button_menu[event->button.button - 1]);
	current_sg->SetCurrentWidget(button_menu[event->button.button - 1]);
	}
      return 0;
      }
    else if(button_action[event->button.button - 1] == SG_PT_MENU) {
      fprintf(stderr, "ERROR: No menu defined for button %d!\n",
	event->button.button);
      exit(1);
      }
    else {
      fprintf(stderr, "ERROR: Unknown setting for button handler (%d)\n",
	button_action[event->button.button - 1]);
      exit(1);
      }
    }
  else if(event->type == SDL_MOUSEBUTTONUP) {
    current_sg->UnsetCurrentWidget();
    if(cur_action == SG_PT_BLOCK) {
      return 0;
      }
    else if(cur_action == SG_PT_BOX) {
      event_data.f[2] = act_x;
      event_data.f[3] = act_y;
      event->type = SDL_SG_EVENT;
      event->user.code = SG_EVENT_BOX + cur_button;  //Preserve Original Button
      event->user.data1 = (void*)(SG_PassThrough*)this;
      event->user.data2 = (void*)&event_data;
      cur_action = SG_PT_IGNORE;
      cur_button = 0;
      return 1;
      }
    else if(cur_action == SG_PT_CLICK) {
      return 0; // Eat these events in this case
      }
    }
  else if(event->type == SDL_MOUSEMOTION) {
    if(cur_action == SG_PT_BLOCK) {
      return 0;
      }
    else if(cur_action == SG_PT_BOX) {
      cur_x = x;
      cur_y = y;
      if(cur_x > 1.0) cur_x = 1.0;
      if(cur_y > 1.0) cur_y = 1.0;
      if(cur_x < -1.0) cur_x = -1.0;
      if(cur_y < -1.0) cur_y = -1.0;
      return 0;
      }
    else if(cur_action == SG_PT_MENU) {
      return 0; // Eat these events in this case // FIXME: ?
      }
    else if(cur_action == SG_PT_CLICK) {
      return 0; // Eat these events in this case
      }
    else {
      if(send_motion) {
	event->type = SDL_SG_EVENT;
	event->user.code = SG_EVENT_MOTION;
	event->user.data1 = (void*)(SG_PassThrough*)this;
	event->user.data2 = (void*)&event_data;
	return 1;
	}
      }
    }
  return 1;
  }

bool SG_PassThrough::RenderSelf(unsigned long cur_time) {
//  fprintf(stderr, "Rendering PassThrough %p!\n", this);

  glPushMatrix();

  if(background) background->Render(cur_time);	//Same "layer" as parent
  glTranslatef(0.0, 0.0, 0.0625);		//Advance to next "layer"

  vector<SG_Widget *>::iterator itrw = widgets.begin();
  for(; itrw != widgets.end(); ++itrw) {
    if(*itrw) {
      glPushMatrix();
      SG_AlignmentGeometry geom;
      CalcGeometry(geom);
      widgets[0]->AdjustGeometry(&geom);
      glTranslatef(geom.xp, geom.yp, 0.0);
      glScalef(geom.xs, geom.ys, 1.0);
      (*itrw)->Render(cur_time);
      glPopMatrix();
      }
    }

  if(cur_action == SG_PT_BOX) {
    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_LINE_LOOP);
    glVertex3f(cur_x, cur_y, 0.0);
    glVertex3f(act_x, cur_y, 0.0);
    glVertex3f(act_x, act_y, 0.0);
    glVertex3f(cur_x, act_y, 0.0);
    glEnd();
    }

  glPopMatrix();

  return 1;
  }

//  bool SG_PassThrough::SetDefaultCursor(GL_MODEL *cur);

//  static GL_MODEL SG_PassThrough::Default_Mouse_Cursor = NULL;

void SG_PassThrough::CalcGeometry(SG_AlignmentGeometry &geom) {
  if(cur_action == SG_PT_MENU) {
    geom.xp = act_x;
    geom.yp = act_y;
    geom.xs = 0.125 - xborder;	//Hardcoded for now
    geom.ys = 0.03125 - yborder;
    }
  else {
    geom.xp = 0.0; // Not used by SG_PassThrough widget
    geom.yp = 0.0; // Not used by SG_PassThrough widget
    geom.xs = 1.0 - xborder;
    geom.ys = 1.0 - yborder;
    }
  }

void SG_PassThrough::SetMenu(int but, const vector<string> itms) {
  if(but < 1 || but > 3) {
    fprintf(stderr, "WARNING: Attempted to set menu to invalid button!\n");
    return;
    }
  if(button_menu[but-1] == NULL) {
    button_menu[but-1] = new SG_Menu(itms);
    button_menu[but-1]->SetID(but);
    }
  else {
    button_menu[but-1]->SetItems(itms);
    }
  }

void SG_PassThrough::SetAspectRatio(float asp) {
  aspect_ratio = asp;
  if(background) background->SetAspectRatio(aspect_ratio);
  if(widgets.size() > 0) {
    SG_AlignmentGeometry geom;
    CalcGeometry(geom);
    widgets[0]->AdjustGeometry(&geom);
    float newaspect = aspect_ratio * geom.xs / geom.ys;
    widgets[0]->SetAspectRatio(newaspect);
    }
  }
