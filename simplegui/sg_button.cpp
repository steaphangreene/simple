#include <GL/gl.h>

#include "sg_button.h"
#include "sg_events.h"
#include "sg_globals.h"

SG_Button::SG_Button(string mes, float red, float green, float blue,
	float tred, float tgreen, float tblue,
	float cred, float cgreen, float cblue)
		: SG_TextArea(mes, red, green, blue, tred, tgreen, tblue) {
  cl_r = cred, cl_g = cgreen, cl_b = cblue;
  }

SG_Button::~SG_Button() {
  }

bool SG_Button::HandleMouseEvent(SDL_Event *event, float x, float y) {
//  if(event->type == SDL_MOUSEBUTTONDOWN)
//    fprintf(stderr, "Button/Handle: Button Down at (%f,%f)\n", x, y);

  if(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == 1) {
    flags |= SG_WIDGET_FLAGS_PRESSED;
    current_sg->SetCurrentWidget(this);
    event->type = SDL_SG_EVENT;
    event->user.code = SG_EVENT_BUTTONPRESS;
    event->user.data1 = (void*)this;
    return 1;
    }
  else if(event->type == SDL_MOUSEBUTTONDOWN) {	// Eat other buttons
    return 0;
    }
  else if(event->type == SDL_MOUSEBUTTONUP) {
    current_sg->UnsetCurrentWidget();
    flags &= (~SG_WIDGET_FLAGS_PRESSED);
    event->type = SDL_SG_EVENT;
    event->user.code = SG_EVENT_BUTTONRELEASE;
    event->user.data1 = (void*)this;
    return 1;
    }  

  return 1;
  }

bool SG_Button::Render() {
//  fprintf(stderr, "Rendering Button %p!\n", this);

  if(flags & SG_WIDGET_FLAGS_HIDDEN) return 1;

  if(flags & SG_WIDGET_FLAGS_PRESSED) glColor3f(cl_r, cl_g, cl_b);
  else glColor3f(bg_r, bg_g, bg_b);

  glBegin(GL_QUADS);
  glVertex3f(-1.0, -1.0, 0.0);
  glVertex3f( 1.0, -1.0, 0.0);
  glVertex3f( 1.0,  1.0, 0.0);
  glVertex3f(-1.0,  1.0, 0.0);
  glEnd();

  return 1;
  }


//  bool SG_Button::SetDefaultCursor(GL_MODEL *cur);
  
//  static GL_MODEL SG_Button::Default_Mouse_Cursor = NULL;

