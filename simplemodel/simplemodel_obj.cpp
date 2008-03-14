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
  if(SimpleModel::glGenBuffersARB == NULL) {
    glDeleteLists(vertices, 1);
    }
  else {
    SimpleModel::glDeleteBuffersARB(1, &vertices);
    }
  }

bool SimpleModel_OBJ::Load(const string &filenm, const string &texnm) {
  filename = filenm;
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
  if(SimpleModel::glGenBuffersARB == NULL) {
    vertices = glGenLists(1);
    glNewList(vertices, GL_COMPILE);
    }
  vector<GLfloat> buffer;	// For VBO Implementation
  int begun = 0;
  format = GL_V3F;
  num_verts = 0;
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
	if(SimpleModel::glGenBuffersARB == NULL) {
	  if(p[3] > 0) glBegin(GL_QUADS);
	  else glBegin(GL_TRIANGLES);
	  }
	begun = 1;
	}
      for(int vert = 0; vert < 4; ++vert) {
	if(p[vert] > 0) {
	  if(SimpleModel::glGenBuffersARB == NULL) {
	    if(t[vert] > 0) glTexCoord2fv(coords[t[vert]-1].data);
	    if(n[vert] > 0) glNormal3fv(norms[n[vert]-1].data);
	    glVertex3fv(points[p[vert]-1].data);
	    }
	  else {
	    if(t[vert] > 0) {
	      buffer.push_back(coords[t[vert]-1].data[0]);
	      buffer.push_back(coords[t[vert]-1].data[1]);
	      format = GL_T2F_V3F;
	      }
	    if(n[vert] > 0) {
	      buffer.push_back(norms[n[vert]-1].data[0]);
	      buffer.push_back(norms[n[vert]-1].data[1]);
	      buffer.push_back(norms[n[vert]-1].data[2]);
	      if(format == GL_V3F) { format = GL_N3F_V3F; }
	      else { format = GL_T2F_N3F_V3F; }
	      }
	    buffer.push_back(points[p[vert]-1].data[0]);
	    buffer.push_back(points[p[vert]-1].data[1]);
	    buffer.push_back(points[p[vert]-1].data[2]);
	    }
	  ++num_verts;
	  }
	}
      }
    while((*ptr) != '\n' && (ptr - buf) < size) { ++ptr; }
    if((ptr - buf) < size) { ++ptr; }
    }
  if(SimpleModel::glGenBuffersARB == NULL) {
    glEnd();
    glEndList();
    }
  else {
    SimpleModel::glGenBuffersARB(1, &vertices);
    SimpleModel::glBindBufferARB(GL_ARRAY_BUFFER, vertices);
    SimpleModel::glBufferDataARB(GL_ARRAY_BUFFER,
	sizeof(GLfloat) * buffer.size(), &(buffer[0]), GL_STATIC_DRAW_ARB);
    SimpleModel::glBindBufferARB(GL_ARRAY_BUFFER, 0);
    }
  valid_models.insert(this);

  return true;
  }

bool SimpleModel_OBJ::RenderSelf(Uint32 cur_time, const vector<int> &anim,
	const vector<Uint32> &start_time, Uint32 anim_offset) const {

  if(SimpleModel::glGenBuffersARB == NULL) {
    if(!glIsList(vertices)) {
      ReloadValidModels();
      //fprintf(stderr, "Warning: Had to reload models %d!\n", glGetError());
      return false;
      }
    glCallList(vertices);
    }
  else {
    if(!SimpleModel::glIsBufferARB(vertices)) {
      ReloadValidModels();
      //fprintf(stderr, "Warning: Had to reload models %d!\n", glGetError());
      return false;
      }
    SimpleModel::glBindBufferARB(GL_ARRAY_BUFFER, vertices);
    glInterleavedArrays(format, 0, NULL);
    glDrawArrays(GL_QUADS, 0, num_verts);
    SimpleModel::glBindBufferARB(GL_ARRAY_BUFFER, 0);
    }
  return true;
  }

set<SimpleModel_OBJ *> SimpleModel_OBJ::valid_models;

void SimpleModel_OBJ::ReloadValidModels() {
  set<SimpleModel_OBJ*>::iterator mod = valid_models.begin();
  for(; mod != valid_models.end(); ++mod) {
    (*mod)->Load((*mod)->filename);
    }
  }
