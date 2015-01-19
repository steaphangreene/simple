// *************************************************************************
//  This file is part of the SimpleModel Example Module by Steaphan Greene
//
//  Copyright 2005-2015 Steaphan Greene <steaphan@gmail.com>
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

#include "simplemodel_q3dir.h"
#include "sm_q3anim.h"
#include "saferead.h"

SimpleModel_MD3::SimpleModel_MD3(const string &filenm, const string &modelnm,
                                 const string &defskin) {
  Load(filenm, modelnm, defskin);
}

SimpleModel_MD3::SimpleModel_MD3() {}

SimpleModel_MD3::~SimpleModel_MD3() {}

bool SimpleModel_MD3::Load(const string &filenm, const string &modelnm,
                           const string &defskin) {
  filename = filenm;
  modelname = modelnm;
  if (modelnm.substr(0, filenm.length()) != filenm) {
    // New format, no duplicated lines in file.
    modelname = filename + "/" + modelnm;
  } else {
    // Old format, duplicated lines in file -OR- zero-length filenm.
    filename = ".";
  }

  skinname = defskin;
  skinname =
      modelname.substr(0, modelname.length() - 4) + "_" + skinname + ".skin";

  SDL_RWops *model = SDL_RWFromZZIP(modelname.c_str(), "rb");
  if (!model) {
    fprintf(stderr, "WARNING: Unable to open model file '%s'!\n",
            modelname.c_str());
    perror("WARNING");
    return false;
  }

  bool external_skinfile = false;

  // These are the parts of the header we're actually reading.
  Sint8 magic[4];
  Sint32 filever;
  Uint32 num_frames;

  // Read parts of the header of the MD3 file
  SDL_RWread(model, &magic, 1, sizeof(magic));
  if (strncmp((char *)magic, "IDP3", 4)) {
    fprintf(stderr, "WARNING: File '%s' is not an MD3 file!\n",
            modelnm.c_str());
    SDL_RWclose(model);
    return false;
  }
  freadLE(filever, model);
  if (filever != 15) {
    fprintf(stderr, "WARNING: File '%s' is not an v15 MD3 file!\n",
            modelnm.c_str());
    SDL_RWclose(model);
    return false;
  }
  SDL_RWseek(model, 68, SEEK_CUR);  // Skip the name of the file

  freadLE(num_frames, model);
  freadLE(num_tags, model);
  Uint32 num_meshes;
  freadLE(num_meshes, model);
  meshes.resize(num_meshes);

  SDL_RWseek(model, 20,
             SEEK_CUR);  // Skip the rest of the header (FIXME: Skins!)

  // FIXME: This skips the bones, do we want to actually use them?
  SDL_RWseek(model, 56 * num_frames, SEEK_CUR);

  pTags.resize(num_frames * num_tags);
  for (unsigned int tag = 0; tag < num_frames * num_tags; ++tag) {
    SDL_RWread(model, pTags[tag].name, 64,
               1);  // FIXME: Don't need this anymore!
    if (tag < num_tags) tags[(char *)pTags[tag].name] = tag;
    freadLE(pTags[tag].pos.data[12], model);  // Location vector
    freadLE(pTags[tag].pos.data[13], model);  // Location vector
    freadLE(pTags[tag].pos.data[14], model);  // Location vector
    pTags[tag].pos.data[15] = 1.0;
    freadLE(pTags[tag].pos.data[0], model);  // Rotation matrix
    freadLE(pTags[tag].pos.data[1], model);  // Rotation matrix
    freadLE(pTags[tag].pos.data[2], model);  // Rotation matrix
    pTags[tag].pos.data[3] = 0.0;
    freadLE(pTags[tag].pos.data[4], model);  // Rotation matrix
    freadLE(pTags[tag].pos.data[5], model);  // Rotation matrix
    freadLE(pTags[tag].pos.data[6], model);  // Rotation matrix
    pTags[tag].pos.data[7] = 0.0;
    freadLE(pTags[tag].pos.data[8], model);   // Rotation matrix
    freadLE(pTags[tag].pos.data[9], model);   // Rotation matrix
    freadLE(pTags[tag].pos.data[10], model);  // Rotation matrix
  }

  // Get the current offset into the file
  long offset = SDL_RWtell(model);

  // Go through all of the sub-objects in this mesh
  for (unsigned int meshnum = 0; meshnum < meshes.size(); meshnum++) {
    // Seek to the start of this mesh and read in it's header
    SDL_RWseek(model, offset + 4, SEEK_SET);  // I'm skipping the meshID here

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
    for (unsigned int shader = 0; shader < num_shaders; ++shader) {
      Sint8 buf[65] = {0};
      SDL_RWread(model, buf, 64, 1);
      if (strlen((char *)buf) > 0) {
        for (int ch = 0; ch < 64; ++ch)
          if (buf[ch] == '\\') buf[ch] = '/';
        AddSkin((char *)buf);
        if (skins.back().length() > 0) {
          SimpleTexture *tmptex =
              new SimpleTexture(filename + "/" + skins.back());
          if (tmptex->type != SIMPLETEXTURE_NONE) {
            texture.push_back(tmptex);
            meshes[meshnum].texture = texture.back();
          } else {
            delete tmptex;
            skins.pop_back();  // FIXME: Create RemoveSkin() function?
            external_skinfile = true;
          }
        }
      } else {
        external_skinfile = true;
      }
    }

    // Seek to the start of the face data, then read it in
    SDL_RWseek(model, offset + face_offset, SEEK_SET);
    for (unsigned int face = 0; face < meshes[meshnum].faces.size(); ++face) {
      freadLE(meshes[meshnum].faces[face].vertices[0], model);
      freadLE(meshes[meshnum].faces[face].vertices[1], model);
      freadLE(meshes[meshnum].faces[face].vertices[2], model);
    }

    // Seek to the start of the texture coordinate data, then read it in
    SDL_RWseek(model, offset + uv_offset, SEEK_SET);
    for (unsigned int vert = 0; vert < meshes[meshnum].coords.size(); ++vert) {
      freadLE(meshes[meshnum].coords[vert].coord[0], model);
      freadLE(meshes[meshnum].coords[vert].coord[1], model);
    }

    // Seek to the start of the triangle information, then read it in.
    SDL_RWseek(model, offset + vertex_offset, SEEK_SET);
    for (unsigned int vert = 0; vert < meshes[meshnum].triangles.size();
         ++vert) {
      Sint16 tmp = 0;
      freadLE(tmp, model);
      meshes[meshnum].triangles[vert].vertex[0] = float(tmp) / 64.0F;
      freadLE(tmp, model);
      meshes[meshnum].triangles[vert].vertex[1] = float(tmp) / 64.0F;
      freadLE(tmp, model);
      meshes[meshnum].triangles[vert].vertex[2] = float(tmp) / 64.0F;

      Uint8 norm[2];
      SDL_RWread(model, norm, 1, 2);

      float lat = norm[0] * (2.0 * M_PI) / 255.0;
      float lng = norm[1] * (2.0 * M_PI) / 255.0;
      meshes[meshnum].triangles[vert].normal[0] = cos(lng) * sin(lat);
      meshes[meshnum].triangles[vert].normal[1] = sin(lng) * sin(lat);
      meshes[meshnum].triangles[vert].normal[2] = cos(lat);
    }

    // Point offset to the next mesh location in the file
    offset += mesh_size;
  }
  SDL_RWclose(model);

  // We have all our skins, no need for a .skin file!
  if (!external_skinfile) return false;

  // Here we load the requested skin (FIXME: without checking the skin list!)
  SDL_RWops *skin = SDL_RWFromZZIP(skinname.c_str(), "rb");
  if (!skin) {
    fprintf(stderr, "WARNING: Unable to open skin file '%s'!\n",
            skinname.c_str());
    perror("WARNING");
    return false;
  }

  SDL_RWseek(skin, 0, SEEK_END);
  int filesz = SDL_RWtell(skin);
  SDL_RWseek(skin, 0, SEEK_SET);
  char *filedata = new char[filesz + 1];  // FIXME: Handle Error!
  char *fileptr = filedata;
  while (fileptr < (filedata + filesz)) {
    int res = SDL_RWread(skin, fileptr, 1, filesz);
    if (res <= 0) {
      fprintf(stderr, "%d\n", filesz);
      fprintf(stderr, "ERROR: Could not read from '%s'\n", skinname.c_str());
      SDL_RWclose(skin);
      return false;
    }
    fileptr += res;
  }
  *fileptr = 0;
  SDL_RWclose(skin);
  fileptr = filedata;

  Sint8 obj[256] = {0}, tex[256] = {0};
  while (1) {
    int num = sscanf(fileptr, " %[^ \n\t\r,],%[^ \n\t\r]\n", obj, tex);
    for (int ch = 0; ch < 256; ++ch)
      if (tex[ch] == '\\') tex[ch] = '/';

    // Skip to past next whitespace
    while ((*fileptr) && !isspace(*fileptr)) ++fileptr;
    while ((*fileptr) && isspace(*fileptr)) ++fileptr;

    if (num < 1)
      break;
    else if (num == 1) {
      // FIXME: What do these lines mean?  Don't know what to do here.
    } else {
      for (unsigned int i = 0; i < meshes.size(); i++) {
        if (!strcasecmp((char *)obj, (char *)meshes[i].name)) {
          SimpleTexture *tmptex =
              new SimpleTexture(filename + "/" + (char *)tex);
          texture.push_back(tmptex);

          tmptex->Update();

          meshes[i].texture = texture.back();
        }
      }
    }
    memset((char *)obj, 0, 256);
    memset((char *)tex, 0, 256);
  }

  delete[] filedata;
  return false;
}

bool SimpleModel_MD3::RenderSelf(Uint32 cur_time, const vector<int> &anim,
                                 const vector<Uint32> &start_time,
                                 Uint32 anim_offset) const {
  glCullFace(GL_FRONT);  // MD3 models use front face culling

  float s2;
  int frame = 0, next = 0;
  int animation = anim[anim_offset];
  if (animation >= LEGS_START) animation -= (LEGS_START - TORSO_START);

  if (anim.size() >= anim_offset) {
    frame = CalcBaseFrame(cur_time, animation, start_time[anim_offset], s2);
    frame = NormalizeFrame(animation, frame);
    next = NormalizeFrame(animation, frame + 1);
  }

  for (unsigned int obj = 0; obj < meshes.size(); ++obj) {
    if (meshes[obj].texture &&
        meshes[obj].texture->type != SIMPLETEXTURE_NONE) {
      glBindTexture(GL_TEXTURE_2D, meshes[obj].texture->GLTexture());

      glBegin(GL_TRIANGLES);

      int vertindex = frame * meshes[obj].coords.size();
      int vertindex2 = next * meshes[obj].coords.size();

      for (unsigned int j = 0; j < meshes[obj].faces.size(); j++) {
        for (int vertex = 0; vertex < 3; vertex++) {
          int index = meshes[obj].faces[j].vertices[vertex];

          glTexCoord2f(meshes[obj].coords[index].coord[0],
                       meshes[obj].coords[index].coord[1]);

          float x1 = meshes[obj].triangles[vertindex + index].vertex[0];
          float y1 = meshes[obj].triangles[vertindex + index].vertex[1];
          float z1 = meshes[obj].triangles[vertindex + index].vertex[2];

          float x2 = meshes[obj].triangles[vertindex2 + index].vertex[0];
          float y2 = meshes[obj].triangles[vertindex2 + index].vertex[1];
          float z2 = meshes[obj].triangles[vertindex2 + index].vertex[2];

          float nx1 = meshes[obj].triangles[vertindex + index].normal[0];
          float ny1 = meshes[obj].triangles[vertindex + index].normal[1];
          float nz1 = meshes[obj].triangles[vertindex + index].normal[2];

          float nx2 = meshes[obj].triangles[vertindex2 + index].normal[0];
          float ny2 = meshes[obj].triangles[vertindex2 + index].normal[1];
          float nz2 = meshes[obj].triangles[vertindex2 + index].normal[2];

          float s1 = (1.0f - s2);

          glNormal3f(s1 * nx1 + s2 * nx2, s1 * ny1 + s2 * ny2,
                     s1 * nz1 + s2 * nz2);
          glVertex3f(s1 * x1 + s2 * x2, s1 * y1 + s2 * y2, s1 * z1 + s2 * z2);
        }
      }
    }
    glEnd();
  }

  glCullFace(GL_BACK);

  return true;
}

int SimpleModel_MD3::AddAnimation(int start, int end, int loop, int fps) {
  MD3AnimationData anim;

  anim.start = start;
  anim.end = end;
  anim.loop = loop;
  anim.fps = fps;
  anim_data.push_back(anim);

  return anim_data.size() - 1;
}

bool SimpleModel_MD3::MoveToTag(Uint32 tag, Uint32 cur_time,
                                const vector<int> &anim,
                                const vector<Uint32> &start_time,
                                Uint32 anim_offset) const {
  if (tag >= num_tags) return false;

  int animation = anim[anim_offset];
  if (animation >= LEGS_START) animation -= (LEGS_START - TORSO_START);

  float s;
  int frame = 0, next = 0;
  if (anim.size() >= anim_offset) {
    frame = CalcBaseFrame(cur_time, animation, start_time[anim_offset], s);
    frame = NormalizeFrame(animation, frame);
    next = NormalizeFrame(animation, frame + 1);
  }

  Matrix4x4 matrix;
  SLERP(matrix, pTags[frame * num_tags + tag].pos,
        pTags[next * num_tags + tag].pos, s);
  glMultMatrixf(matrix.data);

  return true;
}

int SimpleModel_MD3::CalcBaseFrame(Uint32 cur_time, int anim, Uint32 start_time,
                                   float &offset) const {
  int frame = 0;
  offset = 0.0;
  if (anim_data.size() > 0) {
    int start = anim_data[anim].start;
    float fps = anim_data[anim].fps;
    float elapsed = cur_time - start_time;

    float disp = elapsed * fps / 1000.0;
    frame = start + (int)(disp);
    offset = disp - floorf(disp);
  }
  return frame;
}

int SimpleModel_MD3::NormalizeFrame(int anim, int frame) const {
  if (anim_data.size() > 0) {
    int end = anim_data[anim].end;
    int loop = anim_data[anim].loop;

    if (frame >= end && loop > 0)
      frame = (end - loop) + (frame - end + loop) % loop;
    else if (frame >= end)
      frame = end - 1;
  } else {
    frame = 0;
  }
  return frame;
}

const vector<string> &SimpleModel_MD3::GetSkinList() { return skins; }

void SimpleModel_MD3::AddSkin(const string &skinnm) {
  if (skinnm.length() > 4) {
    string suffix = skinnm.substr(skinnm.length() - 4);
    if (suffix == ".tga" || suffix == ".TGA") {
      skins.push_back(skinnm.substr(0, skinnm.length() - 4));
      return;
    }
  }
  skins.push_back(skinnm);
}
