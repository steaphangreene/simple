#ifndef SG_PANEL_H
#define SG_PANEL_H

#include <GL/gl.h>

#include <vector>

using namespace std;

#include "sg_widget.h"

enum SG_TextureType {
  SG_TEXTURE_NONE = 0,
  SG_TEXTURE_COLOR,
  SG_TEXTURE_DEFINED
  };

struct SG_Texture {
  int type;
  GLuint texture;	//Current texture (when active)
  SDL_Surface *cur;	//Current texture buffer
  SDL_Surface *src;	//Only for SG_TEXTURE_DEFINED
  SDL_Color col;	//Only for SG_TEXTURE_COLOR
  SDL_Color fg;		//Only used by children (font color)
  float xfact, yfact;	//Portion of texture actually shown
  };

void BuildTexture(SG_Texture &tex);

class SG_Panel : public SG_Widget {
public:
  SG_Panel(float red, float green, float blue);
  virtual ~SG_Panel();
  virtual bool HandleMouseEvent(SDL_Event *event, float x, float y);
  virtual bool Render();
//  virtual bool SetDefaultCursor(GL_MODEL *cur);
  
protected:
//  static GL_MODEL Default_Mouse_Cursor;
  vector<SG_Texture> texture;
  int state; //Which texture to use - for multi-state support
  };

#endif // SG_PANEL_H

