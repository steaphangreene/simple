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

#include <GL/gl.h>

#include "sg_progressbar.h"
#include "sg_events.h"

SG_ProgressBar::SG_ProgressBar(string mes, float red, float green, float blue,
	float tred, float tgreen, float tblue,
	float dred, float dgreen, float dblue)
		: SG_TextArea(mes, red, green, blue, tred, tgreen, tblue) {
  texture.resize(2);
  texture[1] = texture[0];
  texture[1].col.r = (Uint8)(dred*255.0f);
  texture[1].col.g = (Uint8)(dgreen*255.0f);
  texture[1].col.b = (Uint8)(dblue*255.0f);
  BuildTexture(texture[1], message, 0.0, 0.0);
  }

SG_ProgressBar::~SG_ProgressBar() {
  }

//  bool SG_ProgressBar::SetDefaultCursor(GL_MODEL *cur);
  
//  static GL_MODEL SG_ProgressBar::Default_Mouse_Cursor = NULL;


