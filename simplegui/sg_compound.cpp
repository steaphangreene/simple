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

#include "sg_events.h"
#include "sg_compound.h"

SG_Compound::SG_Compound(int xsz, int ysz, float xbor, float ybor)
	: SG_Table(xsz, ysz, xbor, ybor) {
  }

SG_Compound::~SG_Compound() {	//Even need this?
  vector<SG_Widget *> tmp = widgets;
  vector<SG_Widget *>::iterator itr = tmp.begin();
  widgets.clear();
  wgeom.clear();
  for(; itr != tmp.end(); ++itr) {
    if(*itr) delete (*itr);
    }
  }

bool SG_Compound::HandleEvent(SDL_Event *event, float x, float y) {
//  if(event->type == SDL_MOUSEBUTTONDOWN)
//    fprintf(stderr, "Compound/Handle: Button Down at (%f,%f)\n", x, y);

  int ret = 1;

  vector<SG_Widget *>::iterator itrw = widgets.begin();
  vector<SG_TableGeometry>::iterator itrg = wgeom.begin();
  for(; itrw != widgets.end(); ++itrw, ++itrg) {
    CalcGeometry(itrg);
    if(x >= cur_geom.xp-cur_geom.xs && x <= cur_geom.xp+cur_geom.xs
	&& y >= cur_geom.yp-cur_geom.ys && y <= cur_geom.yp+cur_geom.ys) {
      x -= cur_geom.xp;	//Scale the coordinates to widget's relative coords
      y -= cur_geom.yp;
      x /= cur_geom.xs;
      y /= cur_geom.ys;
      ret = (*itrw)->HandleEvent(event, x, y);
      break;
      }
    }

  if(ret && event->type == SDL_SG_EVENT) {
    return ChildEvent(event);
    }

  if(background) ret = background->HandleEvent(event, x, y);

  if(ret && event->type == SDL_SG_EVENT) {
    return ChildEvent(event);
    }

  return ret;
  }

bool SG_Compound::HandMouseEventTo(SG_Widget *targ, SDL_Event *event,
		float x, float y) {
//  if(event->type == SDL_MOUSEBUTTONUP)
//    fprintf(stderr, "Compound/Hand: Button Up at (%f,%f)\n", x, y);

  if(targ == this) return HandleEvent(event, x, y);

  int ret = 1;

  vector<SG_Widget *>::iterator itrw = widgets.begin();
  vector<SG_TableGeometry>::iterator itrg = wgeom.begin();
  for(; itrw != widgets.end(); ++itrw, ++itrg) {
    CalcGeometry(itrg);
    if((*itrw)->HasWidget(targ)) {
      x -= cur_geom.xp;	//Scale the coordinates to widget's relative coords
      y -= cur_geom.yp;
      x /= cur_geom.xs;
      y /= cur_geom.ys;
      ret = (*itrw)->HandMouseEventTo(targ, event, x, y);
      break;
      }
    }

  if(ret && event->type == SDL_SG_EVENT) {
    return ChildEvent(event);
    }

  if(background->HasWidget(targ))
    ret = background->HandMouseEventTo(targ, event, x, y);

  if(ret && event->type == SDL_SG_EVENT) {
    return ChildEvent(event);
    }

  return ret;
  }

int SG_Compound::ChildEvent(SDL_Event *event) {
  return 1;
  }

//  bool SG_Compound::SetDefaultCursor(GL_MODEL *cur);
  
//  static GL_MODEL SG_Compound::Default_Mouse_Cursor = NULL;
