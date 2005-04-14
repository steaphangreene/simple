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

SG_Scrollable::SG_Scrollable(float xfac, float yfac, float xoff, float yoff)
	: SG_Alignment(0.0, 0.0) {
  SetFactors(xfac, yfac);
  SetOffsets(xoff, yoff);
  }

SG_Scrollable::~SG_Scrollable() {
  vector<SG_Widget *> tmp = widgets;	//Not necessary?
  vector<SG_Widget *>::iterator itrw = tmp.begin();
  widgets.clear();
  for(; itrw != tmp.end(); ++itrw) {
    if(*itrw) delete (*itrw);
    }
  }

void SG_Scrollable::SetFactors(float xfac, float yfac) {
  xfactor = xfac;
  yfactor = yfac;
  }

void SG_Scrollable::SetXFactor(float xfac) {
  xfactor = xfac;
  }

void SG_Scrollable::SetYFactor(float yfac) {
  yfactor = yfac;
  }

void SG_Scrollable::SetOffsets(float xoff, float yoff) {
  xoffset = xoff;
  yoffset = yoff;
  }

void SG_Scrollable::SetXOffset(float xoff) {
  xoffset = xoff;
  }

void SG_Scrollable::SetYOffset(float yoff) {
  yoffset = yoff;
  }

int SG_Scrollable::HandleEvent(SDL_Event *event, float x, float y) {
//  if(event->type == SDL_MOUSEBUTTONDOWN)
//    fprintf(stderr, "Align/Handle: Button Down at (%f,%f)\n", x, y);

  if(flags & SG_WIDGET_FLAGS_IGNORE) return -1; //Ignore all events
  if(flags & SG_WIDGET_FLAGS_DISABLED) return 0; //Eat all events

  int ret = 1;

  if(widgets.size() >= 1 && widgets[0]) {
    CalcGeometry();
    if(x >= cur_geom.xp-cur_geom.xs && x <= cur_geom.xp+cur_geom.xs
	&& y >= cur_geom.yp-cur_geom.ys && y <= cur_geom.yp+cur_geom.ys) {
      float back_x = x, back_y = y;
      
      x -= cur_geom.xp; //Scale the coordinates to widget's relative coords
      y -= cur_geom.yp;
      x /= cur_geom.xs;
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
    if(widgets[0]->HasWidget(targ)) {
      CalcGeometry();
      x -= cur_geom.xp; //Scale the coordinates to widget's relative coords
      y -= cur_geom.yp;
      x /= cur_geom.xs;
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

  if(flags & SG_WIDGET_FLAGS_HIDDEN) return true;

  if(xfactor <= 0.0 || yfactor <= 0.0) return true;

  glPushMatrix();

  if(background) background->Render(cur_time);	//Same "layer" as parent
  glTranslatef(0.0, 0.0, 0.0625);		//Advance to next "layer"

  bool st0, st1, st2, st3;
  GLdouble oeq0[4], oeq1[4], oeq2[4], oeq3[4];

  glGetClipPlane(GL_CLIP_PLANE0, oeq0);
  glGetClipPlane(GL_CLIP_PLANE1, oeq1);
  glGetClipPlane(GL_CLIP_PLANE2, oeq2);
  glGetClipPlane(GL_CLIP_PLANE3, oeq3);

  st0 = glIsEnabled(GL_CLIP_PLANE0);
  st1 = glIsEnabled(GL_CLIP_PLANE1);
  st2 = glIsEnabled(GL_CLIP_PLANE2);
  st3 = glIsEnabled(GL_CLIP_PLANE3);

  GLdouble eq0[4] = { -1.0,  0.0,  0.0,  1.0 - xborder };	// x <= 1.0
  GLdouble eq1[4] = {  1.0,  0.0,  0.0,  1.0 - xborder };	// x >= -1.0
  GLdouble eq2[4] = {  0.0, -1.0,  0.0,  1.0 - yborder };	// y <= 1.0
  GLdouble eq3[4] = {  0.0,  1.0,  0.0,  1.0 - yborder };	// y >= -1.0

  glClipPlane(GL_CLIP_PLANE0, eq0);
  glClipPlane(GL_CLIP_PLANE1, eq1);
  glClipPlane(GL_CLIP_PLANE2, eq2);
  glClipPlane(GL_CLIP_PLANE3, eq3);

  if(!st0) glEnable(GL_CLIP_PLANE0);
  if(!st1) glEnable(GL_CLIP_PLANE1);
  if(!st2) glEnable(GL_CLIP_PLANE2);
  if(!st3) glEnable(GL_CLIP_PLANE3);

  vector<SG_Widget *>::iterator itrw = widgets.begin();
  for(; itrw != widgets.end(); ++itrw) {
    if(*itrw) {
      glPushMatrix();
      CalcGeometry();
      glTranslatef(cur_geom.xp, cur_geom.yp, 0.0);
      glScalef(cur_geom.xs, cur_geom.ys, 1.0);
      (*itrw)->Render(cur_time);
      glPopMatrix();
      }
    }


  glClipPlane(GL_CLIP_PLANE0, oeq0);
  glClipPlane(GL_CLIP_PLANE1, oeq1);
  glClipPlane(GL_CLIP_PLANE2, oeq2);
  glClipPlane(GL_CLIP_PLANE3, oeq3);

  if(!st0) glDisable(GL_CLIP_PLANE0);
  if(!st1) glDisable(GL_CLIP_PLANE1);
  if(!st2) glDisable(GL_CLIP_PLANE2);
  if(!st3) glDisable(GL_CLIP_PLANE3);

  glPopMatrix();

  return 1;
  }

//  bool SG_Scrollable::SetDefaultCursor(GL_MODEL *cur);
  
//  static GL_MODEL SG_Scrollable::Default_Mouse_Cursor = NULL;

void SG_Scrollable::CalcGeometry() {
  double xoff = 0.0, yoff = 0.0;

  if(xfactor > 1.0) xoff = xoffset * (xfactor - 1.0);
  if(yfactor > 1.0) yoff = yoffset * (yfactor - 1.0);

  cur_geom.xp = -xoff + (xfactor - 1.0);
  cur_geom.yp = yoff - (yfactor - 1.0);
  cur_geom.xs = xfactor - xborder;
  cur_geom.ys = yfactor - yborder;
  }
