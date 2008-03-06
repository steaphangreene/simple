// *************************************************************************
//  This file is part of the SimpleModel Example Module by Steaphan Greene
//
//  Copyright 2005-2008 Steaphan Greene <stea@cs.binghamton.edu>
//
//  SimpleModel is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  SimpleModel is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SimpleModel (see the file named "COPYING");
//  if not, write to the the Free Software Foundation, Inc.,
//  59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
// *************************************************************************

#include "SDL.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
using namespace std;

#include "simplemodel_obj.h"
#include "saferead.h"

SimpleModel_OBJ::SimpleModel_OBJ(const string &filenm, const string &texnm) {
  Load(filenm, texnm);
  }

SimpleModel_OBJ::SimpleModel_OBJ() {
  }

SimpleModel_OBJ::~SimpleModel_OBJ() {
  }

bool SimpleModel_OBJ::Load(const string &filenm, const string &texnm) {
  SDL_RWops *model = SDL_RWFromFile(filenm.c_str(), "r");
  int size = SDL_RWseek(model, 0, SEEK_END);
  SDL_RWseek(model, 0, SEEK_SET);
  char buf[size+1], *ptr;
  SDL_RWread(model, buf, size, 1);
  SDL_RWclose(model);
  buf[size] = 0;

  vector<Vector3> points;
  vector<Vector3> norms;
  vector<Vector2> coords;

  ptr = buf;
  while((ptr - buf) < size) {
    if(!strncmp(ptr, "v ", 2)) {
      float x, y, z;
      sscanf(ptr+2, "%f %f %f", &x, &y, &z);
      Vector3 vec = { { x, y, z } };
      points.push_back(vec);
      }
    if(!strncmp(ptr, "vn ", 3)) {
      float x, y, z;
      sscanf(ptr+2, "%f %f %f", &x, &y, &z);
      Vector3 vec = { { x, y, z } };
      norms.push_back(vec);
      }
    if(!strncmp(ptr, "vt ", 3)) {
      float x, y;
      sscanf(ptr+2, "%f %f", &x, &y);
      Vector2 vec = { { x, y } };
      coords.push_back(vec);
      }
    while((*ptr) != '\n' && (ptr - buf) < size) { ++ptr; }
    if((ptr - buf) < size) { ++ptr; }
    }

  ptr = buf;
  display_list = glGenLists(1);
  glNewList(display_list, GL_COMPILE);
  int begun = 0;
  while((ptr - buf) < size) {
    if(!strncmp(ptr, "f ", 2)) {
      int p[4] = { 0, 0, 0, 0 };
      int t[4] = { 0, 0, 0, 0 };
      int n[4] = { 0, 0, 0, 0 };
      int res;
      res = sscanf(ptr+2, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
		p+0, t+0, n+0, p+1, t+1, n+1, p+2, t+2, n+2, p+3, t+3, n+3);
      if(res < 9) {
	res = sscanf(ptr+2, "%d//%d %d//%d %d//%d %d//%d",
		p+0, n+0, p+1, n+1, p+2, n+2, p+3, n+3);
	}
      if(res < 6) {
	res = sscanf(ptr+2, "%d/%d/ %d/%d/ %d/%d/ %d/%d/",
		p+0, t+0, p+1, t+1, p+2, t+2, p+3, t+3);
	}
      if(res < 6) {
	res = sscanf(ptr+2, "%d// %d// %d// %d//", p+0, p+1, p+2, p+3);
	}
      if(res < 3) {
	res = sscanf(ptr+2, "%d %d %d %d", p+0, p+1, p+2, p+3);
	}
      if(res < 3) {
	fprintf(stderr, "Warning: Failed to load '%s'\n", filenm.c_str());
	return false;
	}
      if(!begun) {
	if(p[3] > 0) glBegin(GL_QUADS);
	else glBegin(GL_TRIANGLES);
	begun = 1;
	}
      for(int vert = 0; vert < 4; ++vert) {
	if(p[vert] > 0) {
	  if(n[vert] > 0) glNormal3fv(norms[n[vert]-1].data);
	  if(t[vert] > 0) glTexCoord2fv(coords[t[vert]-1].data);
	  glVertex3fv(points[p[vert]-1].data);
	  }
	}
      }
    while((*ptr) != '\n' && (ptr - buf) < size) { ++ptr; }
    if((ptr - buf) < size) { ++ptr; }
    }
  glEnd();
  glEndList();

  return true;
  }

bool SimpleModel_OBJ::RenderSelf(Uint32 cur_time, const vector<int> &anim,
	const vector<Uint32> &start_time, Uint32 anim_offset) const {
  glCallList(display_list);
  return true;
  }