#include <GL/gl.h>

#include "sg_table.h"

SG_Table::SG_Table(int cls, int rws, float xbor, float ybor)
	: SG_Alignment(xbor, ybor) {
  if(rws < 1 || cls < 1) {
    fprintf(stderr, "Illegal table geometry, (%dx%d)\n", cls, rws);
    exit(1);
    }
  rows = rws;
  cols = cls;
  row = 0;
  col = 0;
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

bool SG_Table::HandleMouseEvent(SDL_Event *event, float x, float y) {
//  if(event->type == SDL_MOUSEBUTTONDOWN)
//    fprintf(stderr, "Table/Handle: Button Down at (%f,%f)\n", x, y);

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
      return (*itrw)->HandleMouseEvent(event, x, y);
      }
    }

  if(background) return background->HandleMouseEvent(event, x, y);

  return 1;
  }

bool SG_Table::HandMouseEventTo(SG_Widget *targ, SDL_Event *event,
		float x, float y) {
//  if(event->type == SDL_MOUSEBUTTONUP)
//    fprintf(stderr, "Table/Hand: Button Up at (%f,%f)\n", x, y);

  if(targ == this) return HandleMouseEvent(event, x, y);

  vector<SG_Widget *>::iterator itrw = widgets.begin();
  vector<SG_TableGeometry>::iterator itrg = wgeom.begin();
  for(; itrw != widgets.end(); ++itrw, ++itrg) {
    CalcGeometry(itrg);
    if((*itrw)->HasWidget(targ)) {
      x -= cur_geom.xp;	//Scale the coordinates to widget's relative coords
      y -= cur_geom.yp;
      x /= cur_geom.xs;
      y /= cur_geom.ys;
      return (*itrw)->HandMouseEventTo(targ, event, x, y);
      }
    }

  if(background->HasWidget(targ))
    return background->HandMouseEventTo(targ, event, x, y);

  return 1;
  }

bool SG_Table::Render() {
//  fprintf(stderr, "Rendering Table %p!\n", this);

  if(flags & SG_WIDGET_FLAGS_HIDDEN) return 1;

  glPushMatrix();

  if(background) background->Render();  //Same "layer" as parent
  glTranslatef(0.0, 0.0, 0.0625);       //Advance to next "layer"

  vector<SG_Widget *>::iterator itrw = widgets.begin();
  vector<SG_TableGeometry>::iterator itrg = wgeom.begin();
  for(; itrw != widgets.end(); ++itrw, ++itrg) {
    if(*itrw) {
      glPushMatrix();
      CalcGeometry(itrg);
      glTranslatef(cur_geom.xp, cur_geom.yp, 0.0);
      glScalef(cur_geom.xs, cur_geom.ys, 1.0);
      (*itrw)->Render();
      glPopMatrix();
      }
    }

  glPopMatrix();

//  fprintf(stderr, "  Done.\n\n");

  return 1;
  }

bool SG_Table::AddWidget(SG_Widget *wid) {
  SG_TableGeometry geom = { col, row, 1, 1 };
  widgets.push_back(wid);
  wgeom.push_back(geom);
  ++col;
  if(col >= cols) {
    col = 0; ++row;
    if(row >= rows) row = 0;
    }
  return 1;
  }

bool SG_Table::AddWidget(SG_Widget *wid, int c1, int r1, int cs, int rs) {
  if(c1 >= cols || c1 < 0 || c1+cs > cols || cs < 1
	|| r1 >= rows || r1 < 0 || r1+rs > rows || rs < 1) {
    fprintf(stderr, "Illegal table placement, (%d,%d)/%dx%d in (%dx%d)\n",
	c1, r1, cs, rs, cols, rows);
    exit(1);
    }

  SG_TableGeometry geom = { c1, r1, cs, rs };
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

  xcs = 2.0 / cols;
  ycs = 2.0 / rows;

  xcp = float((*geom).col) + float((*geom).cols) / 2.0;
  ycp = float((*geom).row) + float((*geom).rows) / 2.0;

  cur_geom.xp = -1.0 + xcs * xcp;
  cur_geom.yp = 1.0 - ycs * ycp;
  cur_geom.xs = xcs * float((*geom).cols) / 2.0 - xborder/float(cols);
  cur_geom.ys = ycs * float((*geom).rows) / 2.0 - yborder/float(rows);

//  fprintf(stderr, "Calced: (%f,%f) %fx%f\n",
//	cur_geom.xp, cur_geom.yp, cur_geom.xs, cur_geom.ys);
  }
