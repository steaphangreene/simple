#ifndef SG_WIDGET_H
#define SG_WIDGET_H

#include <SDL/SDL.h>

#define SG_WIDGET_FLAGS_HIDDEN		1
#define SG_WIDGET_FLAGS_DISABLED	2
#define SG_WIDGET_FLAGS_PRESSED		4

class SG_Widget {
public:
  SG_Widget();
  virtual ~SG_Widget();
  virtual bool HandleMouseEvent(SDL_Event *event, float x, float y);
  virtual bool HandMouseEventTo(SG_Widget *targ, SDL_Event *event,
		float x, float y);
  virtual bool HasWidget(SG_Widget *targ);
  virtual bool Render();
  void Hide() { flags |= SG_WIDGET_FLAGS_HIDDEN; };
  void Show() { flags &= (~SG_WIDGET_FLAGS_HIDDEN); };
  void Disable() { flags |= SG_WIDGET_FLAGS_DISABLED; };
  void Enable() { flags &= (~SG_WIDGET_FLAGS_DISABLED); };
//  virtual bool SetDefaultCursor(GL_MODEL *cur);
//  bool SetCursor(GL_MODEL *cur)
//  bool UnsetCursor()
  
protected:
//  GL_MODEL Mouse_Cursor;
//  static GL_MODEL Default_Mouse_Cursor;
  unsigned long flags;
  };

#endif // SG_WIDGET_H

