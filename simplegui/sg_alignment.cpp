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

#include "sg_alignment.h"
#include "sg_events.h"

SG_Alignment::SG_Alignment(float xbor, float ybor) : SG_Widget() {
  SetBorder(xbor, ybor);
  background = NULL;
  last_edge_event = 0;
  }

SG_Alignment::~SG_Alignment() {
  vector<SG_Widget *> tmp = widgets;
  vector<SG_Widget *>::iterator itrw = tmp.begin();
  widgets.clear();
  for(; itrw != tmp.end(); ++itrw) {
    if(*itrw) delete (*itrw);
    }
  if(background) delete background;
  background = NULL;
  }

int SG_Alignment::HandleEdgeEvent(SDL_Event *event, float x, float y) {
  if(last_edge_event > 0 && event->type == SDL_MOUSEMOTION) {
    int etype = SG_EDGE_NONE;
    if(x < -1.0 + (1.0 / 128)) {
      etype -= 1;
      }
    else if(x > 1.0 - (1.0 / 128)) {
      etype += 1;
      }
    if(y < -1.0 + (1.0 / 128)) {
      etype -= 3;
      }
    else if(y > 1.0 - (1.0 / 128)) {
      etype += 3;
      }
    if(etype != last_edge_event) {
      last_edge_event = etype;
      event->type = SDL_SG_EVENT;
      event->user.code = etype;
      event->user.data1 = (void*)(SG_Alignment*)this;
      event->user.data2 = NULL;
      return 1;
      }
    }
  return 0;
  }

int SG_Alignment::HandleEvent(SDL_Event *event, float x, float y) {
//  if(event->type == SDL_MOUSEBUTTONDOWN)
//    fprintf(stderr, "Align/Handle: Button Down at (%f,%f)\n", x, y);

  if(flags & SG_WIDGET_FLAGS_IGNORE) return -1; //Ignore all events
  if(flags & SG_WIDGET_FLAGS_DISABLED) return 0; //Eat all events

  int ret = HandleEdgeEvent(event, x, y);
  if(ret) return ret;

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

  if(background) {
    ret = background->HandleEvent(event, x, y);
    if(ret != -1) return ret;
    }

  if(event->type == SDL_MOUSEBUTTONDOWN
	&& (event->button.button == 4 || event->button.button == 5)) {
		// Allow mousewheel events to pass through
    return -1;
    }

  return ret;
  }

bool SG_Alignment::HandEventTo(SG_Widget *targ, SDL_Event *event,
		float x, float y) {
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
      return widgets[0]->HandEventTo(targ, event, x, y);
      }
    }

  if(background && background->HasWidget(targ)) {
    return background->HandEventTo(targ, event, x, y);
    }

  return 1;
  }

bool SG_Alignment::HasWidget(SG_Widget *targ) {
  if(targ == this) return 1;
  vector<SG_Widget *>::iterator itrw = widgets.begin();
  for(; itrw != widgets.end(); ++itrw) {
    if((*itrw) && (*itrw)->HasWidget(targ)) return 1;
    }
  if(background && background->HasWidget(targ)) return 1;

  return 0;
  }

void SG_Alignment::SetBorder(float xbor, float ybor) {
  xborder = xbor;
  yborder = ybor;
  }

bool SG_Alignment::RenderSelf(unsigned long cur_time) {
//  fprintf(stderr, "Rendering Alignment %p!\n", this);

  glPushMatrix();

  if(background) background->Render(cur_time);	//Same "layer" as parent
  glTranslatef(0.0, 0.0, 0.0625);	//Advance to next "layer"

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

  glPopMatrix();

  return 1;
  }

void SG_Alignment::Disable() {
  flags |= SG_WIDGET_FLAGS_DISABLED;
  if(background) background->Disable();
  vector<SG_Widget *>::iterator itrw = widgets.begin();
  for(; itrw != widgets.end(); ++itrw) {
    if(*itrw) (*itrw)->Disable();
    }
  }

void SG_Alignment::Enable() {
  flags &= (~SG_WIDGET_FLAGS_DISABLED);
  if(background) background->Enable();
  vector<SG_Widget *>::iterator itrw = widgets.begin();
  for(; itrw != widgets.end(); ++itrw) {
    if(*itrw) (*itrw)->Enable();
    }
  }

void SG_Alignment::TurnOn() {
  flags |= SG_WIDGET_FLAGS_ON;
  if(background) background->TurnOn();
  vector<SG_Widget *>::iterator itrw = widgets.begin();
  for(; itrw != widgets.end(); ++itrw) {
    if(*itrw) (*itrw)->TurnOn();
    }
  }

void SG_Alignment::TurnOff() {
  flags &= (~SG_WIDGET_FLAGS_ON);
  if(background) background->TurnOff();
  vector<SG_Widget *>::iterator itrw = widgets.begin();
  for(; itrw != widgets.end(); ++itrw) {
    if(*itrw) (*itrw)->TurnOff();
    }
  }

bool SG_Alignment::AddWidget(SG_Widget *wid) {
  if(widgets.size() > 0) {
    fprintf(stderr, "WARNING: Alignment Widget Already Full!\n");
    return 0;
    }
  widgets.push_back(wid);
  SetAspectRatio(aspect_ratio); //FIXME: Only need to send to new widget!
  return 1;
  }

void SG_Alignment::RemoveWidget(SG_Widget *wid) {
  vector<SG_Widget *>::iterator itrw = widgets.begin();
  for(; itrw != widgets.end(); ++itrw) {
    if((*itrw) == wid) {
      itrw = widgets.erase(itrw);
      --itrw;  // Undo ++itrw from for() loop.
      }
    }
  }

//  bool SG_Alignment::SetDefaultCursor(GL_MODEL *cur);

//  static GL_MODEL SG_Alignment::Default_Mouse_Cursor = NULL;

void SG_Alignment::CalcGeometry(SG_AlignmentGeometry &geom) {
  geom.xp = 0.0;
  geom.yp = 0.0;
  geom.xs = 1.0 - xborder;
  geom.ys = 1.0 - yborder;
  }

void SG_Alignment::SetAspectRatio(float asp) {
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

void SG_Alignment::SetBackground(SG_Widget *wid) {
  background = wid;
  if(background) background->SetAspectRatio(aspect_ratio);
  }

void SG_Alignment::UnsetBackground() {
  background = NULL;
  }

void SG_Alignment::SetAlignment(int align) {
  vector<SG_Widget *>::const_iterator wid = widgets.begin();
  for(; wid != widgets.end(); ++wid) {
    (*wid)->SetAlignment(align);
    }
  }

void SG_Alignment::EnableEdgeEvents() {
  last_edge_event = SG_EDGE_NONE;
  }

void SG_Alignment::DisableEdgeEvents() {
  last_edge_event = 0;
  }
