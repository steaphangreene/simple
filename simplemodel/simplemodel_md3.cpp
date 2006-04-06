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

#include "simplemodel_q3dir.h"
#include "saferead.h"

SimpleModel_MD3::SimpleModel_MD3(const string &filenm,
	const string &modelnm, const string &defskin) {
  Load(filenm, modelnm, defskin);
  }

SimpleModel_MD3::SimpleModel_MD3() {
  }

SimpleModel_MD3::~SimpleModel_MD3() {
  }

bool SimpleModel_MD3::Load(const string &filenm,
	const string &modelnm, const string &defskin) {
  filename = filenm;
  modelname = modelnm;

  skinname = defskin;
  skinname = modelname.substr(0, modelname.length() - 4) + "_"
	+ skinname+ ".skin";

  SDL_RWops *model = SDL_RWFromFile(modelname.c_str(), "rb");
  if(!model) {
    fprintf(stderr, "WARNING: Unable to open model file '%s'!\n",
	modelname.c_str());
    perror("WARNING");
    return false;
    }

  bool external_skinfile = false;

  //These are the parts of the header we're actually reading.
  Sint8 magic[4];
  Sint32 filever;
  Uint32 num_frames;

  //Read parts of the header of the MD3 file
  SDL_RWread(model, &magic, 1, sizeof(magic));
  if(strncmp((char *)magic, "IDP3", 4)) {
    fprintf(stderr, "WARNING: File '%s' is not an MD3 file!\n",
	modelnm.c_str());
    return false;
    }
  freadLE(filever, model);
  if(filever != 15) {
    fprintf(stderr, "WARNING: File '%s' is not an v15 MD3 file!\n",
	modelnm.c_str());
    return false;
    }
  SDL_RWseek(model, 68, SEEK_CUR); // Skip the name of the file

  freadLE(num_frames, model);
  freadLE(num_tags, model);
  Uint32 num_meshes;
  freadLE(num_meshes, model);
  meshes.resize(num_meshes);

  SDL_RWseek(model, 20, SEEK_CUR); // Skip the rest of the header (FIXME: Skins!)

  //FIXME: This skips the bones, do we want to actually use them?
  SDL_RWseek(model, 56 * num_frames, SEEK_CUR);


  pTags.resize(num_frames * num_tags);
  for(unsigned int tag = 0; tag < num_frames * num_tags; ++tag) {
    SDL_RWread(model, pTags[tag].name, 64, 1);
    freadLE(pTags[tag].pos.data[12], model);	// Location vector
    freadLE(pTags[tag].pos.data[13], model);	// Location vector
    freadLE(pTags[tag].pos.data[14], model);	// Location vector
    pTags[tag].pos.data[15] = 1;
    freadLE(pTags[tag].pos.data[0], model);	// Rotation matrix
    freadLE(pTags[tag].pos.data[1], model);	// Rotation matrix
    freadLE(pTags[tag].pos.data[2], model);	// Rotation matrix
    pTags[tag].pos.data[3] = 0;
    freadLE(pTags[tag].pos.data[4], model);	// Rotation matrix
    freadLE(pTags[tag].pos.data[5], model);	// Rotation matrix
    freadLE(pTags[tag].pos.data[6], model);	// Rotation matrix
    pTags[tag].pos.data[7] = 0;
    freadLE(pTags[tag].pos.data[8], model);	// Rotation matrix
    freadLE(pTags[tag].pos.data[9], model);	// Rotation matrix
    freadLE(pTags[tag].pos.data[10], model);	// Rotation matrix
    }


  // Get the current offset into the file
  long offset = SDL_RWtell(model);

  // Go through all of the sub-objects in this mesh
  for (unsigned int meshnum = 0; meshnum < meshes.size(); meshnum++) {
	// Seek to the start of this mesh and read in it's header
    SDL_RWseek(model, offset+4, SEEK_SET);	//I'm skipping the meshID here

    SDL_RWread(model, meshes[meshnum].name, 68, 1);

    Uint32 num_mframes;
    freadLE(num_mframes, model);

    Uint32 num_shaders;
    freadLE(num_shaders, model);

    Uint32 num_vertices;
    freadLE(num_vertices, model);

    Uint32 num_faces;
    freadLE(num_faces, model);

    Sint32 face_offset;
    freadLE(face_offset, model);

    Sint32 shader_offset;
    freadLE(shader_offset, model);

    Sint32 uv_offset;
    freadLE(uv_offset, model);

    Sint32 vertex_offset;
    freadLE(vertex_offset, model);

    Sint32 mesh_size;
    freadLE(mesh_size, model);

    meshes[meshnum].faces.resize(num_faces);
    meshes[meshnum].coords.resize(num_vertices);
    meshes[meshnum].triangles.resize(num_vertices * num_mframes);

	// Seek to the start of the face data, then read it in
    SDL_RWseek(model, offset + shader_offset, SEEK_SET);
    for(unsigned int shader=0; shader < num_shaders; ++shader) {
      Sint8 buf[65] = {0};
      SDL_RWread(model, buf, 64, 1);
      AddSkin((char*)buf);
      if(skins.back().length() > 0) {
	SimpleTexture *tmptex = new SimpleTexture(skins.back());
	texture.push_back(tmptex);
	meshes[meshnum].texture = texture.back();
	}
      else {
	external_skinfile = true;
	}
      }

	// Seek to the start of the face data, then read it in
    SDL_RWseek(model, offset + face_offset, SEEK_SET);
    for(unsigned int face=0; face < meshes[meshnum].faces.size(); ++face) {
      freadLE(meshes[meshnum].faces[face].vertices[0], model);
      freadLE(meshes[meshnum].faces[face].vertices[1], model);
      freadLE(meshes[meshnum].faces[face].vertices[2], model);
      }

	// Seek to the start of the texture coordinate data, then read it in
    SDL_RWseek(model, offset + uv_offset, SEEK_SET);
    for(unsigned int vert=0; vert < meshes[meshnum].coords.size(); ++vert) {
      freadLE(meshes[meshnum].coords[vert].coord[0], model);
      freadLE(meshes[meshnum].coords[vert].coord[1], model);
      }

	// Seek to the start of the triangle information, then read it in.
    SDL_RWseek(model, offset + vertex_offset, SEEK_SET);
    for(unsigned int vert=0; vert < meshes[meshnum].triangles.size(); ++vert) {
      Sint16 tmp;
      freadLE(tmp, model);
      meshes[meshnum].triangles[vert].vertex[0] = float(tmp) / 64.0F;
      freadLE(tmp, model);
      meshes[meshnum].triangles[vert].vertex[1] = float(tmp) / 64.0F;
      freadLE(tmp, model);
      meshes[meshnum].triangles[vert].vertex[2] = float(tmp) / 64.0F;

      SDL_RWread(model, meshes[meshnum].triangles[vert].normal, 1, 2); // FIXME: What are these for?
      }

    // Point offset to the next mesh location in the file
    offset += mesh_size;
    }
  SDL_RWclose(model);

  // We have all our skins, no need for a .skin file!
  if(!external_skinfile) return false;

  // Here we load the requested skin (FIXME: without checking the skin list!)
  SDL_RWops *skin = SDL_RWFromFile(skinname.c_str(), "rb");
  if(!skin) {
    fprintf(stderr, "WARNING: Unable to open skin file '%s'!\n",
	skinname.c_str());
    perror("WARNING");
    return false;
    }

  SDL_RWseek(skin, 0, SEEK_END);
  int filesz = SDL_RWtell(skin);
  SDL_RWseek(skin, 0, SEEK_SET);
  char *filedata = new char[filesz+1]; //FIXME: Handle Error!
  char *fileptr = filedata;
  while(fileptr < (filedata+filesz)) {
    int res = SDL_RWread(skin, fileptr, 1, filesz);
    if(res <= 0) {
      fprintf(stderr, "%d\n", filesz);
      fprintf(stderr, "ERROR: Could not read from '%s'\n", skinname.c_str());
      return false;
      }
    fileptr += res;
    }
  *fileptr = 0;
  SDL_RWclose(skin);
  fileptr = filedata;

  Sint8 obj[256] = {0}, tex[256] = {0};
  while(1) {
    int num = sscanf(fileptr, " %[^ \n\t\r,],%[^ \n\t\r]\n", obj, tex);

    //Skip to past next whitespace
    while((*fileptr) && !isspace(*fileptr)) ++fileptr;
    while((*fileptr) && isspace(*fileptr)) ++fileptr;

    if(num < 1) break;
    else if(num == 1) {
      //FIXME: What do these lines mean?  Don't know what to do here.
      }
    else {
      for(unsigned int i = 0; i < meshes.size(); i++) {
	if(!strcasecmp((char*)obj, (char*)meshes[i].name)) {
	  SimpleTexture *tmptex = new SimpleTexture((char*)tex);
	  texture.push_back(tmptex);

          tmptex->Update();

	  meshes[i].texture = texture.back();
	  }
	}
      }
    memset((char*)obj, 0, 256);
    memset((char*)tex, 0, 256);
    }

  delete [] filedata;
  return false;
  }

bool SimpleModel_MD3::Render(Uint32 cur_time, const vector<int> &anim,
	const vector<Uint32> &start_time) const {
  glCullFace(GL_FRONT);	//MD3 models use front face culling

  float s2;
  int frame = CalcBaseFrame(cur_time, anim, start_time, s2);
  frame = NormalizeFrame(anim, frame);
  int next = NormalizeFrame(anim, frame+1);

  for(unsigned int obj = 0; obj < meshes.size(); ++obj) {
    if(meshes[obj].texture && meshes[obj].texture->type != SIMPLETEXTURE_NONE) {
      glBindTexture(GL_TEXTURE_2D, meshes[obj].texture->GLTexture());

      glBegin(GL_TRIANGLES);

      int vertindex = frame * meshes[obj].coords.size();
      int vertindex2 = next * meshes[obj].coords.size();

      for(unsigned int j = 0; j < meshes[obj].faces.size(); j++) {
	for(int vertex = 0; vertex < 3; vertex++) {
	  int index = meshes[obj].faces[j].vertices[vertex];

	  glTexCoord2f(meshes[obj].coords[index].coord[0],
		meshes[obj].coords[index].coord[1]);

	  float x1 = meshes[obj].triangles[vertindex + index].vertex[0];
	  float y1 = meshes[obj].triangles[vertindex + index].vertex[1];
	  float z1 = meshes[obj].triangles[vertindex + index].vertex[2];

	  float x2 = meshes[obj].triangles[vertindex2 + index].vertex[0];
	  float y2 = meshes[obj].triangles[vertindex2 + index].vertex[1];
	  float z2 = meshes[obj].triangles[vertindex2 + index].vertex[2];

	  float s1 = (1.0f - s2);

	  glVertex3f(s1*x1 + s2*x2, s1*y1 + s2*y2, s1*z1 + s2*z2);
	  }
        }
      }
    glEnd();
    }

  glCullFace(GL_BACK);

  return false;
  }

int SimpleModel_MD3::AddAnimation(int start, int end, int loop, int fps) {
  MD3AnimationData anim;

  anim.start = start;
  anim.end = end;
  anim.loop = loop;
  anim.fps = fps;
  animations.push_back(anim);

  return animations.size() - 1;
  }

unsigned long SimpleModel_MD3::TagNameToIndex(const string &tagname) const {
  unsigned int tag = 0;
  for(; tag < num_tags; ++tag) {
    if(!strcasecmp((char*)pTags[tag].name, tagname.c_str())) break;
    }
  if(tag == num_tags) return ULONG_MAX;
  else return tag;
  }

bool SimpleModel_MD3::MoveToTag(unsigned long tag, Uint32 cur_time,
	const vector<int> &anim, const vector<Uint32> &start_time) const {
  if(tag >= num_tags) return false;

  float s;
  int frame = CalcBaseFrame(cur_time, anim, start_time, s);
  frame = NormalizeFrame(anim, frame);
  int next = NormalizeFrame(anim, frame+1);

  Matrix4x4 matrix;
  SLERP(matrix, pTags[frame*num_tags+tag].pos, pTags[next*num_tags+tag].pos, s);
  glMultMatrixf(matrix.data);

  return true;
  }

int SimpleModel_MD3::CalcBaseFrame(Uint32 cur_time, const vector<int> &anim,
	const vector<Uint32> &start_time, float &offset) const {
  int frame = 0;
  offset = 0.0;
  if(animations.size() > 0) {
    if(anim.size() < 1 || start_time.size() < 1) {
      fprintf(stderr, "WARNING: Not enough anims/times sent to animated MD3.\n");
      return false;
      }
    int start = animations[anim[0]].start;
    float fps = animations[anim[0]].fps;
    float elapsed = cur_time - start_time[0];

    float disp = elapsed * fps / 1000.0;
    frame = start + (int)(disp);
    offset = disp - floorf(disp);
    }
  return frame;
  }

int SimpleModel_MD3::NormalizeFrame(const vector<int> &anim, int frame) const {
  if(animations.size() > 0) {
    if(anim.size() < 1) {
      fprintf(stderr, "WARNING: Not enough anims/times sent to animated MD3.\n");
      return 0;
      }
    int end = animations[anim[0]].end;
    int loop = animations[anim[0]].loop;

    if(frame >= end && loop > 0)
      frame = (end - loop) + (frame - end + loop) % loop;
    else if(frame >= end)
      frame = end - 1;
    }
  else {
    frame = 0;
    }
  return frame;
  }

const vector<string> &SimpleModel_MD3::GetSkinList() {
  return skins;
  }

void SimpleModel_MD3::AddSkin(const string &skinnm) {
  if(skinnm.length() > 4) {
    string suffix = skinnm.substr(skinnm.length()-4);
    if(suffix == ".tga" || suffix == ".TGA") {
      skins.push_back(skinnm.substr(0, skinnm.length()-4));
      return;
      }
    }
  skins.push_back(skinnm);
  }
