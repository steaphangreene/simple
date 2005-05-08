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

// This file was created from (or actually IS) a basic compound widget
// def, so it's not defined and is really just a place-holder for now.

#include "SDL_opengl.h"

#include "sg_dndboxes.h"
#include "sg_panel.h"
#include "sg_dragable.h"
#include "sg_events.h"
#include "sg_globals.h"

SG_DNDBoxes::SG_DNDBoxes(int xsz, int ysz)
	: SG_Compound(xsz, ysz, 0.0, 0.0) {
  present.resize(xsize*ysize, false);
  basecell.resize(xsize*ysize, false);
  invalids.resize(xsize*ysize, 0);
  cellids.resize(xsize*ysize, 0);
  }

SG_DNDBoxes::~SG_DNDBoxes() {
  }

bool SG_DNDBoxes::Render(unsigned long cur_time) {
  if(flags & SG_WIDGET_FLAGS_HIDDEN) return 1;

  if(background) background->Render(cur_time);
  SG_Widget *tmpback = background;
  background = NULL;

  glPushMatrix();

  glTranslatef(0.0, 0.0, 0.03125);		//Advance to next half "layer"

  glBegin(GL_LINES);
  for(int x = 0; x <= xsize; ++x) {
    for(int y = 0; y <= ysize; ++y) {
      int num_cells;

      num_cells = 0;
      if(x < xsize && y < ysize && present[y*xsize+x]) ++num_cells; 
      if(x < xsize && y > 0 && present[(y-1)*xsize+x]) ++num_cells; 
      if(num_cells > 1 && (!basecell[y*xsize+x])) num_cells = 0;
      if(num_cells > 0) {
	int xrun = 1;
	double bor_col = 1.0;
	if(num_cells > 1) {	// Is between two active cells
	  bor_col = 0.5;
	  while((x+xrun) < xsize && present[y*xsize+x+xrun]
		&& (!basecell[y*xsize+x+xrun])) ++xrun;
	  }
	glColor3f(bor_col, bor_col, bor_col);
	glVertex3f(-1.0 + (2.0*((float)(x+xrun))/((float)(xsize))), 
		1.0 - (2.0*((float)(y))/((float)(ysize))), 0.0);
	glVertex3f(-1.0 + (2.0*((float)(x))/((float)(xsize))), 
		1.0 - (2.0*((float)(y))/((float)(ysize))), 0.0);
	}

      num_cells = 0;
      if(y < ysize && x < xsize && present[y*xsize+x]) ++num_cells; 
      if(y < ysize && x > 0 && present[y*xsize+x-1]) ++num_cells; 
      if(num_cells > 1 && (!basecell[y*xsize+x])) num_cells = 0;
      if(num_cells > 0) {
	int yrun = 1;
	double bor_col = 1.0;
	if(num_cells > 1) {	// Is between two active cells
	  bor_col = 0.5;
	  while((y+yrun) < ysize && present[(y+yrun)*xsize+x]
		&& (!basecell[(y+yrun)*xsize+x])) ++yrun;
	  }
	glColor3f(bor_col, bor_col, bor_col);
	glVertex3f(-1.0 + (2.0*((float)(x))/((float)(xsize))), 
		1.0 - (2.0*((float)(y))/((float)(ysize))), 0.0);
	glVertex3f(-1.0 + (2.0*((float)(x))/((float)(xsize))), 
		1.0 - (2.0*((float)(y+yrun))/((float)(ysize))), 0.0);
	}

      if(x < xsize && y < ysize && basecell[y*xsize+x]) {
	glColor3f(0.5, 0.5, 0.5);
	glVertex3f(-1.0 + (2.0*((float)(x))/((float)(xsize))), 
		1.0 - (2.0*((float)(y))/((float)(ysize))), 0.0);
	glVertex3f(-1.0 + (2.0*((float)(x+1))/((float)(xsize))), 
		1.0 - (2.0*((float)(y+1))/((float)(ysize))), 0.0);
	}
      }
    }
  glEnd();

  glPopMatrix();

  int ret = SG_Table::Render(cur_time);
  background = tmpback;
  return ret;
  }

bool SG_DNDBoxes::AddItem(SDL_Surface *icon, int x1, int y1, int xs, int ys) {
  if(x1 >= xsize || x1 < 0 || x1+xs > xsize || xs < 1
        || y1 >= ysize || y1 < 0 || y1+ys > ysize || ys < 1) {
    fprintf(stderr, "Illegal DND add, (%d,%d)/%dx%d in (%dx%d)\n",
        x1, y1, xs, ys, xsize, ysize);
    return 0;
    }

  for(int x = x1; x < x1+xs; ++x) {
    for(int y = y1; y < y1+ys; ++y) {
      if(!present[y*xsize + x]) {
	fprintf(stderr, "Illegal DND add, cell (%d,%d) not present.\n", x, y);
	return 0;
	}
      }
    }

  float mnx, mny, mxx, mxy;	//Limits
  mnx = -(float)(x1) / (float)(xs);
  mny = -(float)(y1) / (float)(ys);
  mxx = (float)(xsize-(x1+xs)) / (float)(xs);
  mxy = (float)(ysize-(y1+ys)) / (float)(ys);

  SG_Dragable *but = new SG_Dragable(icon);
  but->SetLimits(mnx, mny, mxx, mxy);
  but->SetDisplayLimits(mnx * 2.0, mny * 2.0, mxx * 2.0, mxy * 2.0);
  but->SetTransparent();

  SG_Table::AddWidget(but, x1, y1, xs, ys);
  return 0;
  }
                                                                                
bool SG_DNDBoxes::ChildEvent(SDL_Event *event) {
  if(event->user.code == SG_EVENT_DRAGRELEASE) {
    ((SG_Ranger2D*)event->user.data1)->SetValues(0.0, 0.0);
    event->user.code = SG_EVENT_NEEDTORENDER;
    event->user.data1 = NULL;
    event->user.data2 = NULL;
    return 1;
    }
  else if(event->user.code == SG_EVENT_MOVE2D) {
    }
  return 0; // Silence children doing other things
  }

//  bool SG_DNDBoxes::SetDefaultCursor(GL_MODEL *cur);
  
//  static GL_MODEL SG_DNDBoxes::Default_Mouse_Cursor = NULL;

bool SG_DNDBoxes::AddWidget(SG_Widget *wid) {
  fprintf(stderr, "WARNING: Called %s on DNDBoxes - INVALID!\n", __FUNCTION__);
  return 0;
  }
                                                                                
bool SG_DNDBoxes::AddWidget(SG_Widget *wid, int x1, int y1, int xs, int ys) {
  fprintf(stderr, "WARNING: Called %s on DNDBoxes - INVALID!\n", __FUNCTION__);
  return 0;
  }
                                                                                
void SG_DNDBoxes::RemoveWidget(SG_Widget *wid) {
  fprintf(stderr, "WARNING: Called %s on DNDBoxes - INVALID!\n", __FUNCTION__);
  }

void SG_DNDBoxes::Exclude(int x1, int y1, int xs, int ys) {
  if(x1 >= xsize || x1 < 0 || x1+xs > xsize || xs < 1
        || y1 >= ysize || y1 < 0 || y1+ys > ysize || ys < 1) {
    fprintf(stderr, "Illegal DNDBox Exclude, (%d,%d)/%dx%d in (%dx%d)\n",
        x1, y1, xs, ys, xsize, ysize);
    exit(1);
    }

  for(int x = x1; x < x1+xs; ++x) {
    for(int y = y1; y < y1+ys; ++y) {
      present[y*xsize + x] = false;
      }
    }
  }

void SG_DNDBoxes::Include(int x1, int y1, int xs, int ys, int xcs, int ycs,
	Uint32 id, Uint32 inv) {
  if(x1 >= xsize || x1 < 0 || x1+xs > xsize || xs < 1
        || y1 >= ysize || y1 < 0 || y1+ys > ysize || ys < 1) {
    fprintf(stderr, "Illegal DNDBox Include, (%d,%d)/%dx%d in (%dx%d)\n",
        x1, y1, xs, ys, xsize, ysize);
    exit(1);
    }

  for(int x = x1; x < x1+xs; ++x) {
    for(int y = y1; y < y1+ys; ++y) {
      present[y*xsize + x] = true;
      if((x - x1) % xcs == 0 && (y - y1) % ycs == 0) {	//Is it a base cell?
	basecell[y*xsize + x] = true;
	}
      cellids[y*xsize + x] = id;
      invalids[y*xsize + x] = inv;
      }
    }
  }
