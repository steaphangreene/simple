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

#ifndef SG_TRANSLABEL_H
#define SG_TRANSLABEL_H

#include <GL/gl.h>

#include <string>

using namespace std;

#include "sg_widget.h"

class SG_TransLabel : public SG_Widget {
public:
  SG_TransLabel(string mes, float red, float green, float blue);
  virtual ~SG_TransLabel();
  virtual bool HandleMouseEvent(SDL_Event *event, float x, float y);
  virtual bool Render();
//  virtual bool SetDefaultCursor(GL_MODEL *cur);
  void SetMargins(float xmar, float ymar);
  
protected:
//  static GL_MODEL Default_Mouse_Cursor;
  void BuildTransTexture();
  string message;
  GLuint texture;	//Current texture (when active)
  SDL_Surface *cur;	//Current texture buffer
  SDL_Color fg;
  float xfact, yfact;
  float xmargin, ymargin;
  };

#endif // SG_TRANSLABEL_H
