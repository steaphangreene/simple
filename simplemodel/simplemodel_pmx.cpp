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

#include "simplemodel_pmx.h"
#include "saferead.h"
#include "iconv_string.h"

SimpleModel_PMX::SimpleModel_PMX(const string &filenm,
	const string &defskin) {
  Load(filenm, defskin);
  }

SimpleModel_PMX::SimpleModel_PMX() {
  }

SimpleModel_PMX::~SimpleModel_PMX() {
  }

Uint32 SimpleModel_PMX::ReadVarInt(SDL_RWops *model, Uint8 size) const {
  if(size == 1) {
    Uint8 idx;
    freadLE(idx, model);
    return Uint32(idx);
    }
  else if(size == 2) {
    Uint16 idx;
    freadLE(idx, model);
    return Uint32(idx);
    }
  else if(size == 4) {
    Uint32 idx;
    freadLE(idx, model);
    return idx;
    }
  else {
    fprintf(stderr, "Bad VarInt Size: %u\n", size);
    exit(1);
    }
  }

string SimpleModel_PMX::ReadString(SDL_RWops *model) const {
  Uint32 len;
  string ret = "";
  freadLE(len, model);
  for(Uint32 i = 0; i < len; ++i) {
    if(text_encoding == 1) {
      Uint8 ch;
      freadLE(ch, model);
      ret += char(ch);
      }
    else if(text_encoding == 0) {
      ++i;
      char ch[2];
      freadLE(ch[0], model);
      freadLE(ch[1], model);
      char *utf8 = (char*)malloc(32);
      memset(utf8, 0, 32);

//    The docs seem to be wrong, these are in UTF-16, not Shift_JIS.  :/
      if (iconv_string("UTF-8", "UTF-16LE",
                       ch, ch+2,
                       &utf8, NULL) < 0)
        perror("iconv_string");
      ret += utf8;
      }
    else {
      fprintf(stderr, "ERROR: Bad text encoding [%u]!\n", text_encoding);
      exit(1);
      }
    }

  // Found some with an extra space in this field - detect and fix that
  if(ret[ret.length()-1] == ' ') ret = ret.substr(0, ret.length()-1);

//  fprintf(stderr, "[%u] '%s'\n", len, ret.c_str());
  return ret;
  }

bool SimpleModel_PMX::Load(const string &filenm,
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
  Sint8 magic[4];
  float filever;

  //Read parts of the header of the PMX file
  SDL_RWread(model, &magic, 1, sizeof(magic));
  if(strncmp((char *)magic, "PMX ", 4)) {
    fprintf(stderr, "WARNING: File '%s' is not a PMX file!\n",
	filename.c_str());
    SDL_RWclose(model);
    return false;
    }
  freadLE(filever, model);
  if(filever != 2.0) {
    fprintf(stderr, "WARNING: File '%s' is not a v2.0 PMX file!\n",
	filename.c_str());
    SDL_RWclose(model);
    return false;
    }
  Uint8 length;
  freadLE(length, model);
  if(length != 8) {
    fprintf(stderr, "WARNING: File '%s' does not have length=8!\n",
	filename.c_str());
    SDL_RWclose(model);
    return false;
    }

  freadLE(text_encoding, model);

  Uint8 appendix_uv;
  freadLE(appendix_uv, model);
  Uint8 vertex_size;
  freadLE(vertex_size, model);
  Uint8 texture_index_size;
  freadLE(texture_index_size, model);
  Uint8 material_size;
  freadLE(material_size, model);
  Uint8 bone_index_size;
  freadLE(bone_index_size, model);
  Uint8 morph_index_size;
  freadLE(morph_index_size, model);
  Uint8 rigid_body_index_size;
  freadLE(rigid_body_index_size, model);

  ReadString(model);
  ReadString(model);
  ReadString(model);
  ReadString(model);

  Uint32 num_vertices;
  freadLE(num_vertices, model);

  vertices.resize(num_vertices);
  for(Uint32 vert = 0; vert < num_vertices; ++vert) {
    float x, y, z;

    // Location vector
    freadLE(y, model);
    freadLE(z, model);
    freadLE(x, model);

    // Normalize to SimpleModel proportions and axes
    vertices[vert].vertex[0] = x / -8.0;
    vertices[vert].vertex[1] = y / -8.0;
    vertices[vert].vertex[2] = z / 8.0;

    // Normal vector
    freadLE(y, model);
    freadLE(z, model);
    freadLE(x, model);

    // Normalize to SimpleModel axes
    vertices[vert].normal[0] = -x;
    vertices[vert].normal[1] = -y;
    vertices[vert].normal[2] = z;

    // Texture coordinates
    freadLE(vertices[vert].texture[0], model);
    freadLE(vertices[vert].texture[1], model);

    Uint8 weight_type;
    freadLE(weight_type, model);

    if(weight_type == 0) {
      vertices[vert].bone[0] = ReadVarInt(model, bone_index_size);
      vertices[vert].bone[1] = 0;
      vertices[vert].bone_weight = 1.0;
      }
    else if(weight_type == 1) {
      vertices[vert].bone[0] = ReadVarInt(model, bone_index_size);
      vertices[vert].bone[1] = ReadVarInt(model, bone_index_size);
      freadLE(vertices[vert].bone_weight, model);
      }
    else if(weight_type == 2) {
      vertices[vert].bone[0] = ReadVarInt(model, bone_index_size);
      vertices[vert].bone[1] = ReadVarInt(model, bone_index_size);
      vertices[vert].bone_weight = 0.5;
      SDL_RWseek(model, bone_index_size, SEEK_CUR);
      SDL_RWseek(model, bone_index_size, SEEK_CUR);
      SDL_RWseek(model, 4, SEEK_CUR);
      SDL_RWseek(model, 4, SEEK_CUR);
      SDL_RWseek(model, 4, SEEK_CUR);
      SDL_RWseek(model, 4, SEEK_CUR);
      }
    else if(weight_type == 3) {
      vertices[vert].bone[0] = ReadVarInt(model, bone_index_size);
      vertices[vert].bone[1] = ReadVarInt(model, bone_index_size);
      vertices[vert].bone_weight = 0.5;
      SDL_RWseek(model, 4, SEEK_CUR);

      SDL_RWseek(model, 4, SEEK_CUR);
      SDL_RWseek(model, 4, SEEK_CUR);
      SDL_RWseek(model, 4, SEEK_CUR);

      SDL_RWseek(model, 4, SEEK_CUR);
      SDL_RWseek(model, 4, SEEK_CUR);
      SDL_RWseek(model, 4, SEEK_CUR);

      SDL_RWseek(model, 4, SEEK_CUR);
      SDL_RWseek(model, 4, SEEK_CUR);
      SDL_RWseek(model, 4, SEEK_CUR);
      }
    else {
      fprintf(stderr, "ERROR: Unknown weight_type: %u\n", weight_type);
      exit(1);
      }

    SDL_RWseek(model, 4, SEEK_CUR);
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
      triangles[tri].vertex[vert] = ReadVarInt(model, vertex_size);
      }
    }

  Uint32 num_textures;
  freadLE(num_textures, model);
  texture.resize(num_textures);
  for(Uint32 tex = 0; tex < num_textures; ++tex) {
    string texfile_part = ReadString(model);
    if(texfile_part.length() < 3) {
      fprintf(stderr, "WARNING: Blank tex '%s'\n", texfile_part.c_str());
      texture[tex] = NULL;
      continue;
      }
    string texfile = filename.substr(0, filename.find_last_of("/") + 1);
    texfile += texfile_part;
    SimpleTexture *tmptex = new SimpleTexture(texfile.c_str());
    if(tmptex->type != SIMPLETEXTURE_NONE) {
      texture[tex] = tmptex;
      }
    else {
      fprintf(stderr, "WARNING: Failed to load tex '%s'\n", texfile.c_str());
      texture[tex] = NULL;
      }
    }

  Uint32 num_materials;
  freadLE(num_materials, model);
  material.resize(num_materials);
  for(Uint32 mat = 0; mat < num_materials; ++mat) {
    ReadString(model); // Name (Japanese)
    ReadString(model); // Name (English)

    freadLE(material[mat].diffuse[0], model);
    freadLE(material[mat].diffuse[1], model);
    freadLE(material[mat].diffuse[2], model);
    freadLE(material[mat].diffuse[3], model);

    freadLE(material[mat].specular[0], model);
    freadLE(material[mat].specular[1], model);
    freadLE(material[mat].specular[2], model);

    freadLE(material[mat].specularity, model);

    freadLE(material[mat].ambient[0], model);
    freadLE(material[mat].ambient[1], model);
    freadLE(material[mat].ambient[2], model);

    freadLE(material[mat].mode, model); // Drawing Mode

    SDL_RWseek(model, 16, SEEK_CUR); // Edge Color
    SDL_RWseek(model, 4, SEEK_CUR); // Edge Size

    material[mat].texidx = ReadVarInt(model, texture_index_size);

    SDL_RWseek(model, texture_index_size, SEEK_CUR); // Sphere Index
    SDL_RWseek(model, 1, SEEK_CUR); // Sphere Mode

    Uint8 toon;
    freadLE(toon, model); // Toon Flag
    if(toon == 0) {
      SDL_RWseek(model, texture_index_size, SEEK_CUR); // Toon Index
      }
    else {
      SDL_RWseek(model, 1, SEEK_CUR); // Toon Index
      }

    ReadString(model);

    freadLE(material[mat].num_tris, model);
    material[mat].num_tris /= 3;
    }

  // This section is not in the doc I have.  Using the force here.
  Uint32 num_bones;
  freadLE(num_bones, model);
  bone.resize(num_bones);
  for(Uint32 bn = 0; bn < num_bones; ++bn) {
    bone[bn].name = ReadString(model); // Name
    string name = ReadString(model); // English name (usually blank)
    freadLE(bone[bn].pos.data[0], model);
    freadLE(bone[bn].pos.data[1], model);
    freadLE(bone[bn].pos.data[2], model);
    bone[bn].parent = ReadVarInt(model, bone_index_size);
    SDL_RWseek(model, 4, SEEK_CUR); // "Layer"?
    freadLE(bone[bn].flags, model);
    if(bone[bn].flags & PMX_BONE_FLAG_TAILPOS_IS_BONE) {
      ReadVarInt(model, bone_index_size); // "Tail"?
      }
    else {
      SDL_RWseek(model, 12, SEEK_CUR); // "Tail (Vector3)"?
      }
    if(bone[bn].flags & PMX_BONE_FLAG_IS_EXTERNAL_ROTATION) {
      ReadVarInt(model, bone_index_size); // "Effector"?
      SDL_RWseek(model, 4, SEEK_CUR); // "Factor (float)"?
      }
    if(bone[bn].flags & PMX_BONE_FLAG_HAS_FIXED_AXIS) {
      SDL_RWseek(model, 12, SEEK_CUR); // "Fixed Axis (Vector3)"?
      }
    if(bone[bn].flags & PMX_BONE_FLAG_HAS_LOCAL_COORDINATE) {
      SDL_RWseek(model, 12, SEEK_CUR); // "X Vector (Vector3)"?
      SDL_RWseek(model, 12, SEEK_CUR); // "Z Vector (Vector3)"?
      }
    if(bone[bn].flags & PMX_BONE_FLAG_IS_EXTERNAL_PARENT_DEFORM) {
      SDL_RWseek(model, 4, SEEK_CUR); // "External Key (Uint32)"?
      }
    if(bone[bn].flags & PMX_BONE_FLAG_IS_IK) {
      ReadVarInt(model, bone_index_size); // "IK Info (?)"?
      SDL_RWseek(model, 8, SEEK_CUR); // "IK Info (?)"?
      Uint32 num_ik_links;
      freadLE(num_ik_links, model);
      for(Uint32 ik_link = 0; ik_link < num_ik_links; ++ik_link) {
        ReadVarInt(model, bone_index_size); // "IK Link Info (?)"?
        Uint8 ik_limit_angle;
        freadLE(ik_limit_angle, model); // ?
        if(ik_limit_angle == 1) {
          SDL_RWseek(model, 24, SEEK_CUR); // "min/max (Vector3 x 2)"?
          }
        }
      }
    }

  return false;
  }

bool SimpleModel_PMX::RenderSelf(Uint32 cur_time, const vector<int> &anim,
	const vector<Uint32> &start_time, Uint32 anim_offset) const {

  Uint32 mat = -1;
  Uint32 to_next_mat = 0;
  float xfact = 1.0, yfact = 1.0;

  for(Uint32 tri = 0; tri < triangles.size(); tri++) {
    if(tri >= to_next_mat) {
      do {
        ++mat;
        to_next_mat = tri + material[mat].num_tris;
        } while(tri >= to_next_mat);
      if(tri > 0) glEnd();

      if(MaterialDisabled(mat)) continue;

      Uint32 tex = material[mat].texidx;
      if(tex == 255 || !texture[tex]) {
        glDisable(GL_TEXTURE);
        xfact = 1.0;
        yfact = 1.0;
        }
      else {
        glEnable(GL_TEXTURE);
        glBindTexture(GL_TEXTURE_2D, texture[tex]->GLTexture());
        xfact = texture[tex]->xfact;
        yfact = texture[tex]->yfact;
        }
      if(material[mat].mode & 0x01) {
        glDisable(GL_CULL_FACE);
        }
      else {
        glEnable(GL_CULL_FACE);
        }

      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material[mat].ambient);
      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material[mat].diffuse);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material[mat].specular);
      glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material[mat].specularity);

      glBegin(GL_TRIANGLES);
      }
    for(Uint32 vert = 0; vert < 3; ++vert) {
      glTexCoord2f(vertices[triangles[tri].vertex[vert]].texture[0] * xfact,
                   vertices[triangles[tri].vertex[vert]].texture[1] * yfact);

      glNormal3f(vertices[triangles[tri].vertex[vert]].normal[0],
                 vertices[triangles[tri].vertex[vert]].normal[1],
                 vertices[triangles[tri].vertex[vert]].normal[2]);

      glVertex3f(vertices[triangles[tri].vertex[vert]].vertex[0],
                 vertices[triangles[tri].vertex[vert]].vertex[1],
                 vertices[triangles[tri].vertex[vert]].vertex[2]);
      }
    }

  if(mat >= 0) glEnd();

  return true;
  }
