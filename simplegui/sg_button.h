#ifndef SG_BUTTON_H
#define SG_BUTTON_H

#include "sg_textarea.h"

class SG_Button : public SG_TextArea {
public:
  SG_Button(string mes, float red, float green, float blue,
	float tred, float tgreen, float tblue,
	float cred, float cgreen, float cblue);
  virtual ~SG_Button();
  virtual bool HandleMouseEvent(SDL_Event *event, float x, float y);
//  virtual bool SetDefaultCursor(GL_MODEL *cur);
  
protected:
//  static GL_MODEL Default_Mouse_Cursor;
  };

#endif // SG_BUTTON_H

