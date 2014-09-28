// *************************************************************************
//  This file is part of the SimpleModel Example Module by Steaphan Greene
//
//  Copyright 2005-2014 Steaphan Greene <stea@cs.binghamton.edu>
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
#include "SDL_image.h"

#include <cmath>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>
using namespace std;

#include "simplemodel_pmd.h"
#include "saferead.h"
#include "iconv_string.h"

#define BONE_NONE 0xFFFF

SimpleModel_PMD::SimpleModel_PMD(const string &filenm,
	const string &defskin) {
  Load(filenm, defskin);
  }

SimpleModel_PMD::SimpleModel_PMD() {
  }

SimpleModel_PMD::~SimpleModel_PMD() {
  }

string SimpleModel_PMD::ReadString(SDL_RWops *model, size_t len) const {
  char *ch = (char*)malloc(len+4);
  memset(ch, 0, len+4);
  SDL_RWread(model, ch, 1, len);

  if(strlen(ch) < len) len = strlen(ch);

  string ret;
  char *utf8 = NULL;
  if (iconv_string("UTF-8", "Shift_JIS", ch, ch+len+1, &utf8, NULL) < 0) {
    //perror("iconv_string");
    ret = ch;
    }
  else {
    ret = utf8;
    }
  free(utf8);
  free(ch);

  // Found some with an extra space in this field - detect and fix that
  if(ret[ret.length()-1] == ' ') ret = ret.substr(0, ret.length()-1);

//  fprintf(stderr, "[%lu] '%s'\n", len, ret.c_str());
  return ret;
  }

bool SimpleModel_PMD::Load(const string &filenm,
	const string &defskin) {
  filename = filenm;
  skinname = defskin;

  SDL_RWops *model = SDL_RWFromZZIP(filename.c_str(), "rb");
  if(!model) {
    fprintf(stderr, "WARNING: Unable to open model file '%s'!\n",
	filename.c_str());
    perror("WARNING");
    return false;
    }

  //These are the parts of the header we're actually reading.
  Sint8 magic[3];
  float filever;

  //Read parts of the header of the PMD file
  SDL_RWread(model, &magic, 1, sizeof(magic));
  if(strncmp((char *)magic, "Pmd", 3)) {
    fprintf(stderr, "WARNING: File '%s' is not a PMD file!\n",
	filename.c_str());
    SDL_RWclose(model);
    return false;
    }
  freadLE(filever, model);
  if(filever != 1.0) {
    fprintf(stderr, "WARNING: File '%s' is not a v1.0 PMD file!\n",
	filename.c_str());
    SDL_RWclose(model);
    return false;
    }

  ReadString(model, 20);
  ReadString(model, 256);

  Uint32 num_vertices;
  freadLE(num_vertices, model);
  vertices.resize(num_vertices);
  for(Uint32 vert = 0; vert < num_vertices; ++vert) {
    // Location vector
    freadLE(vertices[vert].vertex[0], model);
    freadLE(vertices[vert].vertex[1], model);
    freadLE(vertices[vert].vertex[2], model);

    // Normal vector
    freadLE(vertices[vert].normal[0], model);
    freadLE(vertices[vert].normal[1], model);
    freadLE(vertices[vert].normal[2], model);

    // Texture coordinates
    freadLE(vertices[vert].texture[0], model);
    freadLE(vertices[vert].texture[1], model);

    // Bone(s) this vertex is linked to
    freadLE(vertices[vert].bone[0], model);
    freadLE(vertices[vert].bone[1], model);
    Uint8 weight;
    freadLE(weight, model);
    vertices[vert].bone_weight = ((float)(weight)) / 100.0;

    // We don't draw edges
    SDL_RWseek(model, 1, SEEK_CUR);
    }

  Uint32 num_triangles;
  freadLE(num_triangles, model);
  if(num_triangles % 3 != 0) {
    fprintf(stderr, "ERROR: num_triangles (%u) %% 3 != 0\n", num_triangles);
    exit(1);
    }
  num_triangles /= 3;
  triangles.resize(num_triangles);
  for(Uint32 tri = 0; tri < num_triangles; ++tri) {
    for(Uint32 vert = 0; vert < 3; ++vert) {
      Uint16 idx;
      freadLE(idx, model);
      triangles[tri].vertex[vert] = idx;
      }
    }

  Uint32 num_materials;
  freadLE(num_materials, model);
  material.resize(num_materials);
  for(Uint32 mat = 0; mat < num_materials; ++mat) {
    material[mat].mode = 1; // All PMD materials are PMX equivalent of mode 1

    freadLE(material[mat].diffuse[0], model);
    freadLE(material[mat].diffuse[1], model);
    freadLE(material[mat].diffuse[2], model);
    freadLE(material[mat].diffuse[3], model);

    freadLE(material[mat].specularity, model);

    freadLE(material[mat].specular[0], model);
    freadLE(material[mat].specular[1], model);
    freadLE(material[mat].specular[2], model);

    freadLE(material[mat].ambient[0], model);
    freadLE(material[mat].ambient[1], model);
    freadLE(material[mat].ambient[2], model);

    SDL_RWseek(model, 1, SEEK_CUR); // Toon Index
    SDL_RWseek(model, 1, SEEK_CUR); // Edge Flag

    freadLE(material[mat].num_tris, model);
    material[mat].num_tris /= 3;

    string texfile_part = ReadString(model, 20);
    if(texfile_part.length() < 3) {
      fprintf(stderr, "WARNING: Blank tex '%s'\n", texfile_part.c_str());
      material[mat].texidx = texture.size();
      texture.push_back(NULL);
      continue;
      }
    if(texfile_part.find('*') != string::npos) {
      texfile_part = texfile_part.substr(0, texfile_part.find('*'));
      // FIXME: Ignoring sphere part (after the '*')
      }
    if(texfile_part.length() < 3) {
      fprintf(stderr, "WARNING: Blank base tex '%s'\n", texfile_part.c_str());
      material[mat].texidx = texture.size();
      texture.push_back(NULL);
      continue;
      }
    string texfile = filename.substr(0, filename.find_last_of("/") + 1);
    texfile += texfile_part;
    SimpleTexture *tmptex = new SimpleTexture(texfile.c_str());
    if(tmptex->type != SIMPLETEXTURE_NONE) {
      material[mat].texidx = texture.size();
      texture.push_back(tmptex);
      }
    else {
      fprintf(stderr, "WARNING: Failed to load tex '%s'\n", texfile.c_str());
      material[mat].texidx = texture.size();
      texture.push_back(NULL);
      delete tmptex;
      }
    }

  Uint16 num_bones;
  freadLE(num_bones, model);
  bone.resize(num_bones);
  for(Uint16 bn = 0; bn < num_bones; ++bn) {
    // Only interpret 15 of the 20 characters, so the name is consistent
    bone[bn].name = ReadString(model, 15);
    SDL_RWseek(model, 5, SEEK_CUR);

    bone_by_name[bone[bn].name] = bn;

    //fprintf(stderr, "Loading Bone #%u: %s\n", bn, bone[bn].name.c_str());

    Uint16 parent = 0xFFFF;
    freadLE(parent, model);
    bone[bn].parent = parent;

    //freadLE(bone[bn].child, model);
    SDL_RWseek(model, 2, SEEK_CUR);

    Uint8 type;
    freadLE(type, model);
    // TODO: Convert "type" to PMX flags

    //freadLE(bone[bn].target, model);
    SDL_RWseek(model, 2, SEEK_CUR);

    freadLE(bone[bn].pos.data[0], model);
    freadLE(bone[bn].pos.data[1], model);
    freadLE(bone[bn].pos.data[2], model);
    }

  return false;
  }

bool SimpleModel_PMD::LoadAnimation(const string &filename) {
  SDL_RWops *model = SDL_RWFromZZIP(filename.c_str(), "rb");
  if(!model) {
    fprintf(stderr, "WARNING: Unable to open animation file '%s'!\n",
	filename.c_str());
    perror("WARNING");
    return false;
    }

  //These are the parts of the header we're actually reading.
  Sint8 magic[30];

  //Read parts of the header of the VMD file
  SDL_RWread(model, &magic, 1, sizeof(magic));
  string name = "";
  if(!strncmp((char *)magic, "Vocaloid Motion Data 0002", 26)) {
    name = ReadString(model, 20);
    }
  else if(!strncmp((char *)magic, "Vocaloid Motion Data file", 26)) {
    name = ReadString(model, 10);
    }
  else {
    fprintf(stderr, "WARNING: File '%s' is not a VMD file!\n",
	filename.c_str());
    SDL_RWclose(model);
    return false;
    }
  fprintf(stderr, "Loading VMD: %s\n", name.c_str());

  Uint32 num_bone_frames;
  freadLE(num_bone_frames, model);
  for(Uint32 kf = 0; kf < num_bone_frames; ++kf) {
    string bone_name = ReadString(model, 15);

    Uint32 frame;
    freadLE(frame, model);

    if(bone_by_name.count(bone_name) != 1) {
//      fprintf(stderr, "Discarding frame %u data for unknown bone (%s).\n",
//              frame, bone_name.c_str());
      SDL_RWseek(model, 64+28, SEEK_CUR);
      continue;
      }

    Uint16 bone = bone_by_name[bone_name];

    // Coordinate vector
    freadLE(bone_frame[bone][frame].pos.data[0], model);
    freadLE(bone_frame[bone][frame].pos.data[1], model);
    freadLE(bone_frame[bone][frame].pos.data[2], model);

    // Rotation quaternion
    freadLE(bone_frame[bone][frame].rot.x, model);
    freadLE(bone_frame[bone][frame].rot.y, model);
    freadLE(bone_frame[bone][frame].rot.z, model);
    freadLE(bone_frame[bone][frame].rot.w, model);

    // Load bezier interp data... stored [0..127]
    // X_x1, Y_x1, Z_x1, R_x1, X_y1, Y_y1, Z_y1, R_y1,
    // X_x2, Y_x2, Z_x2, R_x2, X_y2, Y_y2, Z_y2, R_y2,
    Uint8 bez[16];
    SDL_RWread(model, bez, 1, 16);

    // Scale them to anchor points (0.0, 0.0), and (1.0, 1.0)
    for(Uint32 base = 0; base < 16; ++base) {
      bone_frame[bone][frame].bez_x[0] = bez[0] / 127.0;
      bone_frame[bone][frame].bez_x[1] = bez[4] / 127.0;
      bone_frame[bone][frame].bez_x[2] = bez[8] / 127.0;
      bone_frame[bone][frame].bez_x[3] = bez[12] / 127.0;
      bone_frame[bone][frame].bez_y[0] = bez[1] / 127.0;
      bone_frame[bone][frame].bez_y[1] = bez[5] / 127.0;
      bone_frame[bone][frame].bez_y[2] = bez[9] / 127.0;
      bone_frame[bone][frame].bez_y[3] = bez[13] / 127.0;
      bone_frame[bone][frame].bez_z[0] = bez[2] / 127.0;
      bone_frame[bone][frame].bez_z[1] = bez[6] / 127.0;
      bone_frame[bone][frame].bez_z[2] = bez[10] / 127.0;
      bone_frame[bone][frame].bez_z[3] = bez[14] / 127.0;
      bone_frame[bone][frame].bez_r[0] = bez[3] / 127.0;
      bone_frame[bone][frame].bez_r[1] = bez[7] / 127.0;
      bone_frame[bone][frame].bez_r[2] = bez[11] / 127.0;
      bone_frame[bone][frame].bez_r[3] = bez[15] / 127.0;
      }

    // Remaining 48 bytes are totally redundant???
    // Y_x1, Z_x1, R_x1, X_y1, Y_y1, Z_y1, R_y1, X_x2,
    // Y_x2, Z_x2, R_x2, X_y2, Y_y2, Z_y2, R_y2, 01,
    // Z_x1, R_x1, X_y1, Y_y1, Z_y1, R_y1, X_x2, Y_x2,
    // Z_x2, R_x2, X_y2, Y_y2, Z_y2, R_y2, 01, 00,
    // R_x1, X_y1, Y_y1, Z_y1, R_y1, X_x2, Y_x2, Z_x2,
    // R_x2, X_y2, Y_y2, Z_y2, R_y2, 01, 00, 00
    SDL_RWseek(model, 48, SEEK_CUR);
    }
  return false;
  }
