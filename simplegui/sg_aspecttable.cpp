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

#include "sg_events.h"
#include "sg_aspecttable.h"

SG_AspectTable::SG_AspectTable(float asp, int xsz, int ysz,
		float xbor, float ybor)
	: SG_Table(xsz, ysz, xbor, ybor) {
  fixed_aspect = asp;
  }

SG_AspectTable::~SG_AspectTable() {
  }

int SG_AspectTable::HandleEvent(SDL_Event *event, float x, float y) {
//  if(event->type == SDL_MOUSEBUTTONDOWN)
//    fprintf(stderr, "AspectTable/Handle: Button Down at (%f,%f)\n", x, y);

  if(flags & SG_WIDGET_FLAGS_IGNORE) return -1; //Ignore all events
  if(flags & SG_WIDGET_FLAGS_DISABLED) return 0; //Eat all events

  if(xsize <= 0 || ysize <= 0) return -1;

  int ret = HandleEdgeEvent(event, x, y);
  if(ret) return ret;

  vector<int>::iterator itrgr = wgrav.begin();
  vector<SG_Widget *>::iterator itrw = widgets.begin();
  vector<SG_TableGeometry>::iterator itrg = wgeom.begin();
  for(; itrw != widgets.end(); ++itrw, ++itrg, ++itrgr) {
    SG_AlignmentGeometry geom = CalcGeometry(itrg, itrgr);
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

bool SG_AspectTable::HandEventTo(SG_Widget *targ, SDL_Event *event,
		float x, float y) {
//  if(event->type == SDL_MOUSEBUTTONUP)
//    fprintf(stderr, "AspectTable/Hand: Button Up at (%f,%f)\n", x, y);

  if(targ == this) return HandleEvent(event, x, y);

  if(xsize <= 0 || ysize <= 0) return false;

  vector<int>::iterator itrgr = wgrav.begin();
  vector<SG_Widget *>::iterator itrw = widgets.begin();
  vector<SG_TableGeometry>::iterator itrg = wgeom.begin();
  for(; itrw != widgets.end(); ++itrw, ++itrg, ++itrgr) {
    if((*itrw)->HasWidget(targ)) {
      SG_AlignmentGeometry geom = CalcGeometry(itrg, itrgr);
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

//  bool SG_AspectTable::SetDefaultCursor(GL_MODEL *cur);
  
//  static GL_MODEL SG_AspectTable::Default_Mouse_Cursor = NULL;

bool SG_AspectTable::AddWidget(SG_Widget *wid) {
  return AddWidget(wid, SG_CENTER);
  }

bool SG_AspectTable::AddWidget(SG_Widget *wid, int grav) {
  if(!SG_Table::AddWidget(wid)) {
    return false;
    }
  wgrav.push_back(grav);
  return true;
  }

bool SG_AspectTable::AddWidget(SG_Widget *wid, int x1, int y1) {
  return AddWidget(wid, x1, y1, 1, 1, SG_CENTER);
  }

bool SG_AspectTable::AddWidget(SG_Widget *wid, int x1, int y1, int grav) {
  return AddWidget(wid, x1, y1, 1, 1, grav);
  }

bool SG_AspectTable::AddWidget(SG_Widget *wid, int x1, int y1,
	int xs, int ys) {
  return AddWidget(wid, x1, y1, xs, ys, SG_CENTER);
  }

bool SG_AspectTable::AddWidget(SG_Widget *wid, int x1, int y1,
	int xs, int ys, int grav) {
  if(!SG_Table::AddWidget(wid, x1, y1, xs, ys)) {
    return false;
    }
  wgrav.push_back(grav);
  return true;
  }

bool SG_AspectTable::RenderSelf(unsigned long cur_time) {
//  fprintf(stderr, "Rendering AspectTable %p!\n", this);

  if(xsize <= 0 || ysize <= 0) return true;

  glPushMatrix();

  if(background) background->Render(cur_time);  //Same "layer" as parent
  glTranslatef(0.0, 0.0, 0.0625);               //Advance to next "layer"

  vector<int>::iterator itrgr = wgrav.begin();
  vector<SG_Widget *>::iterator itrw = widgets.begin();
  vector<SG_TableGeometry>::iterator itrg = wgeom.begin();
  for(; itrw != widgets.end(); ++itrw, ++itrg, ++itrgr) {
    if(*itrw) {
      glPushMatrix();
      if(aspect_ratio > fixed_aspect) {
	glScalef(fixed_aspect/aspect_ratio, 1.0, 1.0);
	if((*itrgr) & SG_LEFT) {
	  glTranslatef(1.0 - (aspect_ratio/fixed_aspect), 0.0, 0.0);
	  }
	else if((*itrgr) & SG_RIGHT) {
	  glTranslatef((aspect_ratio/fixed_aspect) - 1.0, 0.0, 0.0);
	  }
	}
      else if(aspect_ratio < fixed_aspect) {
	glScalef(1.0, aspect_ratio/fixed_aspect, 1.0);
	if((*itrgr) & SG_UP) {
	  glTranslatef(0.0, (fixed_aspect/aspect_ratio) - 1.0, 0.0);
	  }
	else if((*itrgr) & SG_DOWN) {
	  glTranslatef(0.0, 1.0 - (fixed_aspect/aspect_ratio), 0.0);
	  }
	}
      SG_AlignmentGeometry geom = CalcGeometry(itrg, itrgr);
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

void SG_AspectTable::SetAspectRatio(float asp) {
  aspect_ratio = asp;
  if(background) background->SetAspectRatio(aspect_ratio);	//BG Unchanged

  vector<int>::iterator itrgr = wgrav.begin();
  vector<SG_Widget *>::iterator itrw = widgets.begin();
  vector<SG_TableGeometry>::iterator itrg = wgeom.begin();
  for(; itrw != widgets.end(); ++itrw, ++itrg, ++itrgr) {
    SG_AlignmentGeometry geom = CalcGeometry(itrg, itrgr);
    (*itrw)->AdjustGeometry(&geom);
    (*itrw)->SetAspectRatio(fixed_aspect * geom.xs / geom.ys);
    }
  }

const SG_AlignmentGeometry SG_AspectTable::CalcGeometry(
	const vector<SG_TableGeometry>::iterator &wgeom,
	const vector<int>::iterator &grav
	) {
  SG_AlignmentGeometry geom;
  float xcs, ycs; //Relative Cell Sizes.
  float xcp, ycp; //Center Cell Poisiton.

  xcs = 2.0 / xsize;
  ycs = 2.0 / ysize;

  xcp = float((*wgeom).xpos) + float((*wgeom).xsize) / 2.0;
  ycp = float((*wgeom).ypos) + float((*wgeom).ysize) / 2.0;

  geom.xp = -1.0 + xcs * xcp;
  geom.yp = 1.0 - ycs * ycp;
  geom.xs = xcs * float((*wgeom).xsize) / 2.0 - xborder/float(xsize);
  geom.ys = ycs * float((*wgeom).ysize) / 2.0 - yborder/float(ysize);

//  fprintf(stderr, "Calced: (%f,%f) %fx%f\n",
//	geom.xp, geom.yp, geom.xs, geom.ys);
  return geom;
  }
