// *************************************************************************
//  This file is part of the SimpleGUI Example Module by Steaphan Greene
//
//  Copyright 2005 Steaphan Greene <stea@cs.binghamton.edu>
//
//  SimpleGUI is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  SimpleGUI is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SimpleGUI (see the file named "COPYING");
//  if not, write to the the Free Software Foundation, Inc.,
//  59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
// *************************************************************************

#ifndef SG_PANEL_H
#define SG_PANEL_H

#include <GL/gl.h>

#include <vector>

using namespace std;

#include "sg_colors.h"
#include "sg_widget.h"

enum SG_TextureType {
  SG_TEXTURE_NONE = 0,
  SG_TEXTURE_COLOR,
  SG_TEXTURE_DEFINED
  };

struct SG_Texture {
  SG_TextureType type;
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
  SG_Panel(int c = SG_COL_BG);
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
