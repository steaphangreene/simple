#ifndef	SIMPLEGUI_H
#define	SIMPLEGUI_H

#include <SDL/SDL.h>

#define ASPECT_NO_SUPPORT	0
#define ASPECT_EXPANDING_X	1
#define ASPECT_EXPANDING_Y	2
#define ASPECT_FIXED_X		4
#define ASPECT_FIXED_Y		8

class SG_Widget;
class SG_Alignment;

class SimpleGUI {
public:
  SimpleGUI(int aspmeth, float asp);
  ~SimpleGUI();
  bool Render(unsigned long cur_time = 0);
  bool ProcessEvent(SDL_Event *event);
  SG_Alignment *MasterWidget() { return mWid; };
  void SetCurrentWidget(SG_Widget *wid) { current_widget = wid; }
  void UnsetCurrentWidget() { SetCurrentWidget(NULL); }

protected:
  bool Resize(float asp);
  SG_Alignment *mWid;
  int aspect_method;
  float aspect;
  float aspect_actual;
  SG_Widget *current_widget;
//  GL_MODEL *mouse_cursor;
//  SG_Editable *focus_widget;

  int ScreenToRelative(float &x, float &y);
  int xoffset, yoffset;	//Current Screen Geometry
  int xsize, ysize;	//Current Screen Geometry

  float mousex, mousey;	//Current Mouse Position

  unsigned long mb_state;	// Bit Vector of Mouse Button States
				// Can't handle more than 32 Buttons
  };

#include "sg_events.h"
#include "sg_widget.h"
#include "sg_alignment.h"
#include "sg_button.h"
#include "sg_panel.h"
#include "sg_passthrough.h"
#include "sg_textarea.h"
#include "sg_table.h"

#endif	//SIMPLEGUI_H

