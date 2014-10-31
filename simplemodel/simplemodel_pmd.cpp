// *************************************************************************
//  This file is part of the SimpleModel Example Module by Steaphan Greene
//
//  Copyright 2005-2014 Steaphan Greene <steaphan@gmail.com>
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

SimpleModel_PMD::SimpleModel_PMD(const string &filename,
                                 const string &defskin) {
  Load(filename, defskin);
}

SimpleModel_PMD::SimpleModel_PMD() {}

SimpleModel_PMD::~SimpleModel_PMD() {}

bool SimpleModel_PMD::Load(const string &filename, const string &defskin) {
  SDL_RWops *model = SDL_RWFromZZIP(filename.c_str(), "rb");
  if (!model) {
    fprintf(stderr, "WARNING: Unable to open model file '%s'!\n",
            filename.c_str());
    perror("WARNING");
    return false;
  }

  // These are the parts of the header we're actually reading.
  Sint8 magic[3];
  float filever;

  // Read parts of the header of the PMD file
  SDL_RWread(model, &magic, 1, sizeof(magic));
  if (strncmp((char *)magic, "Pmd", 3)) {
    fprintf(stderr, "WARNING: File '%s' is not a PMD file!\n",
            filename.c_str());
    SDL_RWclose(model);
    return false;
  }
  freadLE(filever, model);
  if (filever != 1.0) {
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
  for (Uint32 vert = 0; vert < num_vertices; ++vert) {
    // Location vector
    freadLE(vertices[vert].vertex[0], model);
    freadLE(vertices[vert].vertex[1], model);
    freadLE(vertices[vert].vertex[2], model);
    vertices[vert].vertex[0] *= -1.0;

    // Normal vector
    freadLE(vertices[vert].normal[0], model);
    freadLE(vertices[vert].normal[1], model);
    freadLE(vertices[vert].normal[2], model);
    vertices[vert].normal[0] *= -1.0;

    // Texture coordinates
    freadLE(vertices[vert].texcoord[0], model);
    freadLE(vertices[vert].texcoord[1], model);

    // Bone(s) this vertex is linked to
    vertices[vert].bone[0] = ReadVarInt(model, 2);
    vertices[vert].bone[1] = ReadVarInt(model, 2);
    Uint8 weight;
    freadLE(weight, model);
    if (weight == 100) {
      // Really only the first bone, equivalent to PMX type 0
      vertices[vert].bone_weight_type = 0;
    } else if (weight == 0) {
      // Really only the second bone, convert to PMX type 0
      vertices[vert].bone_weight_type = 0;
      vertices[vert].bone[0] = vertices[vert].bone[1];
    } else {
      // Normal PMD two-bone weight, equivalent to PMX type 1
      vertices[vert].bone_weight_type = 1;
      vertices[vert].bone_weight[0] = ((float)(weight)) / 100.0;
      vertices[vert].bone_weight[1] = 1.0 - vertices[vert].bone_weight[0];
    }

    // We don't draw edges
    SDL_RWseek(model, 1, SEEK_CUR);
  }

  Uint32 num_triangles;
  freadLE(num_triangles, model);
  if (num_triangles % 3 != 0) {
    fprintf(stderr, "ERROR: num_triangles (%u) %% 3 != 0\n", num_triangles);
    exit(1);
  }
  num_triangles /= 3;
  triangles.resize(num_triangles);
  for (Uint32 tri = 0; tri < num_triangles; ++tri) {
    for (Uint32 vert = 0; vert < 3; ++vert) {
      Uint16 idx = 0;
      freadLE(idx, model);
      triangles[tri].vertex[vert] = idx;
    }
  }

  Uint32 num_materials;
  freadLE(num_materials, model);
  material.resize(num_materials);
  for (Uint32 mat = 0; mat < num_materials; ++mat) {
    material[mat].mode = 1;  // All PMD materials are PMX equivalent of mode 1

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

    SDL_RWseek(model, 1, SEEK_CUR);  // Toon Index
    SDL_RWseek(model, 1, SEEK_CUR);  // Edge Flag

    freadLE(material[mat].num_tris, model);
    material[mat].num_tris /= 3;

    string texfile_part = ReadString(model, 20);
    if (texfile_part.length() < 3) {
      fprintf(stderr, "WARNING: Blank tex '%s'\n", texfile_part.c_str());
      material[mat].texidx = texture.size();
      texture.push_back(NULL);
      continue;
    }
    if (texfile_part.find('*') != string::npos) {
      texfile_part = texfile_part.substr(0, texfile_part.find('*'));
      // FIXME: Ignoring sphere part (after the '*')
    }
    if (texfile_part.length() < 3) {
      fprintf(stderr, "WARNING: Blank base tex '%s'\n", texfile_part.c_str());
      material[mat].texidx = texture.size();
      texture.push_back(NULL);
      continue;
    }
    string texfile = filename.substr(0, filename.find_last_of("/") + 1);
    texfile += texfile_part;
    SimpleTexture *tmptex = new SimpleTexture(texfile.c_str());
    if (tmptex->type != SIMPLETEXTURE_NONE) {
      material[mat].texidx = texture.size();
      texture.push_back(tmptex);
    } else {
      fprintf(stderr, "WARNING: Failed to load tex '%s'\n", texfile.c_str());
      material[mat].texidx = texture.size();
      texture.push_back(NULL);
      delete tmptex;
    }
  }

  Uint32 num_bones;
  num_bones = ReadVarInt(model, 2);
  bone.resize(num_bones);
  for (Uint32 bn = 0; bn < num_bones; ++bn) {
    // Only interpret 15 of the 20 characters, so the name is consistent
    bone[bn].name = ReadString(model, 15);
    SDL_RWseek(model, 5, SEEK_CUR);

    bone_by_name[bone[bn].name] = bn;

    // fprintf(stderr, "Loading Bone #%u: %s\n", bn, bone[bn].name.c_str());

    bone[bn].parent = ReadVarInt(model, 2);

    // freadLE(bone[bn].child, model);
    SDL_RWseek(model, 2, SEEK_CUR);

    Uint8 type;
    freadLE(type, model);
    // TODO: Convert "type" to PMX flags

    // freadLE(bone[bn].target, model);
    SDL_RWseek(model, 2, SEEK_CUR);

    freadLE(bone[bn].pos.data[0], model);
    freadLE(bone[bn].pos.data[1], model);
    freadLE(bone[bn].pos.data[2], model);
    bone[bn].pos.data[0] *= -1.0;

    bone[bn].effector = 0xFFFFFFFF;  // Effector is not supported in PMD format
  }

  Uint32 num_ik_chains;
  num_ik_chains = ReadVarInt(model, 2);
  for (Uint32 ik = 0; ik < num_ik_chains; ++ik) {
    Uint32 bn = ReadVarInt(model, 2);
    bone_target[bn] = ReadVarInt(model, 2);

    Uint32 num_ik_links = ReadVarInt(model, 1);

    // Ignore IK Algorithm Hints, For Now
    SDL_RWseek(model, 6, SEEK_CUR);

    for (Uint32 link = 0; link < num_ik_links; ++link) {
      IKLink ik;
      ik.bone = ReadVarInt(model, 2);
      ik.limited = 0;  // Always 0 - PMD format doesn't support these limits
      ik_link[bn].push_back(ik);
    }
  }

  return false;
}
