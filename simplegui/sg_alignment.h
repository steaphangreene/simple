#ifndef SG_ALIGNMENT_H
#define SG_ALIGNMENT_H

#include "sg_widget.h"

#include <vector>
using namespace std;

struct SG_SubwidgetGeometry {
  float xp, yp; //Relative Position (Center)
  float xs, ys; //Relative Size (From Center)
  };

class SG_Alignment : public SG_Widget {
public:
  SG_Alignment(float xbor = 0.0, float ybor = 0.0);
  virtual ~SG_Alignment();
  void SetBorder(float xbor, float ybor);
  virtual bool HandleMouseEvent(SDL_Event *event, float x, float y);
  virtual bool HandMouseEventTo(SG_Widget *targ, SDL_Event *event,
		float x, float y);
  virtual bool HasWidget(SG_Widget *targ);
  virtual bool Render();
//  virtual bool SetDefaultCursor(GL_MODEL *cur);
  virtual bool AddWidget(SG_Widget *wid);
  virtual void RemoveWidget(SG_Widget *wid);
  void SetBackground(SG_Widget *wid) { background = wid; };
  void UnsetBackground() { background = NULL; };
  
protected:
  void CalcGeometry();
  SG_SubwidgetGeometry cur_geom;

//  static GL_MODEL Default_Mouse_Cursor;
  vector<SG_Widget *> widgets;
  SG_Widget *background;
  float xborder, yborder;
  };

#endif // SG_ALIGNMENT_H

