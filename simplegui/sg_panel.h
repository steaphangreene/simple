#ifndef SG_PANEL_H
#define SG_PANEL_H

#include "sg_widget.h"

class SG_Panel : public SG_Widget {
public:
  SG_Panel(float red, float green, float blue);
  virtual ~SG_Panel();
  virtual bool HandleMouseEvent(SDL_Event *event, float x, float y);
  virtual bool Render();
//  virtual bool SetDefaultCursor(GL_MODEL *cur);
  
protected:
//  static GL_MODEL Default_Mouse_Cursor;
  float bg_r, bg_g, bg_b;
  };

#endif // SG_PANEL_H

