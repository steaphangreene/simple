// *************************************************************************
//  This file is part of the SimpleModel Example Module by Steaphan Greene
//
//  Copyright 2005-2015 Steaphan Greene <steaphan@gmail.com>
//
//  SimpleModel is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 3 of the License, or
//  (at your option) any later version.
//
//  SimpleModel is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SimpleModel (see the file named "COPYING");
//  If not, see <http://www.gnu.org/licenses/>.
//
// *************************************************************************

#include "SDL.h"

#include <cstdio>
#include <cstdlib>
using namespace std;

#include "simplemodel_cube.h"

SimpleModel_Cube::SimpleModel_Cube(const string &filenm) {}

SimpleModel_Cube::SimpleModel_Cube() {}

SimpleModel_Cube::~SimpleModel_Cube() {}

bool SimpleModel_Cube::Load(const string &filenm) { return true; }

bool SimpleModel_Cube::RenderSelf(Uint32 cur_time, const vector<int> &anim,
                                  const vector<Uint32> &start_time,
                                  Uint32 anim_offset) const {
  glBegin(GL_QUADS);

  glTexCoord2f(0.0, 0.0);
  glVertex3f(-1.0, 1.0, -1.0);
  glTexCoord2f(0.0, 0.5);
  glVertex3f(-1.0, 1.0, 1.0);
  glTexCoord2f(0.25, 0.5);
  glVertex3f(1.0, 1.0, 1.0);
  glTexCoord2f(0.25, 0.0);
  glVertex3f(1.0, 1.0, -1.0);

  glTexCoord2f(0.50, 0.0);
  glVertex3f(-1.0, -1.0, -1.0);
  glTexCoord2f(0.25, 0.0);
  glVertex3f(1.0, -1.0, -1.0);
  glTexCoord2f(0.25, 0.5);
  glVertex3f(1.0, -1.0, 1.0);
  glTexCoord2f(0.50, 0.5);
  glVertex3f(-1.0, -1.0, 1.0);

  glTexCoord2f(0.50, 0.5);
  glVertex3f(-1.0, -1.0, 1.0);
  glTexCoord2f(0.75, 0.5);
  glVertex3f(-1.0, 1.0, 1.0);
  glTexCoord2f(0.75, 0.0);
  glVertex3f(-1.0, 1.0, -1.0);
  glTexCoord2f(0.50, 0.0);
  glVertex3f(-1.0, -1.0, -1.0);

  glTexCoord2f(0.75, 0.5);
  glVertex3f(1.0, -1.0, 1.0);
  glTexCoord2f(0.75, 0.0);
  glVertex3f(1.0, -1.0, -1.0);
  glTexCoord2f(1.0, 0.0);
  glVertex3f(1.0, 1.0, -1.0);
  glTexCoord2f(1.0, 0.5);
  glVertex3f(1.0, 1.0, 1.0);

  glTexCoord2f(0.0, 0.5);
  glVertex3f(-1.0, -1.0, -1.0);
  glTexCoord2f(0.0, 1.0);
  glVertex3f(-1.0, 1.0, -1.0);
  glTexCoord2f(0.5, 1.0);
  glVertex3f(1.0, 1.0, -1.0);
  glTexCoord2f(0.5, 0.5);
  glVertex3f(1.0, -1.0, -1.0);

  glTexCoord2f(0.5, 0.5);
  glVertex3f(-1.0, -1.0, 1.0);
  glTexCoord2f(1.0, 0.5);
  glVertex3f(1.0, -1.0, 1.0);
  glTexCoord2f(1.0, 1.0);
  glVertex3f(1.0, 1.0, 1.0);
  glTexCoord2f(0.5, 1.0);
  glVertex3f(-1.0, 1.0, 1.0);

  glEnd();

  return true;
}
