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

#include <SDL/SDL_opengl.h>

#include "sg_table.h"

SG_Table::SG_Table(int xsz, int ysz, float xbor, float ybor)
	: SG_Alignment(xbor, ybor) {
  if(ysz < 1 || xsz < 1) {
    fprintf(stderr, "Illegal table geometry, (%dx%d)\n", xsz, ysz);
    exit(1);
    }
  xsize = xsz;
  ysize = ysz;
  xpos = 0;
  ypos = 0;
  }

SG_Table::~SG_Table() {
  vector<SG_Widget *> tmp = widgets;
  vector<SG_Widget *>::iterator itr = tmp.begin();
  widgets.clear();
  wgeom.clear();
  for(; itr != tmp.end(); ++itr) {
    if(*itr) delete (*itr);
    }
  }

int SG_Table::HandleEvent(SDL_Event *event, float x, float y) {
//  if(event->type == SDL_MOUSEBUTTONDOWN)
//    fprintf(stderr, "Table/Handle: Button Down at (%f,%f)\n", x, y);

  if(flags & SG_WIDGET_FLAGS_IGNORE) return -1; //Ignore all events
  if(flags & SG_WIDGET_FLAGS_DISABLED) return 0; //Eat all events

  int ret = 1;

  vector<SG_Widget *>::iterator itrw = widgets.begin();
  vector<SG_TableGeometry>::iterator itrg = wgeom.begin();
  for(; itrw != widgets.end(); ++itrw, ++itrg) {
    CalcGeometry(itrg);
    if(x >= cur_geom.xp-cur_geom.xs && x <= cur_geom.xp+cur_geom.xs
	&& y >= cur_geom.yp-cur_geom.ys && y <= cur_geom.yp+cur_geom.ys) {
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

  return ret;
  }

bool SG_Table::HandEventTo(SG_Widget *targ, SDL_Event *event,
		float x, float y) {
//  if(event->type == SDL_MOUSEBUTTONUP)
//    fprintf(stderr, "Table/Hand: Button Up at (%f,%f)\n", x, y);

  if(targ == this) return HandleEvent(event, x, y);

  vector<SG_Widget *>::iterator itrw = widgets.begin();
  vector<SG_TableGeometry>::iterator itrg = wgeom.begin();
  for(; itrw != widgets.end(); ++itrw, ++itrg) {
    CalcGeometry(itrg);
    if((*itrw)->HasWidget(targ)) {
      x -= cur_geom.xp;	//Scale the coordinates to widget's relative coords
      y -= cur_geom.yp;
      x /= cur_geom.xs;
      y /= cur_geom.ys;
      return (*itrw)->HandEventTo(targ, event, x, y);
      }
    }

  if(background->HasWidget(targ))
    return background->HandEventTo(targ, event, x, y);

  return 1;
  }

bool SG_Table::Render(unsigned long cur_time) {
//  fprintf(stderr, "Rendering Table %p!\n", this);

  if(flags & SG_WIDGET_FLAGS_HIDDEN) return 1;

  glPushMatrix();

  if(background) background->Render(cur_time);	//Same "layer" as parent
  glTranslatef(0.0, 0.0, 0.0625);		//Advance to next "layer"

  vector<SG_Widget *>::iterator itrw = widgets.begin();
  vector<SG_TableGeometry>::iterator itrg = wgeom.begin();
  for(; itrw != widgets.end(); ++itrw, ++itrg) {
    if(*itrw) {
      glPushMatrix();
      CalcGeometry(itrg);
      glTranslatef(cur_geom.xp, cur_geom.yp, 0.0);
      glScalef(cur_geom.xs, cur_geom.ys, 1.0);
      (*itrw)->Render(cur_time);
      glPopMatrix();
      }
    }

  glPopMatrix();

//  fprintf(stderr, "  Done.\n\n");

  return 1;
  }

bool SG_Table::AddWidget(SG_Widget *wid) {
  SG_TableGeometry geom = { xpos, ypos, 1, 1 };
  widgets.push_back(wid);
  wgeom.push_back(geom);
  ++xpos;
  if(xpos >= xsize) {
    xpos = 0; ++ypos;
    if(ypos >= ysize) ypos = 0;
    }
  return 1;
  }

bool SG_Table::AddWidget(SG_Widget *wid, int x1, int y1, int xs, int ys) {
  if(x1 >= xsize || x1 < 0 || x1+xs > xsize || xs < 1
	|| y1 >= ysize || y1 < 0 || y1+ys > ysize || ys < 1) {
    fprintf(stderr, "Illegal table placement, (%d,%d)/%dx%d in (%dx%d)\n",
	x1, y1, xs, ys, xsize, ysize);
    exit(1);
    }

  SG_TableGeometry geom = { x1, y1, xs, ys };
  widgets.push_back(wid);
  wgeom.push_back(geom);
  return 1;
  }

void SG_Table::RemoveWidget(SG_Widget *wid) {
  vector<SG_Widget *>::iterator itrw = widgets.begin();
  vector<SG_TableGeometry>::iterator itrg = wgeom.begin();
  for(; itrw != widgets.end(); ++itrw, ++itrg) {
    if((*itrw) == wid) {
      itrw = widgets.erase(itrw);
      itrg = wgeom.erase(itrg);
      --itrw;  // Undo ++itrw from for() loop.
      --itrg;  // Undo ++itrg from for() loop.
      }
    }
  }

//  bool SG_Table::SetDefaultCursor(GL_MODEL *cur);
  
//  static GL_MODEL SG_Table::Default_Mouse_Cursor = NULL;

void SG_Table::CalcGeometry(const vector<SG_TableGeometry>::iterator &geom) {
  float xcs, ycs; //Relative Cell Sizes.
  float xcp, ycp; //Center Cell Poisiton.

  xcs = 2.0 / xsize;
  ycs = 2.0 / ysize;

  xcp = float((*geom).xpos) + float((*geom).xsize) / 2.0;
  ycp = float((*geom).ypos) + float((*geom).ysize) / 2.0;

  cur_geom.xp = -1.0 + xcs * xcp;
  cur_geom.yp = 1.0 - ycs * ycp;
  cur_geom.xs = xcs * float((*geom).xsize) / 2.0 - xborder/float(xsize);
  cur_geom.ys = ycs * float((*geom).ysize) / 2.0 - yborder/float(ysize);

//  fprintf(stderr, "Calced: (%f,%f) %fx%f\n",
//	cur_geom.xp, cur_geom.yp, cur_geom.xs, cur_geom.ys);
  }
