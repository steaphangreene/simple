// *************************************************************************
//  This file is part of the SimpleModel Example Module by Steaphan Greene
//
//  Copyright 2005 Steaphan Greene <stea@cs.binghamton.edu>
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

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>
using namespace std;

#include "simplemodel_q3dir.h"

static long fread_longLE(FILE *fl) {
  union { long l; char c[4]; } data;

  fread(&data.l, 1, sizeof(long), fl);

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  swap(data.c[3], data.c[0]);
  swap(data.c[2], data.c[1]);
#endif

  return data.l;
  }

static unsigned long fread_ulongLE(FILE *fl) {
  union { unsigned long l; char c[4]; } data;

  fread(&data.l, 1, sizeof(unsigned long), fl);

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  swap(data.c[3], data.c[0]);
  swap(data.c[2], data.c[1]);
#endif

  return data.l;
  }

static short fread_shortLE(FILE *fl) {
  union { short s; char c[4]; } data;

  fread(&data.s, 1, sizeof(short), fl);

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  swap(data.c[3], data.c[0]);
  swap(data.c[2], data.c[1]);
#endif

  return data.s;
  }

static float fread_floatLE(FILE *fl) {
  union { float f; char c[4]; } data;

  fread(&data.f, 1, sizeof(float), fl);

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  swap(data.c[3], data.c[0]);
  swap(data.c[2], data.c[1]);
#endif

  return data.f;
  }


SimpleModel_Md3::SimpleModel_Md3(const string &filenm,
	const string &modelnm, const string &skinnm) {
  current_anim = 0;
  current_frame = 0;
  last_time = 0;

  Load(filenm, modelnm, skinnm);
  }

SimpleModel_Md3::SimpleModel_Md3() {
  current_anim = 0;
  current_frame = 0;
  last_time = 0;
  }

SimpleModel_Md3::~SimpleModel_Md3() {
  }

static int nextpoweroftwo(int x) {
  if(x <= 2) return 2;

  --x;		//Hitch it down in case it's exactly a power of 2
  int p = 1;
  for(; x != 1; ++p, x>>=1);
  x <<= p;
  return x;
  }


bool SimpleModel_Md3::Load(const string &filenm,
	const string &modelnm, const string &skinnm) {
  filename = filenm;
  modelname = modelnm;
  if(skinnm.length() >  0) skinname = skinnm;
  else skinname = modelname.substr(0, skinname.length() - 4) + "_default.skin";

  FILE *model = fopen(modelname.c_str(), "r");
  if(!model) {
    fprintf(stderr, "WARNING: Unable to open model file '%s'!\n",
	skinname.c_str());
    return false;
    }

  //These are the parts of the header we're actually reading.
  char magic[4];
  long filever;
  unsigned long num_frames;

  //Read parts of the header of the MD3 file
  fread(&magic, 1, sizeof(magic), model);
  if(strncmp((char*)(&magic), "IDP3", 4)) {
    fprintf(stderr, "WARNING: File '%s' is not an MD3 file!\n",
	skinname.c_str());
    return false;
    }
  filever = fread_longLE(model);
  if(filever != 15) {
    fprintf(stderr, "WARNING: File '%s' is not an v15 MD3 file!\n",
	skinname.c_str());
    return false;
    }
  fseek(model, 68, SEEK_CUR); // Skip the name of the file

  num_frames = fread_ulongLE(model);
  num_tags = fread_ulongLE(model);
  meshes.resize(fread_longLE(model));

  fseek(model, 20, SEEK_CUR); // Skip the rest of the header (FIXME: Skins!)


  //FIXME: This skips the bones, do we want to actually use them?
  fseek(model, 56 * num_frames, SEEK_CUR);


  pTags.resize(num_frames * num_tags);
  for(unsigned int tag = 0; tag < num_frames * num_tags; ++tag) {
    fread(pTags[tag].name, 64, 1, model);
    pTags[tag].pos.data[12] = fread_floatLE(model);	// Location vector
    pTags[tag].pos.data[13] = fread_floatLE(model);	// Location vector
    pTags[tag].pos.data[14] = fread_floatLE(model);	// Location vector
    pTags[tag].pos.data[15] = 1;
    pTags[tag].pos.data[0] = fread_floatLE(model);	// Rotation matrix
    pTags[tag].pos.data[1] = fread_floatLE(model);	// Rotation matrix
    pTags[tag].pos.data[2] = fread_floatLE(model);	// Rotation matrix
    pTags[tag].pos.data[3] = 0;
    pTags[tag].pos.data[4] = fread_floatLE(model);	// Rotation matrix
    pTags[tag].pos.data[5] = fread_floatLE(model);	// Rotation matrix
    pTags[tag].pos.data[6] = fread_floatLE(model);	// Rotation matrix
    pTags[tag].pos.data[7] = 0;
    pTags[tag].pos.data[8] = fread_floatLE(model);	// Rotation matrix
    pTags[tag].pos.data[9] = fread_floatLE(model);	// Rotation matrix
    pTags[tag].pos.data[10] = fread_floatLE(model);	// Rotation matrix
    }


  // Get the current offset into the file
  long offset = ftell(model);

  // Go through all of the sub-objects in this mesh
  for (unsigned int meshnum = 0; meshnum < meshes.size(); meshnum++) {
	// Seek to the start of this mesh and read in it's header
    fseek(model, offset+4, SEEK_SET);	//I'm skipping the meshID here

    fread(meshes[meshnum].name, 68, 1, model);

    unsigned long num_mframes = fread_ulongLE(model);
    fseek(model, 4, SEEK_CUR);		//I'm skipping the number of skins here
    unsigned long num_vertices = fread_ulongLE(model);
    unsigned long num_faces = fread_ulongLE(model);

    long face_offset = fread_longLE(model);
    fseek(model, 4, SEEK_CUR);		//I'm skipping the header size here
    long uv_offset = fread_longLE(model);
    long vertex_offset = fread_longLE(model);
    long mesh_size = fread_longLE(model);


    meshes[meshnum].faces.resize(num_faces);
    meshes[meshnum].coords.resize(num_vertices);
    meshes[meshnum].triangles.resize(num_vertices * num_mframes);

    //FIXME: We skip the skin names here

	// Seek to the start of the face data, then read it in
    fseek(model, offset + face_offset, SEEK_SET);
    for(unsigned int face=0; face < meshes[meshnum].faces.size(); ++face) {
      meshes[meshnum].faces[face].vertices[0] = fread_longLE(model);
      meshes[meshnum].faces[face].vertices[1] = fread_longLE(model);
      meshes[meshnum].faces[face].vertices[2] = fread_longLE(model);
      }

	// Seek to the start of the texture coordinate data, then read it in
    fseek(model, offset + uv_offset, SEEK_SET);
    for(unsigned int vert=0; vert < meshes[meshnum].coords.size(); ++vert) {
      meshes[meshnum].coords[vert].coord[0] = fread_floatLE(model);
      meshes[meshnum].coords[vert].coord[1] = fread_floatLE(model);
      }

	// Seek to the start of the triangle information, then read it in.
    fseek(model, offset + vertex_offset, SEEK_SET);
    for(unsigned int vert=0; vert < meshes[meshnum].triangles.size(); ++vert) {
      meshes[meshnum].triangles[vert].vertex[0] = float(fread_shortLE(model)) / 64.0F;
      meshes[meshnum].triangles[vert].vertex[1] = float(fread_shortLE(model)) / 64.0F;
      meshes[meshnum].triangles[vert].vertex[2] = float(fread_shortLE(model)) / 64.0F;
      fread(meshes[meshnum].triangles[vert].normal, 1, 2, model); // FIXME: What are these for?
      }

    // Point offset to the next mesh location in the file
    offset += mesh_size;
    }
  fclose(model);


  // Here we load the requested skin (FIXME: without checking the skin list!)
  FILE *skin = fopen(skinname.c_str(), "r");
  if(!skin) {
    fprintf(stderr, "WARNING: Unable to open skin file '%s'!\n",
	skinname.c_str());
    return false;
    }

  char obj[256] = {0}, tex[256] = {0};
  while(1) {
    int num = fscanf(skin, " %[^ \n\t\r,],%[^ \n\t\r]\n", obj, tex);
    if(num < 1) break;
    else if(num == 1) {
      //FIXME: What do these lines mean?  Don't know what to do here.
      }
    else {
      for(unsigned int i = 0; i < meshes.size(); i++) {
	if(!strcasecmp(obj, meshes[i].name)) {
	  SM_Texture *tmptex = new SM_Texture(tex);
	  texture.push_back(tmptex);

	  //FIXME: This should be in SM_Texture!
	  int xsize = nextpoweroftwo(tmptex->src->w);
	  int ysize = nextpoweroftwo(tmptex->src->h);
	  tmptex->cur = SDL_CreateRGBSurface(0, xsize, ysize, 32,
		SG_SDL_RGBA_COLFIELDS);
	  memset(tmptex->cur->pixels, 0, xsize*ysize*4);
	  SDL_SetAlpha(tmptex->src, 0, SDL_ALPHA_OPAQUE);
	  SDL_BlitSurface(tmptex->src, NULL, tmptex->cur, NULL);

          tmptex->Update();

	  meshes[i].texture = texture.back()->GLTexture();
	  }
	}
      }
    memset(obj, 0, 256);
    memset(tex, 0, 256);
    }
  fclose(skin);

  return false;
  }

bool SimpleModel_Md3::Render(Uint32 cur_time) {
  glCullFace(GL_FRONT);	//Md3 models use front face culling

  int start = 0, end = 1;

  if(animations.size() > 0) {
    start = animations[current_anim].start;
    end   = animations[current_anim].end;

    int next_frame = (current_frame + 1);
    if(next_frame >= end) next_frame = start;

    //FIXME: Actually interpolate!
    Uint32 time = SDL_GetTicks();
    float since = time - last_time;

    int fps = animations[current_anim].fps;
    if(since >= (1000.0 / fps)) {
      current_frame = next_frame;
      last_time = time;
      }
    }

  for(unsigned int obj = 0; obj < meshes.size(); ++obj) {
    glBindTexture(GL_TEXTURE_2D, meshes[obj].texture);

    glBegin(GL_TRIANGLES);

    int vertindex = current_frame * meshes[obj].coords.size();

    for(unsigned int j = 0; j < meshes[obj].faces.size(); j++) {
      for(int whichVertex = 0; whichVertex < 3; whichVertex++) {
	int index = meshes[obj].faces[j].vertices[whichVertex];

	glTexCoord2f(meshes[obj].coords[index].coord[0],
		meshes[obj].coords[index].coord[1]);

	glVertex3f(meshes[obj].triangles[vertindex + index].vertex[0],
		meshes[obj].triangles[vertindex + index].vertex[1],
		meshes[obj].triangles[vertindex +index].vertex[2]);
        }
      }
    glEnd();
    }

  glCullFace(GL_BACK);

  return false;
  }

void SimpleModel_Md3::SetAnimation(int anim) {
  if(animations.size() > 0 && (int)(animations.size()) > anim) {
    current_anim = anim;
    current_frame = animations[current_anim].start;
    last_time = SDL_GetTicks();
    }
  }

int SimpleModel_Md3::AddAnimation(int start, int end, int loop, int fps) {
  Md3AnimationData anim;

  anim.start = start;
  anim.end = end;
  anim.loop = loop;
  anim.fps = fps;
  animations.push_back(anim);

  return animations.size() - 1;
  }

int SimpleModel_Md3::GetAnimation() {
  return current_anim;
  }

unsigned long SimpleModel_Md3::TagNameToIndex(const string &tagname) {
  unsigned int tag = 0;
  for(; tag < num_tags; ++tag) {
    if(!strcasecmp(pTags[tag].name, tagname.c_str())) break;
    }
  if(tag == num_tags) return ULONG_MAX;
  else return tag;
  }

bool SimpleModel_Md3::MoveToTag(unsigned long tag) {
  if(tag >= num_tags) return false;

  //FIXME: Do Interpolation!
  Quaternion matrix = pTags[current_frame * num_tags + tag].pos;

  glMultMatrixf(matrix.data);

  return true;
  }
