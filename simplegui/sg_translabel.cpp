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

#include "SDL_opengl.h"

#include "sg_globals.h"
#include "sg_translabel.h"

#include "SDL_ttf.h"

SG_TransLabel::SG_TransLabel(string mes, int col, float mx, float my)
	: SG_TextArea(mes,
	(col == -1) ? current_sg->NewColor(0.0, 0.0, 0.0) : col,
	(col == -1) ? current_sg->NewColor(0.0, 0.0, 0.0) : col,
	mx, my) {
  SetTransparent(true);
  Ignore();
  }

SG_TransLabel::~SG_TransLabel() {
  }

//  bool SG_TransLabel::SetDefaultCursor(GL_MODEL *cur);
  
//  static GL_MODEL SG_TransLabel::Default_Mouse_Cursor = NULL;
