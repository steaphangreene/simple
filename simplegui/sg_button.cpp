#include <GL/gl.h>

#include "sg_button.h"
#include "sg_events.h"
#include "sg_globals.h"

SG_Button::SG_Button(string mes, float red, float green, float blue,
	float tred, float tgreen, float tblue,
	float cred, float cgreen, float cblue)
		: SG_TextArea(mes, red, green, blue, tred, tgreen, tblue) {
  texture.resize(3);
  texture[1] = texture[0];
  texture[1].col.r = (Uint8)(cred*255.0f);
  texture[1].col.g = (Uint8)(cgreen*255.0f);
  texture[1].col.b = (Uint8)(cblue*255.0f);
  BuildTexture(texture[1], message, 0.0, 0.0);
  }

SG_Button::~SG_Button() {
  }

bool SG_Button::HandleMouseEvent(SDL_Event *event, float x, float y) {
//  if(event->type == SDL_MOUSEBUTTONDOWN)
//    fprintf(stderr, "Button/Handle: Button Down at (%f,%f)\n", x, y);

  if(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == 1) {
    flags |= SG_WIDGET_FLAGS_PRESSED;
    state = 1;
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
    state = 0;
    event->type = SDL_SG_EVENT;
    event->user.code = SG_EVENT_BUTTONRELEASE;
    event->user.data1 = (void*)this;
    return 1;
    }  

  return 1;
  }

//  bool SG_Button::SetDefaultCursor(GL_MODEL *cur);
  
//  static GL_MODEL SG_Button::Default_Mouse_Cursor = NULL;

