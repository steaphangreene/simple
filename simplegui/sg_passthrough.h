#ifndef SG_PASSTHROUGH_H
#define SG_PASSTHROUGH_H

#include "sg_alignment.h"

class SG_PassThrough : public SG_Alignment {
public:
  SG_PassThrough(int la, int ma, int ra);
  virtual ~SG_PassThrough();
  virtual bool HandleMouseEvent(SDL_Event *event, float x, float y);
  virtual bool Render();
//  virtual bool SetDefaultCursor(GL_MODEL *cur);
  
protected:
  int button_action[3];	//Auto-Handling of the main three buttons
  int cur_action, cur_button;
  float act_x, act_y;
  float cur_x, cur_y;

  void CalcGeometry();

//  static GL_MODEL Default_Mouse_Cursor;
  };

#define SG_PT_IGNORE	0
#define SG_PT_BLOCK	1
#define SG_PT_CLICK	2
#define SG_PT_BOX	3

#endif // SG_PASSTHROUGH_H

