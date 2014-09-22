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
    float x, y, z;

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
      material[mat].texture = NULL;
      continue;
      }
    if(texfile_part.find('*') != string::npos) {
      texfile_part = texfile_part.substr(0, texfile_part.find('*'));
      // FIXME: Ignoring sphere part (after the '*')
      }
    if(texfile_part.length() < 3) {
      fprintf(stderr, "WARNING: Blank base tex '%s'\n", texfile_part.c_str());
      material[mat].texture = NULL;
      continue;
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

    freadLE(bone[bn].parent, model);
    freadLE(bone[bn].child, model);
    freadLE(bone[bn].type, model);
    freadLE(bone[bn].target, model);

    freadLE(bone[bn].pos[0], model);
    freadLE(bone[bn].pos[1], model);
    freadLE(bone[bn].pos[2], model);
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

  Uint32 num_keyframes;
  freadLE(num_keyframes, model);
  for(Uint32 kf = 0; kf < num_keyframes; ++kf) {
    string bone_name = ReadString(model, 15);

    Uint32 frame;
    freadLE(frame, model);

    if(bone_by_name.count(bone_name) != 1) {
      fprintf(stderr, "Discarding frame %u data for unknown bone (%s).\n",
              frame, bone_name.c_str());
      SDL_RWseek(model, 64+28, SEEK_CUR);
      continue;
      }

    //fprintf(stderr, "Keeping frame %u data for known bone (%s).\n",
    //        frame, bone_name.c_str());

    Uint16 bone = bone_by_name[bone_name];

    // Coordinate vector
    freadLE(keyframe[bone][frame].pos[0], model);
    freadLE(keyframe[bone][frame].pos[1], model);
    freadLE(keyframe[bone][frame].pos[2], model);

    // Rotation quaternion
    freadLE(keyframe[bone][frame].rot.x, model);
    freadLE(keyframe[bone][frame].rot.y, model);
    freadLE(keyframe[bone][frame].rot.z, model);
    freadLE(keyframe[bone][frame].rot.w, model);

    // TODO: Lots of unknown data ignored
    SDL_RWseek(model, 64, SEEK_CUR);
    }
  return false;
  }

bool SimpleModel_PMD::RenderSelf(Uint32 cur_time, const vector<int> &anim,
	const vector<Uint32> &start_time, Uint32 anim_offset) const {

  float frame = float(cur_time - start_time[0]) * 30.0 / 1000.0;

  Uint32 mat = -1;
  Uint32 to_next_mat = 0;
  float xfact = 1.0, yfact = 1.0;

  // Transform to SimpleModel axes and scale
  glPushMatrix();
  glScalef(0.125, 0.125, 0.125);
  glRotatef(-90.0, 0.0, 1.0, 0.0);
  glRotatef(-90.0, 0.0, 0.0, 1.0);

  float bone_off[bone.size()][3];
  Quaternion bone_rot[bone.size()];
  for(Uint32 mat = 0; mat < bone.size(); ++mat) {
    bone_off[mat][0] = 0.0;
    bone_off[mat][1] = 0.0;
    bone_off[mat][2] = 0.0;
    bone_rot[mat].x = 0.0;
    bone_rot[mat].y = 0.0;
    bone_rot[mat].z = 0.0;
    bone_rot[mat].w = 1.0;
    }

  for(auto bn=keyframe.begin(); bn != keyframe.end(); ++bn) {
    Uint16 bone_id = bn->first;
    Uint32 last = 0;
    Quaternion q1 = {1.0, 0.0, 0.0, 0.0}, q2, rot;
    float x = 0.0, y = 0.0, z = 0.0;
    for(auto fr=bn->second.begin(); fr != bn->second.end(); ++fr) {
      if(fr->first <= frame) {
        x = fr->second.pos[0];
        y = fr->second.pos[1];
        z = fr->second.pos[2];

        q1 = fr->second.rot;

        last = fr->first;
        }
      else {
        float weight = float(frame - last) / float(fr->first - last);
        q2 = fr->second.rot;
        SLERP(rot, q1, q2, weight);

        x = x * (1.0 - weight) + weight * fr->second.pos[0];
        y = y * (1.0 - weight) + weight * fr->second.pos[1];
        z = z * (1.0 - weight) + weight * fr->second.pos[2];

        while(bone_id != 0) {
          CombineQuaternions(bone_rot[bone_id], rot, bone_rot[bone_id]);

          bone_off[bone_id][0] += x;
          bone_off[bone_id][1] += y;
          bone_off[bone_id][2] += z;

          // TODO: It's way more complicated than this.  :)
          bone_id = bone[bone_id].child;
          }
        break;
        }
      }
    }

  glDisable(GL_CULL_FACE);
  for(Uint32 tri = 0; tri < triangles.size(); tri++) {
    if(tri >= to_next_mat) {
      do {
        ++mat;
        to_next_mat = tri + material[mat].num_tris;
        } while(tri >= to_next_mat);
      if(tri > 0) {
        glEnd();
        glPopMatrix();
        }

      if(MaterialDisabled(mat)) continue;

      if(!material[mat].texture) {
        glDisable(GL_TEXTURE);
        xfact = 1.0;
        yfact = 1.0;
        }
      else {
        glEnable(GL_TEXTURE);
        glBindTexture(GL_TEXTURE_2D, material[mat].texture->GLTexture());
        xfact = material[mat].texture->xfact;
        yfact = material[mat].texture->yfact;
        }

      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material[mat].ambient);
      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material[mat].diffuse);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material[mat].specular);
      glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material[mat].specularity);

      glPushMatrix();

/*
      float bone_weight = vertices[triangles[tri].vertex[0]].bone_weight;
      Quaternion rot_quat, q1, q2;
      q1 = bone_rot[vertices[triangles[tri].vertex[0]].bone[0]];
      q2 = bone_rot[vertices[triangles[tri].vertex[0]].bone[1]];

// NLERP
      rot_quat.w = q1.w * bone_weight + q2.w * (1.0 - bone_weight);
      rot_quat.x = q1.x * bone_weight + q2.x * (1.0 - bone_weight);
      rot_quat.y = q1.y * bone_weight + q2.y * (1.0 - bone_weight);
      rot_quat.z = q1.z * bone_weight + q2.z * (1.0 - bone_weight);
      Normalize(rot_quat, rot_quat);

// SLERP
//      SLERP(rot_quat, q1, q2, bone_weight);

// USE JUST 1
//      rot_quat = bone_rot[vertices[triangles[tri].vertex[0]].bone[0]];


      Matrix4x4 rot_mat;
      QuaternionToMatrix4x4(rot_mat, rot_quat);

      glTranslatef(bone[vertices[triangles[tri].vertex[0]].bone[0]].pos[0],
                   bone[vertices[triangles[tri].vertex[0]].bone[0]].pos[1],
                   bone[vertices[triangles[tri].vertex[0]].bone[0]].pos[2]);
      glMultMatrixf(rot_mat.data);
      glTranslatef(-bone[vertices[triangles[tri].vertex[0]].bone[0]].pos[0],
                   -bone[vertices[triangles[tri].vertex[0]].bone[0]].pos[1],
                   -bone[vertices[triangles[tri].vertex[0]].bone[0]].pos[2]);
*/

/*
      float x, y, z;
      x = bone_off[vertices[triangles[tri].vertex[0]].bone[0]][0]
            * vertices[triangles[tri].vertex[0]].bone_weight
        + +bone_off[vertices[triangles[tri].vertex[0]].bone[1]][0]
            * (1.0 - vertices[triangles[tri].vertex[0]].bone_weight);
      y = bone_off[vertices[triangles[tri].vertex[0]].bone[0]][1]
            * vertices[triangles[tri].vertex[0]].bone_weight
        + +bone_off[vertices[triangles[tri].vertex[0]].bone[1]][1]
            * (1.0 - vertices[triangles[tri].vertex[0]].bone_weight);
      z = bone_off[vertices[triangles[tri].vertex[0]].bone[0]][2]
            * vertices[triangles[tri].vertex[0]].bone_weight
        + +bone_off[vertices[triangles[tri].vertex[0]].bone[1]][2]
            * (1.0 - vertices[triangles[tri].vertex[0]].bone_weight);
      glTranslatef(x, y, z);
*/

      glBegin(GL_TRIANGLES);
      }
    for(Uint32 vert = 0; vert < 3; ++vert) {
      glTexCoord2f(vertices[triangles[tri].vertex[vert]].texture[0] * xfact,
                   vertices[triangles[tri].vertex[vert]].texture[1] * yfact);

      glNormal3f(vertices[triangles[tri].vertex[vert]].normal[0],
                 vertices[triangles[tri].vertex[vert]].normal[1],
                 vertices[triangles[tri].vertex[vert]].normal[2]);

      float xoff, yoff, zoff, bone_weight, x, y, z;
      bone_weight = vertices[triangles[tri].vertex[vert]].bone_weight;
      Uint16 bone1 = vertices[triangles[tri].vertex[vert]].bone[0];
      Uint16 bone2 = vertices[triangles[tri].vertex[vert]].bone[1];

      Quaternion rot_quat, q1, q2;
      q1 = bone_rot[bone1];
      q2 = bone_rot[bone2];

// NLERP
      rot_quat.w = q1.w * bone_weight + q2.w * (1.0 - bone_weight);
      rot_quat.x = q1.x * bone_weight + q2.x * (1.0 - bone_weight);
      rot_quat.y = q1.y * bone_weight + q2.y * (1.0 - bone_weight);
      rot_quat.z = q1.z * bone_weight + q2.z * (1.0 - bone_weight);
      Normalize(rot_quat, rot_quat);

// SLERP
//      SLERP(rot_quat, q1, q2, bone_weight);

// USE JUST 1
//      rot_quat = bone_rot[vertices[triangles[tri].vertex[0]].bone[0]];


//      xpos = bone[bone1].pos[0] * bone_weight
//           + bone[bone2].pos[0] * (1.0 - bone_weight);
      xoff = bone_off[bone1][0] * bone_weight
           + bone_off[bone2][0] * (1.0 - bone_weight);
//      ypos = bone[bone1].pos[1] * bone_weight
//           + bone[bone2].pos[1] * (1.0 - bone_weight);
      yoff = bone_off[bone1][1] * bone_weight
           + bone_off[bone2][1] * (1.0 - bone_weight);
//      zpos = bone[bone1].pos[2] * bone_weight
//           + bone[bone2].pos[2] * (1.0 - bone_weight);
      zoff = bone_off[bone1][2] * bone_weight
           + bone_off[bone2][2] * (1.0 - bone_weight);

      x = vertices[triangles[tri].vertex[vert]].vertex[0] + xoff;
      y = vertices[triangles[tri].vertex[vert]].vertex[1] + yoff;
      z = vertices[triangles[tri].vertex[vert]].vertex[2] + zoff;

// TODO: Working on this
//      x -= xpos;
//      y -= ypos;
//      z -= zpos;

      QuaternionRotate(x, y, z, rot_quat);

      glVertex3f(x, y, z);
      }
    }

  if(mat >= 0) {
    glEnd();
    glPopMatrix();
    }

  glPopMatrix();

  return true;
  }
