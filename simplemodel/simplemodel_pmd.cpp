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

  string ret;
  char *utf8 = NULL;
  if (iconv_string("UTF-8", "Shift_JIS", ch, ch+len, &utf8, NULL) < 0) {
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

    // Skip the bones for now
    SDL_RWseek(model, 2, SEEK_CUR);
    SDL_RWseek(model, 2, SEEK_CUR);
    SDL_RWseek(model, 1, SEEK_CUR);

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
    if(texfile_part.find('*') != string::npos) {
      texfile_part = texfile_part.substr(0, texfile_part.find('*'));
      // FIXME: Ignoring sphere part (after the '*')
      }
    string texfile = filename.substr(0, filename.find_last_of("/") + 1);
    texfile += texfile_part;
    SimpleTexture *tmptex = new SimpleTexture(texfile.c_str());
    if(tmptex->type != SIMPLETEXTURE_NONE) {
      material[mat].texture = tmptex;
      }
    else {
      fprintf(stderr, "WARNING: Failed to load tex '%s'\n", texfile.c_str());
      material[mat].texture = NULL;
      }

    }
  return false;
  }

bool SimpleModel_PMD::RenderSelf(Uint32 cur_time, const vector<int> &anim,
	const vector<Uint32> &start_time, Uint32 anim_offset) const {

  Uint32 mat = -1;
  Uint32 to_next_mat = 0;

  glDisable(GL_CULL_FACE);
  for(Uint32 tri = 0; tri < triangles.size(); tri++) {
    if(tri >= to_next_mat) {
      do {
        ++mat;
        to_next_mat = tri + material[mat].num_tris;
        } while(tri >= to_next_mat);
      if(tri > 0) glEnd();

      if(MaterialDisabled(mat)) continue;

      if(!material[mat].texture) {
        glDisable(GL_TEXTURE);
        }
      else {
        glEnable(GL_TEXTURE);
        glBindTexture(GL_TEXTURE_2D, material[mat].texture->GLTexture());
        }

      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material[mat].ambient);
      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material[mat].diffuse);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material[mat].specular);
      glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material[mat].specularity);

      glBegin(GL_TRIANGLES);
      }
    for(Uint32 vert = 0; vert < 3; ++vert) {
      glTexCoord2f(vertices[triangles[tri].vertex[vert]].texture[0],
                   vertices[triangles[tri].vertex[vert]].texture[1]);

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
