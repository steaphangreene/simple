// *************************************************************************
//  This file is part of the SimpleGUI Example Module by Steaphan Greene
//
//  Copyright 2005-2006 Steaphan Greene <stea@cs.binghamton.edu>
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

#include "sg_positional.h"

SG_Positional::SG_Positional()
	: SG_Alignment(0.0, 0.0),
	  ranger(0.0, 0.0, 0.0, 0.0, -1.0, -1.0, 1.0, 1.0) {
  }

SG_Positional::~SG_Positional() {
  vector<SG_Widget *> tmp = widgets;
  vector<SG_Widget *>::iterator itr = tmp.begin();
  widgets.clear();
  wgeom.clear();
  for(; itr != tmp.end(); ++itr) {
    if(*itr) delete (*itr);
    }
  }

int SG_Positional::HandleEvent(SDL_Event *event, float x, float y) {
  if(flags & SG_WIDGET_FLAGS_IGNORE) return -1; //Ignore all events
  if(flags & SG_WIDGET_FLAGS_DISABLED) return 0; //Eat all events

  int ret = 1;

  vector<SG_Widget *>::iterator itrw = widgets.begin();
  vector<SG_AlignmentGeometry>::iterator itrg = wgeom.begin();
  for(; itrw != widgets.end(); ++itrw, ++itrg) {
    CalcGeometry(itrg);
    SG_AlignmentGeometry adj_geom = cur_geom;
    (*itrw)->AdjustGeometry(&adj_geom);
    if(x >= adj_geom.xp-adj_geom.xs && x <= adj_geom.xp+adj_geom.xs
	&& y >= adj_geom.yp-adj_geom.ys && y <= adj_geom.yp+adj_geom.ys) {
      float back_x = x, back_y = y;

      x -= cur_geom.xp;	//Scale the coordinates to widget's relative coords
      y -= cur_geom.yp;
      x /= cur_geom.xs;
      y /= cur_geom.ys;
      ret = (*itrw)->HandleEvent(event, x, y);
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

bool SG_Positional::HandEventTo(SG_Widget *targ, SDL_Event *event,
		float x, float y) {
  if(targ == this) return HandleEvent(event, x, y);

  vector<SG_Widget *>::iterator itrw = widgets.begin();
  vector<SG_AlignmentGeometry>::iterator itrg = wgeom.begin();
  for(; itrw != widgets.end(); ++itrw, ++itrg) {
    if((*itrw)->HasWidget(targ)) {
      CalcGeometry(itrg);
      x -= cur_geom.xp;	//Scale the coordinates to widget's relative coords
      y -= cur_geom.yp;
      x /= cur_geom.xs;
      y /= cur_geom.ys;
      return (*itrw)->HandEventTo(targ, event, x, y);
      }
    }

  if(background->HasWidget(targ))
    return background->HandEventTo(targ, event, x, y);

  return true;
  }

bool SG_Positional::RenderSelf(unsigned long cur_time) {
  if(flags & SG_WIDGET_FLAGS_HIDDEN) return true;

  glPushMatrix();

  if(background) background->Render(cur_time);	//Same "layer" as parent
  glTranslatef(0.0, 0.0, 0.0625);		//Advance to next "layer"

  vector<SG_Widget *>::iterator itrw = widgets.begin();
  vector<SG_AlignmentGeometry>::iterator itrg = wgeom.begin();
  for(; itrw != widgets.end(); ++itrw, ++itrg) {
    if(*itrw) {
      glPushMatrix();
      CalcGeometry(itrg);
      (*itrw)->AdjustGeometry(&cur_geom);
      glTranslatef(cur_geom.xp, cur_geom.yp, 0.0);
      glScalef(cur_geom.xs, cur_geom.ys, 1.0);
      (*itrw)->Render(cur_time);
      glPopMatrix();
      }
    }

  glPopMatrix();

//  fprintf(stderr, "  Done.\n\n");

  return true;
  }

bool SG_Positional::AddWidget(SG_Widget *wid,
	const float xoff, const float yoff,
	const float xsize, const float ysize, bool modal) {
  SG_AlignmentGeometry geom = { xoff, yoff, xsize, ysize };
  widgets.push_back(wid);
  wgeom.push_back(geom);
  SetAspectRatio(aspect_ratio); //FIXME: Only need to send to new widget!
  return true;
  }

void SG_Positional::RemoveWidget(SG_Widget *wid) {
  vector<SG_Widget *>::iterator itrw = widgets.begin();
  vector<SG_AlignmentGeometry>::iterator itrg = wgeom.begin();
  for(; itrw != widgets.end(); ++itrw, ++itrg) {
    if((*itrw) == wid) {
      itrw = widgets.erase(itrw);
      itrg = wgeom.erase(itrg);
      --itrw;  // Undo ++itrw from for() loop.
      --itrg;  // Undo ++itrg from for() loop.
      }
    }
  }

//  bool SG_Positional::SetDefaultCursor(GL_MODEL *cur);
  
//  static GL_MODEL SG_Positional::Default_Mouse_Cursor = NULL;

void SG_Positional::CalcGeometry(
	const vector<SG_AlignmentGeometry>::iterator &geom
	) {
  cur_geom = *geom;
  }

void SG_Positional::SetAspectRatio(double asp) {
  aspect_ratio = asp;
  if(background) background->SetAspectRatio(aspect_ratio);

  vector<SG_Widget *>::iterator itrw = widgets.begin();
  vector<SG_AlignmentGeometry>::iterator itrg = wgeom.begin();
  for(; itrw != widgets.end(); ++itrw, ++itrg) {
    CalcGeometry(itrg);
    (*itrw)->AdjustGeometry(&cur_geom);
    (*itrw)->SetAspectRatio(aspect_ratio * cur_geom.xs / cur_geom.ys);
    }
  }
