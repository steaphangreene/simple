#include "sg_alignment.h"

SG_Widget::SG_Widget() {
  flags = 0;
  }

SG_Widget::~SG_Widget() {
  }

bool SG_Widget::HandleMouseEvent(SDL_Event *event, float x, float y) {
  return 1;
  }

bool SG_Widget::HandMouseEventTo(SG_Widget *targ, SDL_Event *event,
		float x, float y) {
  if(targ == this) return HandleMouseEvent(event, x, y);
  return 1;
  }

bool SG_Widget::HasWidget(SG_Widget *targ) {
  return this == targ;
  }

bool SG_Widget::Render() {
  return 1;
  }


//  bool SG_Widget::SetDefaultCursor(GL_MODEL *cur);
//  bool SG_Widget::SetCursor(GL_MODEL *cur)
//  bool SG_Widget::UnsetCursor()
  
//  static GL_MODEL SG_Widget::Default_Mouse_Cursor = NULL;

