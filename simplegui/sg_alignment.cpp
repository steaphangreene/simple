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

#include "sg_alignment.h"

SG_Alignment::SG_Alignment(float xbor, float ybor) : SG_Widget() {
  SetBorder(xbor, ybor);
  background = NULL;
  }

SG_Alignment::~SG_Alignment() {
  vector<SG_Widget *> tmp = widgets;
  vector<SG_Widget *>::iterator itrw = tmp.begin();
  widgets.clear();
  for(; itrw != tmp.end(); ++itrw) {
    if(*itrw) delete (*itrw);
    }
  }

bool SG_Alignment::HandleMouseEvent(SDL_Event *event, float x, float y) {
//  if(event->type == SDL_MOUSEBUTTONDOWN)
//    fprintf(stderr, "Align/Handle: Button Down at (%f,%f)\n", x, y);

  if(widgets.size() >= 1 && widgets[0]) {
    CalcGeometry();
    if(x >= -cur_geom.xs && x <= cur_geom.xs
		&& y >= -cur_geom.ys && y <= cur_geom.ys) {
      x /= cur_geom.xs; //Scale the coordinates to widget's relative coords
      y /= cur_geom.ys;
      return widgets[0]->HandleMouseEvent(event, x, y);
      }
    }

  if(background) return background->HandleMouseEvent(event, x, y);

  return 1;
  }

bool SG_Alignment::HandMouseEventTo(SG_Widget *targ, SDL_Event *event,
		float x, float y) {
//  if(event->type == SDL_MOUSEBUTTONUP)
//    fprintf(stderr, "Align/Hand: Button Up at (%f,%f)\n", x, y);

  if(targ == this) return HandleMouseEvent(event, x, y);

  if(widgets.size() >= 1 && widgets[0]) {
    CalcGeometry();
    if(widgets[0]->HasWidget(targ)) {
      x /= cur_geom.xs; //Scale the coordinates to widget's relative coords
      y /= cur_geom.ys;
      return widgets[0]->HandMouseEventTo(targ, event, x, y);
      }
    }

  if(background && background->HasWidget(targ)) {
    return background->HandMouseEventTo(targ, event, x, y);
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

bool SG_Alignment::Render(unsigned long cur_time) {
//  fprintf(stderr, "Rendering Alignment %p!\n", this);

  if(flags & SG_WIDGET_FLAGS_HIDDEN) return 1;

  glPushMatrix();

  if(background) background->Render(cur_time);	//Same "layer" as parent
  glTranslatef(0.0, 0.0, 0.0625);	//Advance to next "layer"

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

bool SG_Alignment::AddWidget(SG_Widget *wid) {
  if(widgets.size() > 0) {
    fprintf(stderr, "Warning: Alignment Widget Already Full!\n");
    return 0;
    }
  widgets.push_back(wid);
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

void SG_Alignment::CalcGeometry() {
  // cur_geom.xp = 0.0; // Not used by SG_Alignment widget
  // cur_geom.yp = 0.0; // Not used by SG_Alignment widget
  cur_geom.xs = 1.0 - xborder;
  cur_geom.ys = 1.0 - yborder;
  }
