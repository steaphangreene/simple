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

#include "sg_scrollable.h"

SG_Scrollable::SG_Scrollable(float xfac, float yfac)
	: SG_Alignment(0.0, 0.0) {
  xfactor = xfac;
  yfactor = yfac;
  }

SG_Scrollable::~SG_Scrollable() {
  vector<SG_Widget *> tmp = widgets;	//Not necessary?
  vector<SG_Widget *>::iterator itrw = tmp.begin();
  widgets.clear();
  for(; itrw != tmp.end(); ++itrw) {
    if(*itrw) delete (*itrw);
    }
  }

int SG_Scrollable::HandleEvent(SDL_Event *event, float x, float y) {
//  if(event->type == SDL_MOUSEBUTTONDOWN)
//    fprintf(stderr, "Align/Handle: Button Down at (%f,%f)\n", x, y);

  if(flags & SG_WIDGET_FLAGS_IGNORE) return -1; //Ignore all events
  if(flags & SG_WIDGET_FLAGS_DISABLED) return 0; //Eat all events

  int ret = 1;

  if(widgets.size() >= 1 && widgets[0]) {
    CalcGeometry();
    if(x >= -cur_geom.xs && x <= cur_geom.xs
		&& y >= -cur_geom.ys && y <= cur_geom.ys) {
      float back_x = x, back_y = y;
      
      x /= cur_geom.xs; //Scale the coordinates to widget's relative coords
      y /= cur_geom.ys;
      return widgets[0]->HandleEvent(event, x, y);

      x = back_x; y = back_y;
      }
    }

  if(background) {
    ret = background->HandleEvent(event, x, y);
    if(ret != -1) return ret;
    }

  return ret;
  }

bool SG_Scrollable::HandEventTo(SG_Widget *targ, SDL_Event *event,
		float x, float y) {
//  if(event->type == SDL_MOUSEBUTTONUP)
//    fprintf(stderr, "Align/Hand: Button Up at (%f,%f)\n", x, y);

  if(targ == this) return HandleEvent(event, x, y);

  if(widgets.size() >= 1 && widgets[0]) {
    CalcGeometry();
    if(widgets[0]->HasWidget(targ)) {
      x /= cur_geom.xs; //Scale the coordinates to widget's relative coords
      y /= cur_geom.ys;
      return widgets[0]->HandEventTo(targ, event, x, y);
      }
    }

  if(background && background->HasWidget(targ)) {
    return background->HandEventTo(targ, event, x, y);
    }

  return 1;
  }

bool SG_Scrollable::Render(unsigned long cur_time) {
//  fprintf(stderr, "Rendering Scrollable %p!\n", this);

  if(flags & SG_WIDGET_FLAGS_HIDDEN) return 1;

  glPushMatrix();

  if(background) background->Render(cur_time);	//Same "layer" as parent
  glTranslatef(0.0, 0.0, 0.0625);		//Advance to next "layer"

  vector<SG_Widget *>::iterator itrw = widgets.begin();
  for(; itrw != widgets.end(); ++itrw) {
    if(*itrw) {
      glPushMatrix();
      CalcGeometry();
      glScalef(cur_geom.xs, cur_geom.ys, 1.0);
      (*itrw)->Render(cur_time);
      glPopMatrix();
      }
    }

  glPopMatrix();

  return 1;
  }

//  bool SG_Scrollable::SetDefaultCursor(GL_MODEL *cur);
  
//  static GL_MODEL SG_Scrollable::Default_Mouse_Cursor = NULL;

void SG_Scrollable::CalcGeometry() {
  // cur_geom.xp = 0.0; // Not used by SG_Scrollable widget
  // cur_geom.yp = 0.0; // Not used by SG_Scrollable widget
  cur_geom.xs = 1.0 - xborder;
  cur_geom.ys = 1.0 - yborder;
  }
