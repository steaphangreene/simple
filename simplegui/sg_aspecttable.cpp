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

  int ret = SG_Table::HandleEvent(event, x, y);

  return ret;
  }

bool SG_AspectTable::HandEventTo(SG_Widget *targ, SDL_Event *event,
		float x, float y) {
//  if(event->type == SDL_MOUSEBUTTONUP)
//    fprintf(stderr, "AspectTable/Hand: Button Up at (%f,%f)\n", x, y);

  if(targ == this) return HandleEvent(event, x, y);

  int ret = SG_Table::HandEventTo(targ, event, x, y);

  return ret;
  }

//  bool SG_AspectTable::SetDefaultCursor(GL_MODEL *cur);
  
//  static GL_MODEL SG_AspectTable::Default_Mouse_Cursor = NULL;

bool SG_AspectTable::AddWidget(SG_Widget *wid, int grav) {
  if(!SG_Table::AddWidget(wid)) {
    return false;
    }
  wgrav.push_back(grav);
  return true;
  }

bool SG_AspectTable::AddWidget(SG_Widget *wid, int x1, int y1, int grav) {
  return AddWidget(wid, x1, y1, 1, 1, grav);
  }

bool SG_AspectTable::AddWidget(SG_Widget *wid, int x1, int y1,
	int xs, int ys, int grav) {
  if(!SG_Table::AddWidget(wid, x1, y1, xs, ys)) {
    return false;
    }
  wgrav.push_back(grav);

  //= geom;
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

void SG_AspectTable::SetAspectRatio(float asp) {
  aspect_ratio = asp;
  if(background) background->SetAspectRatio(aspect_ratio);	//BG Unchanged

  vector<SG_Widget *>::iterator itrw = widgets.begin();
  vector<SG_TableGeometry>::iterator itrg = wgeom.begin();
  for(; itrw != widgets.end(); ++itrw, ++itrg) {
    CalcGeometry(itrg);
    (*itrw)->AdjustGeometry(&cur_geom);
    (*itrw)->SetAspectRatio(fixed_aspect * cur_geom.xs / cur_geom.ys);
    }
  }
