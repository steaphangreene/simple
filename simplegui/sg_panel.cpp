#include <GL/gl.h>

#include "sg_panel.h"

SG_Panel::SG_Panel(float red, float green, float blue) : SG_Widget() {
  bg_r = red, bg_g = green, bg_b = blue;
  }

SG_Panel::~SG_Panel() {
  }

bool SG_Panel::HandleMouseEvent(SDL_Event *event, float x, float y) {
//  if(event->type == SDL_MOUSEBUTTONDOWN)
//    fprintf(stderr, "Panel/Handle: Button Down at (%f,%f)\n", x, y);

  return 0;	// This widget eats all mouse events
  }

bool SG_Panel::Render() {
//  fprintf(stderr, "Rendering Panel %p!\n", this);

  if(flags & SG_WIDGET_FLAGS_HIDDEN) return 1;

  glColor3f(bg_r, bg_g, bg_b);
  glBegin(GL_QUADS);
  glVertex3f(-1.0, -1.0, 0.0);
  glVertex3f( 1.0, -1.0, 0.0);
  glVertex3f( 1.0,  1.0, 0.0);
  glVertex3f(-1.0,  1.0, 0.0);
  glEnd();

  return 1;
  }


//  bool SG_Panel::SetDefaultCursor(GL_MODEL *cur);
  
//  static GL_MODEL SG_Panel::Default_Mouse_Cursor = NULL;

