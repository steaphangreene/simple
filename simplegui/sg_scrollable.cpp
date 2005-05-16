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

#include <cmath>
using namespace std;

#include "SDL_opengl.h"

#include "sg_scrollable.h"
#include "sg_textarea.h"
#include "sg_table.h"
#include "sg_events.h"

SG_Scrollable::SG_Scrollable(float xspn, float yspn, float xval, float yval,
	float xmin, float ymin, float xmax, float ymax)
	: SG_Alignment(0.0, 0.0),
	  SG_Ranger2D(xspn, yspn, xval, yval, xmin, ymin, xmax, ymax) {
  subwidget_handles = false;
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
    SG_AlignmentGeometry adj_geom = cur_geom;
    widgets[0]->AdjustGeometry(&adj_geom);
    if(x >= adj_geom.xp-adj_geom.xs && x <= adj_geom.xp+adj_geom.xs
	&& y >= adj_geom.yp-adj_geom.ys && y <= adj_geom.yp+adj_geom.ys) {
      float back_x = x, back_y = y;

      x -= cur_geom.xp; //Scale the coordinates to widget's relative coords
      y -= cur_geom.yp;
      x /= cur_geom.xs;
      y /= cur_geom.ys;
      ret = widgets[0]->HandleEvent(event, x, y);
      if(ret != -1) return ret;

      x = back_x; y = back_y;
      }
    }

  if(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == 4) {
    SetYValue(YValue() - 0.5);
    event->type = SDL_SG_EVENT;
    event->user.code = SG_EVENT_NEEDTORENDER;
    event->user.data1 = NULL;
    event->user.data2 = NULL;
    return 1;
    }
  else if(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == 5) {
    SetYValue(YValue() + 0.5);
    event->type = SDL_SG_EVENT;
    event->user.code = SG_EVENT_NEEDTORENDER;
    event->user.data1 = NULL;
    event->user.data2 = NULL;
    return 1;
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

  if(subwidget_handles) return SG_Alignment::Render(cur_time);

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
      widgets[0]->AdjustGeometry(&cur_geom);
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
  double xfac = 1.0, yfac = 1.0;
  double xbas = 0.0, ybas = 0.0;
  double xoff = 0.0, yoff = 0.0;
  double xrat = 0.0, yrat = 0.0;

  if(XSpan() > 0.0 && XMax() != XMin()) {
    xfac = fabs(XMax() - XMin()) / XSpan();
    xbas = xfac - 1.0;
    xrat = fabs(XValue() - XMin()) / (fabs(XMax() - XMin()) - XSpan());
    if(xbas > 0.0) xoff = xrat * xbas * 2.0;
    }
  if(YSpan() > 0.0 && YMax() != YMin()) {
    yfac = fabs(YMax() - YMin()) / YSpan();
    ybas = yfac - 1.0;
    yrat = fabs(YValue() - YMin()) / (fabs(YMax() - YMin()) - YSpan());
//    fprintf(stderr, "yval = %f, ymin = %f, ymax = %f, yspn = %f\n",
//	YValue(), YMin(), YMax(), YSpan());
    if(ybas > 0.0) yoff = yrat * ybas * 2.0;
    }

//  fprintf(stderr, "yrat = %f, ybas = %f, yoff = %f\n", yrat, ybas, yoff);

//  cur_geom.xp = 0.0;
//  cur_geom.yp = 0.0;
//  cur_geom.xp = (-(xfac - 1.0) / 2.0) - xoff;
//  cur_geom.yp = (-(yfac - 1.0) / 2.0) + yoff;
//  cur_geom.xp = -xoff;
//  cur_geom.yp = yoff;
  cur_geom.xp = (xbas - xoff);
  cur_geom.yp = -(ybas - yoff);
  cur_geom.xs = xfac - xborder;
  cur_geom.ys = yfac - yborder;
  }

bool SG_Scrollable::AddWidget(SG_Widget *wid) {
  subwidget_handles = false;
  return SG_Alignment::AddWidget(wid);
  }

bool SG_Scrollable::AddWidget(SG_Table *tab) {
  subwidget_handles = false;
  int ret = SG_Alignment::AddWidget((SG_Widget*)tab);	//Add it normally
  if(ret) tab->LinkResize(this);			//Link its resizing in
  return ret;
  }

bool SG_Scrollable::AddWidget(SG_TextArea *text) {
  subwidget_handles = true;
  int ret = SG_Alignment::AddWidget((SG_Widget*)text);	//Add it normally
  if(ret) text->LinkTo(this);				//Link its sizing in
  return ret;
  }
