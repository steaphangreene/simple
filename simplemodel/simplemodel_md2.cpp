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

SimpleModel_MD2::SimpleModel_MD2(const string &filenm,
	const string &modelnm, const string &defskin) {
  Load(filenm, modelnm, defskin);
  }

SimpleModel_MD2::~SimpleModel_MD2() {
  }

bool SimpleModel_MD2::Load(const string &filenm,
	const string &modelnm, const string &defskin) {
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

  Uint32 num_triangles;
  freadLE(num_triangles, model);

  Uint32 num_glcomms;
  freadLE(num_glcomms, model);

  Uint32 num_frames;
  freadLE(num_frames, model);

  Sint32 skins_offset;
  freadLE(skins_offset, model);

  Sint32 texcoords_offset;
  freadLE(texcoords_offset, model);

  Sint32 triangles_offset;
  freadLE(triangles_offset, model);

  Sint32 frames_offset;
  freadLE(frames_offset, model);

  Sint32 glcomm_offset;
  freadLE(glcomm_offset, model);

  bool unset=1;
  float mins[3];
  float maxs[3];

  //Read the Frames
  verts.resize(num_frames);
  SDL_RWseek(model, frames_offset, SEEK_SET);
  for(Uint32 frame=0; frame < num_frames; ++frame) {
    float scale[3], translate[3];
    freadLE(scale[0], model);
    freadLE(scale[1], model);
    freadLE(scale[2], model);
    freadLE(translate[0], model);
    freadLE(translate[1], model);
    freadLE(translate[2], model);

    scale[0] /= 32.0;
    scale[1] /= 32.0;
    scale[2] /= 32.0;
    translate[0] /= 32.0;
    translate[1] /= 32.0;
    translate[2] /= 32.0;

    Sint8 name[20] = {0};
    SDL_RWread(model, (char*)name, 1, 16);
    verts[frame].resize(num_vertices);
    for(Uint32 vert=0; vert < num_vertices; ++vert) {
      Uint8 vert_info[4];
      freadLE(vert_info[0], model);
      freadLE(vert_info[1], model);
      freadLE(vert_info[2], model);
      freadLE(vert_info[3], model);	//FIXME: Use this normal vec index!

      verts[frame][vert].data[0] = (scale[0] * vert_info[0]) + translate[0];
      verts[frame][vert].data[1] = (scale[1] * vert_info[1]) + translate[1];
      verts[frame][vert].data[2] = (scale[2] * vert_info[2]) + translate[2];

      if(unset) {
	unset = false;
	mins[0] = verts[frame][vert].data[0];
	mins[1] = verts[frame][vert].data[1];
	mins[2] = verts[frame][vert].data[2];
	maxs[0] = verts[frame][vert].data[0];
	maxs[1] = verts[frame][vert].data[1];
	maxs[2] = verts[frame][vert].data[2];
	}
      else {
	if(verts[frame][vert].data[0] < mins[0]) mins[0] = verts[frame][vert].data[0];
	if(verts[frame][vert].data[1] < mins[1]) mins[1] = verts[frame][vert].data[1];
	if(verts[frame][vert].data[2] < mins[2]) mins[2] = verts[frame][vert].data[2];
	if(verts[frame][vert].data[0] > maxs[0]) maxs[0] = verts[frame][vert].data[0];
	if(verts[frame][vert].data[1] > maxs[1]) maxs[1] = verts[frame][vert].data[1];
	if(verts[frame][vert].data[2] > maxs[2]) maxs[2] = verts[frame][vert].data[2];
	}
      }
    }

  fprintf(stderr, "(%.2f-%.2f, %.2f-%.2f, %.2f-%.2f)\n",
	mins[0], maxs[0], mins[1], maxs[1], mins[2], maxs[2]);

//  //I don't think I need this anymore.
//  tri_vert.resize(num_triangles*3);
//  tri_tex.resize(num_triangles*3);
//  SDL_RWseek(model, triangles_offset, SEEK_SET);
//  for(Uint32 tri=0; tri < num_triangles; tri += 3) {
//    freadLE(tri_vert[tri + 0], model);
//    freadLE(tri_vert[tri + 1], model);
//    freadLE(tri_vert[tri + 2], model);
//    freadLE(tri_tex[tri + 0], model);
//    freadLE(tri_tex[tri + 1], model);
//    freadLE(tri_tex[tri + 2], model);
//    }

  glcomms.resize(num_glcomms);
  SDL_RWseek(model, glcomm_offset, SEEK_SET);
  for(Uint32 glcomm=0; glcomm < num_glcomms; ++glcomm) {
    Sint32 comstat;
    freadLE(comstat, model);
    if(comstat == 0) break;	//Done.
    if(comstat < 0) {
      glcomms[glcomm].strip = true;
      comstat = -comstat;
      }
    else {
      glcomms[glcomm].strip = false;
      }
    glcomms[glcomm].verts.resize(comstat);
    for(Sint32 glind=0; glind < comstat; ++glind) {
      freadLE(glcomms[glcomm].verts[glind].tex_x, model);
      freadLE(glcomms[glcomm].verts[glind].tex_y, model);
      freadLE(glcomms[glcomm].verts[glind].vindex, model);
      }
    }

  if(defskin.length() > 0) {
    AddSkin(filenm + "/" + defskin);
    }

  SDL_RWseek(model, skins_offset, SEEK_SET);
  for(Uint32 skin=0; skin < num_skins; ++skin) {
    Sint8 skinnm[68] = {0};
    SDL_RWread(model, skinnm, 1, 64);
    AddSkin(filenm + "/" + (char*)skinnm);
    }

  return false;
  }

bool SimpleModel_MD2::RenderSelf(Uint32 cur_time, const vector<int> &anim,
	const vector<Uint32> &start_time, Uint32 anim_offset) const {
//  glCullFace(GL_FRONT);	//Do MD2 models use front face culling too?

  glBegin(GL_TRIANGLES);

//  if(texture.size() > 0) glBindTexture(GL_TEXTURE_2D, texture[0]->GLTexture());

  vector<GLCommand>::const_iterator glcomm = glcomms.begin();
  for(; glcomm != glcomms.end(); ++glcomm) {
    if(glcomm->strip) glBegin(GL_TRIANGLE_STRIP);
    else glBegin(GL_TRIANGLES);

    Uint32 frame = (cur_time/100) % verts.size();

    vector<GLVertex>::const_iterator glvert = glcomm->verts.begin();
    for(; glvert != glcomm->verts.end(); ++glvert) {
//      if(texture.size() > 0) glTexCoord2f(glvert->tex_x, glvert->tex_y);
      glVertex3f(verts[frame][glvert->vindex].data[0],
		verts[frame][glvert->vindex].data[1],
		verts[frame][glvert->vindex].data[2]
		);
      }

    glEnd();
    }

//  glCullFace(GL_BACK);

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
  int frame = 0;
  return frame;
  }

int SimpleModel_MD2::NormalizeFrame(int anim, int frame) const {
  return frame;
  }

const vector<string> &SimpleModel_MD2::GetSkinList() {
  return skins;
  }

void SimpleModel_MD2::AddSkin(const string &skinnm) {
  fprintf(stderr, "Load skin: '%s'\n", skinnm.c_str());
//  SimpleTexture *skin = new SimpleTexture(skinnm.c_str());
//  if(skin && (skin->type != SIMPLETEXTURE_NONE)) {
//    fprintf(stderr, "Success!\n");
//    texture.push_back(skin);
//    skins.push_back(skinnm);
//    }
//  else if(skin) {
//    fprintf(stderr, "Failed!\n");
//    delete skin;
//    }
  }
