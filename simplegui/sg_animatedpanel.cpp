// *************************************************************************
//  This file is part of the SimpleGUI Example Module by Steaphan Greene
//
//  Copyright 2005-2014 Steaphan Greene <steaphan@gmail.com>
//
//  SimpleGUI is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 3 of the License, or
//  (at your option) any later version.
//
//  SimpleGUI is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SimpleGUI (see the file named "COPYING");
//  If not, see <http://www.gnu.org/licenses/>.
//
// *************************************************************************

#include "SDL_opengl.h"

#include "sg_globals.h"
#include "sg_animatedpanel.h"

#include "SDL_ttf.h"

SG_AnimatedPanel::SG_AnimatedPanel(const vector<SimpleTexture> &txtrs, float mspf)
	: SG_Panel(txtrs[0]) {
  for(int tx=1; tx < int(txtrs.size()); ++tx) {
    texture.push_back(txtrs[tx]);
    }
  speed = mspf;
  }

SG_AnimatedPanel::~SG_AnimatedPanel() {
  }

bool SG_AnimatedPanel::RenderSelf(unsigned long cur_time) {
  state = int((float)(cur_time) / speed + 0.5) % texture.size();
  return SG_Panel::RenderSelf(cur_time);
  }
//  bool SG_AnimatedPanel::SetDefaultCursor(GL_MODEL *cur);

//  static GL_MODEL SG_AnimatedPanel::Default_Mouse_Cursor = NULL;
