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

#include "sg_editable.h"
#include "sg_globals.h"
#include "sg_events.h"

SG_Editable::SG_Editable(string mes, int c, int tc, int dc, int fc)
		: SG_TextArea(mes, c, tc) {
  texture.resize(3);
  texture[1] = texture[0];
  texture[1].col = *(current_sg->Color(dc));
  BuildTexture(texture[1], message, xmargin, ymargin);

  texture[2] = texture[0];
  texture[2].col = *(current_sg->Color(fc));
  BuildTexture(texture[2], message, xmargin, ymargin);
  }

SG_Editable::~SG_Editable() {
  }

//  bool SG_Editable::SetDefaultCursor(GL_MODEL *cur);
  
//  static GL_MODEL SG_Editable::Default_Mouse_Cursor = NULL;


