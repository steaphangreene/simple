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

  int ret = HandleEdgeEvent(event, x, y);
  if(ret) return ret;

  vector<SG_Widget *>::iterator itrw = widgets.begin();
  vector<SG_AlignmentGeometry>::iterator itrg = wgeom.begin();
  for(; itrw != widgets.end(); ++itrw, ++itrg) {
    SG_AlignmentGeometry geom;
    CalcGeometry(geom, itrg);
    SG_AlignmentGeometry adj_geom = geom;
    (*itrw)->AdjustGeometry(&adj_geom);
    if(x >= adj_geom.xp-adj_geom.xs && x <= adj_geom.xp+adj_geom.xs
	&& y >= adj_geom.yp-adj_geom.ys && y <= adj_geom.yp+adj_geom.ys) {
      float back_x = x, back_y = y;

      x -= geom.xp;	//Scale the coordinates to widget's relative coords
      y -= geom.yp;
      x /= geom.xs;
      y /= geom.ys;
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
      SG_AlignmentGeometry geom;
      CalcGeometry(geom, itrg);
      x -= geom.xp;	//Scale the coordinates to widget's relative coords
      y -= geom.yp;
      x /= geom.xs;
      y /= geom.ys;
      return (*itrw)->HandEventTo(targ, event, x, y);
      }
    }

  if(background->HasWidget(targ))
    return background->HandEventTo(targ, event, x, y);

  return true;
  }

bool SG_Positional::RenderSelf(unsigned long cur_time) {
  glPushMatrix();

  if(background) background->Render(cur_time);	//Same "layer" as parent
  glTranslatef(0.0, 0.0, 0.0625);		//Advance to next "layer"

  vector<SG_Widget *>::iterator itrw = widgets.begin();
  vector<SG_AlignmentGeometry>::iterator itrg = wgeom.begin();
  for(; itrw != widgets.end(); ++itrw, ++itrg) {
    if(*itrw) {
      glPushMatrix();
      SG_AlignmentGeometry geom;
      CalcGeometry(geom, itrg);
      (*itrw)->AdjustGeometry(&geom);
      glTranslatef(geom.xp, geom.yp, 0.0);
      glScalef(geom.xs, geom.ys, 1.0);
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

void SG_Positional::CalcGeometry(SG_AlignmentGeometry &geom,
	const vector<SG_AlignmentGeometry>::iterator &wgeom
	) {
  geom = *wgeom;
  }

void SG_Positional::SetAspectRatio(float asp) {
  aspect_ratio = asp;
  if(background) background->SetAspectRatio(aspect_ratio);

  vector<SG_Widget *>::iterator itrw = widgets.begin();
  vector<SG_AlignmentGeometry>::iterator itrg = wgeom.begin();
  for(; itrw != widgets.end(); ++itrw, ++itrg) {
    SG_AlignmentGeometry geom;
    CalcGeometry(geom, itrg);
    (*itrw)->AdjustGeometry(&geom);
    (*itrw)->SetAspectRatio(aspect_ratio * geom.xs / geom.ys);
    }
  }
