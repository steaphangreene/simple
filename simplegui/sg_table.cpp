// *************************************************************************
//  This file is part of the SimpleGUI Example Module by Steaphan Greene
//
//  Copyright 2005-2014 Steaphan Greene <steaphan@gmail.com>
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

#include "sg_table.h"

SG_Table::SG_Table(int xsz, int ysz, float xbor, float ybor)
    : SG_Alignment(xbor, ybor), ranger(xsz, ysz, 0.0, 0.0, 0.0, 0.0, xsz, ysz) {
  if (ysz < 0 || xsz < 0) {
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
  for (; itr != tmp.end(); ++itr) {
    if (*itr) delete (*itr);
  }
}

int SG_Table::HandleEvent(SDL_Event *event, float x, float y) {
  //  if(event->type == SDL_MOUSEBUTTONDOWN)
  //    fprintf(stderr, "Table/Handle: Button Down at (%f,%f)\n", x, y);

  if (flags & SG_WIDGET_FLAGS_IGNORE) return -1;   // Ignore all events
  if (flags & SG_WIDGET_FLAGS_DISABLED) return 0;  // Eat all events

  if (xsize <= 0 || ysize <= 0) return -1;

  int ret = HandleEdgeEvent(event, x, y);
  if (ret) return ret;

  vector<SG_Widget *>::iterator itrw = widgets.begin();
  vector<SG_TableGeometry>::iterator itrg = wgeom.begin();
  for (; itrw != widgets.end(); ++itrw, ++itrg) {
    SG_AlignmentGeometry geom;
    CalcGeometry(geom, itrg);
    SG_AlignmentGeometry adj_geom = geom;
    (*itrw)->AdjustGeometry(&adj_geom);
    if (x >= adj_geom.xp - adj_geom.xs && x <= adj_geom.xp + adj_geom.xs &&
        y >= adj_geom.yp - adj_geom.ys && y <= adj_geom.yp + adj_geom.ys) {
      float back_x = x, back_y = y;

      x -= geom.xp;  // Scale the coordinates to widget's relative coords
      y -= geom.yp;
      x /= geom.xs;
      y /= geom.ys;
      ret = (*itrw)->HandleEvent(event, x, y);
      if (ret != -1) return ret;

      x = back_x;
      y = back_y;
    }
  }

  if (background) {
    ret = background->HandleEvent(event, x, y);
    if (ret != -1) return ret;
  }

  if (event->type == SDL_MOUSEBUTTONDOWN &&
      (event->button.button == 4 || event->button.button == 5)) {
    // Allow mousewheel events to pass through
    return -1;
  }

  return ret;
}

bool SG_Table::HandEventTo(SG_Widget *targ, SDL_Event *event, float x,
                           float y) {
  //  if(event->type == SDL_MOUSEBUTTONUP)
  //    fprintf(stderr, "Table/Hand: Button Up at (%f,%f)\n", x, y);

  if (targ == this) return HandleEvent(event, x, y);

  if (xsize <= 0 || ysize <= 0) return false;

  vector<SG_Widget *>::iterator itrw = widgets.begin();
  vector<SG_TableGeometry>::iterator itrg = wgeom.begin();
  for (; itrw != widgets.end(); ++itrw, ++itrg) {
    if ((*itrw)->HasWidget(targ)) {
      SG_AlignmentGeometry geom;
      CalcGeometry(geom, itrg);
      x -= geom.xp;  // Scale the coordinates to widget's relative coords
      y -= geom.yp;
      x /= geom.xs;
      y /= geom.ys;
      return (*itrw)->HandEventTo(targ, event, x, y);
    }
  }

  if (background->HasWidget(targ))
    return background->HandEventTo(targ, event, x, y);

  return true;
}

bool SG_Table::RenderSelf(unsigned long cur_time) {
  //  fprintf(stderr, "Rendering Table %p!\n", this);

  if (xsize <= 0 || ysize <= 0) return true;

  glPushMatrix();

  if (background) background->Render(cur_time);  // Same "layer" as parent
  glTranslatef(0.0, 0.0, 0.0625);                // Advance to next "layer"

  vector<SG_Widget *>::iterator itrw = widgets.begin();
  vector<SG_TableGeometry>::iterator itrg = wgeom.begin();
  for (; itrw != widgets.end(); ++itrw, ++itrg) {
    if (*itrw) {
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

bool SG_Table::AddWidget(SG_Widget *wid) {
  SG_TableGeometry geom = {xpos, ypos, 1, 1};
  widgets.push_back(wid);
  wgeom.push_back(geom);
  ++xpos;
  if (xpos >= xsize) {
    xpos = 0;
    ++ypos;
    if (ypos >= ysize) ypos = 0;
  }
  SetAspectRatio(aspect_ratio);  // FIXME: Only need to send to new widget!
  return true;
}

bool SG_Table::AddWidget(SG_Widget *wid, int x1, int y1, int xs, int ys) {
  if (x1 >= xsize || x1 < 0 || x1 + xs > xsize || xs < 1 || y1 >= ysize ||
      y1 < 0 || y1 + ys > ysize || ys < 1) {
    fprintf(stderr, "Illegal table placement, (%d,%d)/%dx%d in (%dx%d)\n", x1,
            y1, xs, ys, xsize, ysize);
    exit(1);
  }

  SG_TableGeometry geom = {x1, y1, xs, ys};
  widgets.push_back(wid);
  wgeom.push_back(geom);
  SetAspectRatio(aspect_ratio);  // FIXME: Only need to send to new widget!
  return true;
}

void SG_Table::RemoveWidget(SG_Widget *wid) {
  vector<SG_Widget *>::iterator itrw = widgets.begin();
  vector<SG_TableGeometry>::iterator itrg = wgeom.begin();
  for (; itrw != widgets.end(); ++itrw, ++itrg) {
    if ((*itrw) == wid) {
      itrw = widgets.erase(itrw);
      itrg = wgeom.erase(itrg);
      --itrw;  // Undo ++itrw from for() loop.
      --itrg;  // Undo ++itrg from for() loop.
    }
  }
}

//  bool SG_Table::SetDefaultCursor(GL_MODEL *cur);

//  static GL_MODEL SG_Table::Default_Mouse_Cursor = NULL;

void SG_Table::CalcGeometry(SG_AlignmentGeometry &geom,
                            const vector<SG_TableGeometry>::iterator &wgeom) {
  float xcs, ycs;  // Relative Cell Sizes.
  float xcp, ycp;  // Center Cell Poisiton.

  xcs = 2.0 / xsize;
  ycs = 2.0 / ysize;

  xcp = float((*wgeom).xpos) + float((*wgeom).xsize) / 2.0;
  ycp = float((*wgeom).ypos) + float((*wgeom).ysize) / 2.0;

  geom.xp = -1.0 + xcs * xcp;
  geom.yp = 1.0 - ycs * ycp;
  geom.xs = xcs * float((*wgeom).xsize) / 2.0 - xborder / float(xsize);
  geom.ys = ycs * float((*wgeom).ysize) / 2.0 - yborder / float(ysize);
}

void SG_Table::Resize(int xsz, int ysz) {
  if (ysize > ysz || xsize > xsz) {  // Remove widgets that don't fit anymore
    vector<SG_Widget *>::iterator itrw = widgets.begin();
    vector<SG_TableGeometry>::iterator itrg = wgeom.begin();
    for (; itrw != widgets.end(); ++itrw, ++itrg) {
      if (itrg->xpos + itrg->xsize > xsz || itrg->ypos + itrg->ysize > ysz) {
        delete (*itrw);
        itrw = widgets.erase(itrw);
        itrg = wgeom.erase(itrg);
        --itrw;  // Undo ++itrw from for() loop.
        --itrg;  // Undo ++itrg from for() loop.
      }
    }
  }
  xsize = xsz;
  ysize = ysz;
  SendResize();
}

void SG_Table::AddCol(int xel) {
  if (xel > xsize || xel < 0) return;

  vector<SG_Widget *>::iterator itrw = widgets.begin();
  vector<SG_TableGeometry>::iterator itrg = wgeom.begin();
  for (; itrw != widgets.end(); ++itrw, ++itrg) {
    if (xel <= itrg->xpos) {  // Adjust those past it up one
      ++itrg->xpos;
    }
  }
  ++xsize;
  SendXResize();
}

void SG_Table::AddRow(int yel) {
  if (yel > ysize || yel < 0) return;

  vector<SG_Widget *>::iterator itrw = widgets.begin();
  vector<SG_TableGeometry>::iterator itrg = wgeom.begin();
  for (; itrw != widgets.end(); ++itrw, ++itrg) {
    if (yel <= itrg->ypos) {  // Adjust those past it up one
      ++itrg->ypos;
    }
  }
  ++ysize;
  SendYResize();
}

void SG_Table::ClearCol(int xel) {
  if (xel >= xsize || xel < 0) return;

  vector<SG_Widget *>::iterator itrw = widgets.begin();
  vector<SG_TableGeometry>::iterator itrg = wgeom.begin();
  for (; itrw != widgets.end(); ++itrw, ++itrg) {
    if (xel >= itrg->xpos && xel < itrg->xpos + itrg->xsize) {
      itrw = widgets.erase(itrw);  // Remove widgets that get nailed
      itrg = wgeom.erase(itrg);
      --itrw;  // Undo ++itrw from for() loop.
      --itrg;  // Undo ++itrg from for() loop.
    }
  }
}

void SG_Table::ClearRow(int yel) {
  if (yel >= ysize || yel < 0) return;

  vector<SG_Widget *>::iterator itrw = widgets.begin();
  vector<SG_TableGeometry>::iterator itrg = wgeom.begin();
  for (; itrw != widgets.end(); ++itrw, ++itrg) {
    if (yel >= itrg->ypos && yel < itrg->ypos + itrg->ysize) {
      itrw = widgets.erase(itrw);  // Remove widgets that get nailed
      itrg = wgeom.erase(itrg);
      --itrw;  // Undo ++itrw from for() loop.
      --itrg;  // Undo ++itrg from for() loop.
    }
  }
}

void SG_Table::RemoveCol(int xel) {
  if (xel >= xsize || xel < 0) return;

  vector<SG_Widget *>::iterator itrw = widgets.begin();
  vector<SG_TableGeometry>::iterator itrg = wgeom.begin();
  for (; itrw != widgets.end(); ++itrw, ++itrg) {
    if (xel >= itrg->xpos && xel < itrg->xpos + itrg->xsize) {
      itrw = widgets.erase(itrw);  // Remove widgets that get nailed
      itrg = wgeom.erase(itrg);
      --itrw;                       // Undo ++itrw from for() loop.
      --itrg;                       // Undo ++itrg from for() loop.
    } else if (xel < itrg->xpos) {  // And adjust those past it back one
      --itrg->xpos;
    }
  }
  --xsize;
  SendXResize();
}

void SG_Table::RemoveRow(int yel) {
  if (yel >= ysize || yel < 0) return;

  vector<SG_Widget *>::iterator itrw = widgets.begin();
  vector<SG_TableGeometry>::iterator itrg = wgeom.begin();
  for (; itrw != widgets.end(); ++itrw, ++itrg) {
    if (yel >= itrg->ypos && yel < itrg->ypos + itrg->ysize) {
      itrw = widgets.erase(itrw);  // Remove widgets that get nailed
      itrg = wgeom.erase(itrg);
      --itrw;                       // Undo ++itrw from for() loop.
      --itrg;                       // Undo ++itrg from for() loop.
    } else if (yel < itrg->ypos) {  // And adjust those past it back one
      --itrg->ypos;
    }
  }
  --ysize;
  SendYResize();
}

void SG_Table::RemoveCols(const vector<int> &xels) {  // FIXME: Do this better!
  vector<int>::const_iterator itr = xels.begin();
  for (; itr != xels.end(); ++itr) {
    RemoveCol(*itr);
  }
  //  SendXResize();	//Done in RemoveCol() for now.
}

void SG_Table::RemoveRows(const vector<int> &yels) {  // FIXME: Do this better!
  vector<int>::const_iterator itr = yels.begin();
  for (; itr != yels.end(); ++itr) {
    RemoveRow(*itr);
  }
  //  SendYResize();	//Done in RemoveRow() for now.
}

void SG_Table::Substitute(SG_Widget *oldwid, SG_Widget *newwid) {
  vector<SG_Widget *>::iterator itrw = widgets.begin();
  vector<SG_TableGeometry>::iterator itrg = wgeom.begin();
  for (; itrw != widgets.end(); ++itrw, ++itrg) {
    if ((*itrw) == oldwid) {
      (*itrw) = newwid;
      break;
    }
  }
}

void SG_Table::LinkResize(SG_Ranger2D *rang) {
  ranger.LinkFrom(rang);
  SendResize();
}

void SG_Table::LinkXResize(SG_Ranger2D *rang) {
  ranger.LinkXFrom(rang);
  SendXResize();
}

void SG_Table::LinkYResize(SG_Ranger2D *rang) {
  ranger.LinkYFrom(rang);
  SendYResize();
}

void SG_Table::LinkXResize(SG_Ranger *rang) {
  ranger.LinkXFrom(rang);
  SendXResize();
}

void SG_Table::LinkYResize(SG_Ranger *rang) {
  ranger.LinkYFrom(rang);
  SendYResize();
}

void SG_Table::SendResize() {
  ranger.SetLimits(0.0, 0.0, xsize, ysize);
  SetAspectRatio(aspect_ratio);
}

void SG_Table::SendXResize() {
  ranger.SetXLimits(0.0, xsize);
  SetAspectRatio(aspect_ratio);
}

void SG_Table::SendYResize() {
  ranger.SetYLimits(0.0, ysize);
  SetAspectRatio(aspect_ratio);
}

void SG_Table::SetActive(float xst, float yst, float xen, float yen) {
  ranger.SetActive(xst, yst, xen, yen);
}

void SG_Table::SetAspectRatio(float asp) {
  aspect_ratio = asp;
  if (background) background->SetAspectRatio(aspect_ratio);

  vector<SG_Widget *>::iterator itrw = widgets.begin();
  vector<SG_TableGeometry>::iterator itrg = wgeom.begin();
  for (; itrw != widgets.end(); ++itrw, ++itrg) {
    SG_AlignmentGeometry geom;
    CalcGeometry(geom, itrg);
    (*itrw)->AdjustGeometry(&geom);
    (*itrw)->SetAspectRatio(aspect_ratio * geom.xs / geom.ys);
  }
}

void SG_Table::RangerChanged() { SetAspectRatio(aspect_ratio); }
