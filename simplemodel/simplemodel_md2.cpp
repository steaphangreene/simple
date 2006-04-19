// *************************************************************************
//  This file is part of the SimpleModel Example Module by Steaphan Greene
//
//  Copyright 2005-2006 Steaphan Greene <stea@cs.binghamton.edu>
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
#include "SDL_image.h"

#include <cmath>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>
using namespace std;

#include "simplemodel_md2.h"
#include "saferead.h"

const int anim_data[21][3] = {	// Each represents: start, end, fps
  {   0,  39,  9 },	// STAND
  {  40,  45, 10 },	// RUN
  {  46,  53, 10 },	// ATTACK
  {  54,  57,  7 },	// PAIN_A
  {  58,  61,  7 },	// PAIN_B
  {  62,  65,  7 },	// PAIN_C
  {  66,  71,  7 },	// JUMP
  {  72,  83,  7 },	// FLIP
  {  84,  94,  7 },	// SALUTE
  {  95, 111, 10 },	// FALLBACK
  { 112, 122,  7 },	// WAVE
  { 123, 134,  6 },	// POINT
  { 135, 153, 10 },	// CROUCH_STAND
  { 154, 159,  7 },	// CROUCH_WALK
  { 160, 168, 10 },	// CROUCH_ATTACK
  { 196, 172,  7 },	// CROUCH_PAIN
  { 173, 177,  5 },	// CROUCH_DEATH
  { 178, 183,  7 },	// DEATH_FALLBACK
  { 184, 189,  7 },	// DEATH_FALLFORWARD
  { 190, 197,  7 },	// DEATH_FALLBACKSLOW
  };

SimpleModel_MD2::SimpleModel_MD2(const string &filenm,
	const string &modelnm, const string &defskin) {
  Load(filenm, modelnm, defskin);
  }

SimpleModel_MD2::~SimpleModel_MD2() {
  }

bool SimpleModel_MD2::Load(const string &filenm,
	const string &modelnm, const string &defskin) {
  const Vector3 normals[162] = {
    { { -0.5257,  0.0000,  0.8507 } }, 
    { { -0.4429,  0.2389,  0.8642 } }, 
    { { -0.2952,  0.0000,  0.9554 } }, 
    { { -0.3090,  0.5000,  0.8090 } }, 
    { { -0.1625,  0.2629,  0.9511 } }, 
    { {  0.0000,  0.0000,  1.0000 } }, 
    { {  0.0000,  0.8507,  0.5257 } }, 
    { { -0.1476,  0.7166,  0.6817 } }, 
    { {  0.1476,  0.7166,  0.6817 } }, 
    { {  0.0000,  0.5257,  0.8507 } }, 
    { {  0.3090,  0.5000,  0.8090 } }, 
    { {  0.5257,  0.0000,  0.8507 } }, 
    { {  0.2952,  0.0000,  0.9554 } }, 
    { {  0.4429,  0.2389,  0.8642 } }, 
    { {  0.1625,  0.2629,  0.9511 } }, 
    { { -0.6817,  0.1476,  0.7166 } }, 
    { { -0.8090,  0.3090,  0.5000 } }, 
    { { -0.5878,  0.4253,  0.6882 } }, 
    { { -0.8507,  0.5257,  0.0000 } }, 
    { { -0.8642,  0.4429,  0.2389 } }, 
    { { -0.7166,  0.6817,  0.1476 } }, 
    { { -0.6882,  0.5878,  0.4253 } }, 
    { { -0.5000,  0.8090,  0.3090 } }, 
    { { -0.2389,  0.8642,  0.4429 } }, 
    { { -0.4253,  0.6882,  0.5878 } }, 
    { { -0.7166,  0.6817, -0.1476 } }, 
    { { -0.5000,  0.8090, -0.3090 } }, 
    { { -0.5257,  0.8507,  0.0000 } }, 
    { {  0.0000,  0.8507, -0.5257 } }, 
    { { -0.2389,  0.8642, -0.4429 } }, 
    { {  0.0000,  0.9554, -0.2952 } }, 
    { { -0.2629,  0.9511, -0.1625 } }, 
    { {  0.0000,  1.0000,  0.0000 } }, 
    { {  0.0000,  0.9554,  0.2952 } }, 
    { { -0.2629,  0.9511,  0.1625 } }, 
    { {  0.2389,  0.8642,  0.4429 } }, 
    { {  0.2629,  0.9511,  0.1625 } }, 
    { {  0.5000,  0.8090,  0.3090 } }, 
    { {  0.2389,  0.8642, -0.4429 } }, 
    { {  0.2629,  0.9511, -0.1625 } }, 
    { {  0.5000,  0.8090, -0.3090 } }, 
    { {  0.8507,  0.5257,  0.0000 } }, 
    { {  0.7166,  0.6817,  0.1476 } }, 
    { {  0.7166,  0.6817, -0.1476 } }, 
    { {  0.5257,  0.8507,  0.0000 } }, 
    { {  0.4253,  0.6882,  0.5878 } }, 
    { {  0.8642,  0.4429,  0.2389 } }, 
    { {  0.6882,  0.5878,  0.4253 } }, 
    { {  0.8090,  0.3090,  0.5000 } }, 
    { {  0.6817,  0.1476,  0.7166 } }, 
    { {  0.5878,  0.4253,  0.6882 } }, 
    { {  0.9554,  0.2952,  0.0000 } }, 
    { {  1.0000,  0.0000,  0.0000 } }, 
    { {  0.9511,  0.1625,  0.2629 } }, 
    { {  0.8507, -0.5257,  0.0000 } }, 
    { {  0.9554, -0.2952,  0.0000 } }, 
    { {  0.8642, -0.4429,  0.2389 } }, 
    { {  0.9511, -0.1625,  0.2629 } }, 
    { {  0.8090, -0.3090,  0.5000 } }, 
    { {  0.6817, -0.1476,  0.7166 } }, 
    { {  0.8507,  0.0000,  0.5257 } }, 
    { {  0.8642,  0.4429, -0.2389 } }, 
    { {  0.8090,  0.3090, -0.5000 } }, 
    { {  0.9511,  0.1625, -0.2629 } }, 
    { {  0.5257,  0.0000, -0.8507 } }, 
    { {  0.6817,  0.1476, -0.7166 } }, 
    { {  0.6817, -0.1476, -0.7166 } }, 
    { {  0.8507,  0.0000, -0.5257 } }, 
    { {  0.8090, -0.3090, -0.5000 } }, 
    { {  0.8642, -0.4429, -0.2389 } }, 
    { {  0.9511, -0.1625, -0.2629 } }, 
    { {  0.1476,  0.7166, -0.6817 } }, 
    { {  0.3090,  0.5000, -0.8090 } }, 
    { {  0.4253,  0.6882, -0.5878 } }, 
    { {  0.4429,  0.2389, -0.8642 } }, 
    { {  0.5878,  0.4253, -0.6882 } }, 
    { {  0.6882,  0.5878, -0.4253 } }, 
    { { -0.1476,  0.7166, -0.6817 } }, 
    { { -0.3090,  0.5000, -0.8090 } }, 
    { {  0.0000,  0.5257, -0.8507 } }, 
    { { -0.5257,  0.0000, -0.8507 } }, 
    { { -0.4429,  0.2389, -0.8642 } }, 
    { { -0.2952,  0.0000, -0.9554 } }, 
    { { -0.1625,  0.2629, -0.9511 } }, 
    { {  0.0000,  0.0000, -1.0000 } }, 
    { {  0.2952,  0.0000, -0.9554 } }, 
    { {  0.1625,  0.2629, -0.9511 } }, 
    { { -0.4429, -0.2389, -0.8642 } }, 
    { { -0.3090, -0.5000, -0.8090 } }, 
    { { -0.1625, -0.2629, -0.9511 } }, 
    { {  0.0000, -0.8507, -0.5257 } }, 
    { { -0.1476, -0.7166, -0.6817 } }, 
    { {  0.1476, -0.7166, -0.6817 } }, 
    { {  0.0000, -0.5257, -0.8507 } }, 
    { {  0.3090, -0.5000, -0.8090 } }, 
    { {  0.4429, -0.2389, -0.8642 } }, 
    { {  0.1625, -0.2629, -0.9511 } }, 
    { {  0.2389, -0.8642, -0.4429 } }, 
    { {  0.5000, -0.8090, -0.3090 } }, 
    { {  0.4253, -0.6882, -0.5878 } }, 
    { {  0.7166, -0.6817, -0.1476 } }, 
    { {  0.6882, -0.5878, -0.4253 } }, 
    { {  0.5878, -0.4253, -0.6882 } }, 
    { {  0.0000, -0.9554, -0.2952 } }, 
    { {  0.0000, -1.0000,  0.0000 } }, 
    { {  0.2629, -0.9511, -0.1625 } }, 
    { {  0.0000, -0.8507,  0.5257 } }, 
    { {  0.0000, -0.9554,  0.2952 } }, 
    { {  0.2389, -0.8642,  0.4429 } }, 
    { {  0.2629, -0.9511,  0.1625 } }, 
    { {  0.5000, -0.8090,  0.3090 } }, 
    { {  0.7166, -0.6817,  0.1476 } }, 
    { {  0.5257, -0.8507,  0.0000 } }, 
    { { -0.2389, -0.8642, -0.4429 } }, 
    { { -0.5000, -0.8090, -0.3090 } }, 
    { { -0.2629, -0.9511, -0.1625 } }, 
    { { -0.8507, -0.5257,  0.0000 } }, 
    { { -0.7166, -0.6817, -0.1476 } }, 
    { { -0.7166, -0.6817,  0.1476 } }, 
    { { -0.5257, -0.8507,  0.0000 } }, 
    { { -0.5000, -0.8090,  0.3090 } }, 
    { { -0.2389, -0.8642,  0.4429 } }, 
    { { -0.2629, -0.9511,  0.1625 } }, 
    { { -0.8642, -0.4429,  0.2389 } }, 
    { { -0.8090, -0.3090,  0.5000 } }, 
    { { -0.6882, -0.5878,  0.4253 } }, 
    { { -0.6817, -0.1476,  0.7166 } }, 
    { { -0.4429, -0.2389,  0.8642 } }, 
    { { -0.5878, -0.4253,  0.6882 } }, 
    { { -0.3090, -0.5000,  0.8090 } }, 
    { { -0.1476, -0.7166,  0.6817 } }, 
    { { -0.4253, -0.6882,  0.5878 } }, 
    { { -0.1625, -0.2629,  0.9511 } }, 
    { {  0.4429, -0.2389,  0.8642 } }, 
    { {  0.1625, -0.2629,  0.9511 } }, 
    { {  0.3090, -0.5000,  0.8090 } }, 
    { {  0.1476, -0.7166,  0.6817 } }, 
    { {  0.0000, -0.5257,  0.8507 } }, 
    { {  0.4253, -0.6882,  0.5878 } }, 
    { {  0.5878, -0.4253,  0.6882 } }, 
    { {  0.6882, -0.5878,  0.4253 } }, 
    { { -0.9554,  0.2952,  0.0000 } }, 
    { { -0.9511,  0.1625,  0.2629 } }, 
    { { -1.0000,  0.0000,  0.0000 } }, 
    { { -0.8507,  0.0000,  0.5257 } }, 
    { { -0.9554, -0.2952,  0.0000 } }, 
    { { -0.9511, -0.1625,  0.2629 } }, 
    { { -0.8642,  0.4429, -0.2389 } }, 
    { { -0.9511,  0.1625, -0.2629 } }, 
    { { -0.8090,  0.3090, -0.5000 } }, 
    { { -0.8642, -0.4429, -0.2389 } }, 
    { { -0.9511, -0.1625, -0.2629 } }, 
    { { -0.8090, -0.3090, -0.5000 } }, 
    { { -0.6817,  0.1476, -0.7166 } }, 
    { { -0.6817, -0.1476, -0.7166 } }, 
    { { -0.8507,  0.0000, -0.5257 } }, 
    { { -0.6882,  0.5878, -0.4253 } }, 
    { { -0.5878,  0.4253, -0.6882 } }, 
    { { -0.4253,  0.6882, -0.5878 } }, 
    { { -0.4253, -0.6882, -0.5878 } }, 
    { { -0.5878, -0.4253, -0.6882 } }, 
    { { -0.6882, -0.5878, -0.4253 } }, 
    };

  filename = filenm;
  modelname = modelnm;
  if(modelnm.substr(0, filenm.length()) != filenm) {
    //New format, no duplicated lines in file.
    modelname = filename + "/" + modelnm;
    }
  else {
    //Old format, duplicated lines in file -OR- zero-length filenm.
    filename = ".";
    }

  SDL_RWops *model = SDL_RWFromZZIP(modelname.c_str(), "rb");
  if(!model) {
    fprintf(stderr, "WARNING: Unable to open model file '%s'!\n",
	modelname.c_str());
    perror("WARNING");
    return false;
    }

  //Read parts of the header of the MD2 file
  Sint8 magic[4];
  SDL_RWread(model, &magic, 1, sizeof(magic));
  if(strncmp((char *)magic, "IDP2", 4)) {
    fprintf(stderr, "WARNING: File '%s' is not an MD2 file!\n",
	modelnm.c_str());
    SDL_RWclose(model);
    return false;
    }

  Sint32 filever;
  freadLE(filever, model);
  if(filever != 8) {
    fprintf(stderr, "WARNING: File '%s' is not an v8 MD2 file!\n",
	modelnm.c_str());
    SDL_RWclose(model);
    return false;
    }

  Uint32 tex_width;
  freadLE(tex_width, model);

  Uint32 tex_height;
  freadLE(tex_height, model);

  SDL_RWseek(model, 4, SEEK_CUR); // Skip the framesize

  Uint32 num_skins;
  freadLE(num_skins, model);

  Uint32 num_vertices;
  freadLE(num_vertices, model);

  Uint32 num_texcoord;
  freadLE(num_texcoord, model);

  SDL_RWseek(model, 4, SEEK_CUR); // Skip the number of triangles

  Uint32 num_glcomms;
  freadLE(num_glcomms, model);

  Uint32 num_frames;
  freadLE(num_frames, model);

  Sint32 skins_offset;
  freadLE(skins_offset, model);

  Sint32 texcoords_offset;
  freadLE(texcoords_offset, model);

  SDL_RWseek(model, 4, SEEK_CUR); // Skip the offset of the triangles

  Sint32 frames_offset;
  freadLE(frames_offset, model);

  Sint32 glcomm_offset;
  freadLE(glcomm_offset, model);

//  bool unset=1;
//  float mins[3];
//  float maxs[3];

  //Read the Frames
  verts.resize(num_frames);
  norms.resize(num_frames);
  SDL_RWseek(model, frames_offset, SEEK_SET);
  for(Uint32 frame=0; frame < num_frames; ++frame) {
    float scale[3], translate[3];
    freadLE(scale[0], model);
    freadLE(scale[1], model);
    freadLE(scale[2], model);
    freadLE(translate[0], model);
    freadLE(translate[1], model);
    freadLE(translate[2], model);

    translate[2] += 24.0;

    scale[0] /= 16.0;
    scale[1] /= 16.0;
    scale[2] /= 16.0;
    translate[0] /= 16.0;
    translate[1] /= 16.0;
    translate[2] /= 16.0;

    Sint8 name[20] = {0};
    SDL_RWread(model, (char*)name, 1, 16);
    verts[frame].resize(num_vertices);
    norms[frame].resize(num_vertices);
    for(Uint32 vert=0; vert < num_vertices; ++vert) {
      Uint8 vert_info[4];
      freadLE(vert_info[0], model);
      freadLE(vert_info[1], model);
      freadLE(vert_info[2], model);
      freadLE(vert_info[3], model);

      verts[frame][vert].data[0] = (scale[0] * vert_info[0]) + translate[0];
      verts[frame][vert].data[1] = (scale[1] * vert_info[1]) + translate[1];
      verts[frame][vert].data[2] = (scale[2] * vert_info[2]) + translate[2];

      norms[frame][vert].data[0] = normals[vert_info[3]].data[0];
      norms[frame][vert].data[1] = normals[vert_info[3]].data[1];
      norms[frame][vert].data[2] = normals[vert_info[3]].data[2];

//      if(unset) {
//	unset = false;
//	mins[0] = verts[frame][vert].data[0];
//	mins[1] = verts[frame][vert].data[1];
//	mins[2] = verts[frame][vert].data[2];
//	maxs[0] = verts[frame][vert].data[0];
//	maxs[1] = verts[frame][vert].data[1];
//	maxs[2] = verts[frame][vert].data[2];
//	}
//      else {
//	if(verts[frame][vert].data[0] < mins[0]) mins[0] = verts[frame][vert].data[0];
//	if(verts[frame][vert].data[1] < mins[1]) mins[1] = verts[frame][vert].data[1];
//	if(verts[frame][vert].data[2] < mins[2]) mins[2] = verts[frame][vert].data[2];
//	if(verts[frame][vert].data[0] > maxs[0]) maxs[0] = verts[frame][vert].data[0];
//	if(verts[frame][vert].data[1] > maxs[1]) maxs[1] = verts[frame][vert].data[1];
//	if(verts[frame][vert].data[2] > maxs[2]) maxs[2] = verts[frame][vert].data[2];
//	}
      }
    }

//  fprintf(stderr, "(%.2f-%.2f, %.2f-%.2f, %.2f-%.2f)\n",
//	mins[0], maxs[0], mins[1], maxs[1], mins[2], maxs[2]);
//

  if(defskin.length() > 0) {
    AddSkin(filenm + "/" + defskin);
    }

  SDL_RWseek(model, skins_offset, SEEK_SET);
  for(Uint32 skin=0; skin < num_skins; ++skin) {
    Sint8 skinnm[68] = {0};
    SDL_RWread(model, skinnm, 1, 64);
    AddSkin(filenm + "/" + (char*)skinnm);
    }

  glcomms.resize(num_glcomms);
  SDL_RWseek(model, glcomm_offset, SEEK_SET);
  for(Uint32 glcomm=0; glcomm < num_glcomms; ++glcomm) {
    Sint32 comstat;
    freadLE(comstat, model);
    if(comstat == 0) break;	//Done.
    if(comstat < 0) {
      glcomms[glcomm].strip = false;
      comstat = -comstat;
      }
    else {
      glcomms[glcomm].strip = true;
      }
    glcomms[glcomm].verts.resize(comstat);
    for(Sint32 glind=0; glind < comstat; ++glind) {
      freadLE(glcomms[glcomm].verts[glind].tex_x, model);
      freadLE(glcomms[glcomm].verts[glind].tex_y, model);
      freadLE(glcomms[glcomm].verts[glind].vindex, model);
      }
    }

  animations["STAND"] = 0;
  animations["RUN"] = 1;
  animations["ATTACK"] = 2;
  animations["PAIN_A"] = 3;
  animations["PAIN_B"] = 4;
  animations["PAIN_C"] = 5;
  animations["JUMP"] = 6;
  animations["FLIP"] = 7;
  animations["SALUTE"] = 8;
  animations["FALLBACK"] = 9;
  animations["WAVE"] = 10;
  animations["POINT"] = 11;
  animations["CROUCH_STAND"] = 12;
  animations["CROUCH_WALK"] = 13;
  animations["CROUCH_ATTACK"] = 14;
  animations["CROUCH_PAIN"] = 15;
  animations["CROUCH_DEATH"] = 16;
  animations["DEATH_FALLBACK"] = 17;
  animations["DEATH_FALLFORWARD"] = 18;
  animations["DEATH_FALLBACKSLOW"] = 19;

  return false;
  }

bool SimpleModel_MD2::RenderSelf(Uint32 cur_time, const vector<int> &anim,
	const vector<Uint32> &start_time, Uint32 anim_offset) const {
  glCullFace(GL_FRONT);	//MD2 models use front face culling

  if(texture.size() > 0) glBindTexture(GL_TEXTURE_2D, texture[0]->GLTexture());

  vector<GLCommand>::const_iterator glcomm = glcomms.begin();
  for(; glcomm != glcomms.end(); ++glcomm) {
    if(glcomm->strip) glBegin(GL_TRIANGLE_STRIP);
    else glBegin(GL_TRIANGLE_FAN);

    float fac = 0.0;
    Uint32 frame, next;
    frame = CalcBaseFrame(cur_time, anim[anim_offset], start_time[anim_offset], fac);
    frame = NormalizeFrame(anim[anim_offset], frame);
    next = NormalizeFrame(anim[anim_offset], frame + 1);

    float sx=1.0, sy=1.0;
    if(texture.size() > 0) {
      sx = texture[0]->xfact;
      sy = texture[0]->yfact;
      }

    Vector3 vert, norm;

    vector<GLVertex>::const_iterator glvert = glcomm->verts.begin();
    for(; glvert != glcomm->verts.end(); ++glvert) {
      if(texture.size() > 0)
	glTexCoord2f(glvert->tex_x * sx, glvert->tex_y * sy);
      LERP(norm, norms[frame][glvert->vindex], norms[next][glvert->vindex], fac);
      LERP(vert, verts[frame][glvert->vindex], verts[next][glvert->vindex], fac);
      glNormal3fv(norm.data);
      glVertex3fv(vert.data);
      }

    glEnd();
    }

  glCullFace(GL_BACK);

  return true;
  }

bool SimpleModel_MD2::MoveToTag(Uint32 tag, Uint32 cur_time,
	const vector<int> &anim, const vector<Uint32> &start_time,
	Uint32 anim_offset) const {
  if(tag >= tags.size()) return false;

  return true;
  }

int SimpleModel_MD2::CalcBaseFrame(Uint32 cur_time, int anim, Uint32 start_time,
	float &offset) const {
  int frame = anim_data[anim][0];
  offset = (float(cur_time - start_time) / 1000.0) * float(anim_data[anim][2]);
  frame += int(offset);
  offset -= truncf(offset);
  return frame;
  }

int SimpleModel_MD2::NormalizeFrame(int anim, int frame) const {
  if(frame < anim_data[anim][0]) frame = anim_data[anim][0];
  else if(frame > anim_data[anim][1]) frame = anim_data[anim][1];
  return frame;
  }

const vector<string> &SimpleModel_MD2::GetSkinList() {
  return skins;
  }

void SimpleModel_MD2::AddSkin(const string &skinnm) {
  SimpleTexture *skin = new SimpleTexture(skinnm.c_str());
  if(skin && (skin->type != SIMPLETEXTURE_NONE)) {
    skin->GLTexture();
    texture.push_back(skin);
    skins.push_back(skinnm);
    }
  else if(skin) {
    delete skin;
    }
  }
