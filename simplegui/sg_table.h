#ifndef SG_TABLE_H
#define SG_TABLE_H

#include "sg_alignment.h"

#include <vector>
using namespace std;

struct SG_TableGeometry {
  int col, row; //Starting Column/Row
  int cols, rows; //Column/Row Size
  };

class SG_Table : public SG_Alignment {
public:
  SG_Table(int cls, int rws, float xbor = 0.0, float ybor = 0.0);
  virtual ~SG_Table();
  virtual bool HandleMouseEvent(SDL_Event *event, float x, float y);
  virtual bool HandMouseEventTo(SG_Widget *targ, SDL_Event *event,
		float x, float y);
  virtual bool Render();
//  virtual bool SetDefaultCursor(GL_MODEL *cur);
  virtual bool AddWidget(SG_Widget *wid, int c1, int r1, int cs=1, int rs=1);
  virtual bool AddWidget(SG_Widget *wid);
  virtual void RemoveWidget(SG_Widget *wid);
  
protected:
  void CalcGeometry(const vector<SG_TableGeometry>::iterator &geom);

//  static GL_MODEL Default_Mouse_Cursor;
  int cols, rows;	//Geometry of Table
  int col, row;		//Current Row/Column
  vector<SG_TableGeometry> wgeom;
  };

#endif // SG_TABLE_H

