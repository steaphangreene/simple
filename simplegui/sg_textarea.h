#ifndef SG_TEXTAREA_H
#define SG_TEXTAREA_H

#include "sg_panel.h"

#include <string>

using namespace std;

class SG_TextArea : public SG_Panel {
public:
  SG_TextArea(string mes, float red, float green, float blue,
		float tred, float tgreen, float tblue);
  virtual ~SG_TextArea();
  virtual bool HandleMouseEvent(SDL_Event *event, float x, float y);
  virtual bool Render();
//  virtual bool SetDefaultCursor(GL_MODEL *cur);
  void SetMargins(float xmar, float ymar);
  
protected:
//  static GL_MODEL Default_Mouse_Cursor;
  float tx_r, tx_g, tx_b;
  float xmargin, ymargin;
  string message;
  };

#endif // SG_TEXTAREA_H

