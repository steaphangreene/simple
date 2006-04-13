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
#include <iostream>
using namespace std;

#include "simplemodel_md.h"
#include "saferead.h"

SimpleModel_MD::SimpleModel_MD(const string &filenm) {
  }

SimpleModel_MD::SimpleModel_MD() {
  mat_ident = identity4x4;
  }

SimpleModel_MD::~SimpleModel_MD() {
  }

bool SimpleModel_MD::Render(Uint32 cur_time, const vector<int> & anim, const vector<Uint32> & start_time) const {
  TransformInfo cur_transforms;
  AnimationInfo anim_info;
  float factor;  

  anim_info.cur_frame = CalcBaseFrame(cur_time, anim, start_time, factor);
  anim_info.cur_frame = NormalizeFrame(anim, anim_info.cur_frame);
  anim_info.cur_seq = sequences.begin() + anim.at(0);
  anim_info.interpolation_weight = factor;

  cur_transforms.bone_transforms.resize(bones.size());
  cur_transforms.geoset_matrices.resize(geosets.at(0).group_counts.size());

  //fprintf(stderr, "Start frame: %d\n", sequences.at(anim[0]).start);
  //fprintf(stderr, "End frame: %d\n", sequences.at(anim[0]).end);
  //fprintf(stderr, "Frame: %d.\n", anim_info.cur_frame);

  CalcTransforms(cur_transforms, mat_ident, -1, anim_info);
  geosets.at(0).CalculateGroupMatrices(cur_transforms);
 
  MDXVertex vert;
  for(Uint32 i = 0; i < geosets.at(0).indices.size(); i += 3) {
    Uint32 v1 = geosets.at(0).indices.at(i);
    Uint32 v2 = geosets.at(0).indices.at(i + 1);
    Uint32 v3 = geosets.at(0).indices.at(i + 2);

    MDXVertex vec1 = geosets.at(0).vertices.at(v1);
    MDXVertex vec2 = geosets.at(0).vertices.at(v2);
    MDXVertex vec3 = geosets.at(0).vertices.at(v3);

    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);

    glColor3f(1.0f, 1.0f, 1.0f);

    glBindTexture(GL_TEXTURE_2D, texture.at(geosets.at(0).texture_id)->GLTexture());
    glBegin(GL_TRIANGLES);
      Uint32 mindex = geosets.at(0).vertex_groups.at(v1);
      MatVecMult(vert, cur_transforms.geoset_matrices.at(mindex), vec1);  
      glTexCoord2f(geosets.at(0).texture_coords_uvas.at(0).at(v1).coord[0], geosets.at(0).texture_coords_uvas.at(0).at(v1).coord[1]);
      glVertex3f(vert.x, vert.y, vert.z);

      mindex = geosets.at(0).vertex_groups.at(v2);
      MatVecMult(vert, cur_transforms.geoset_matrices.at(mindex), vec2);  
      glTexCoord2f(geosets.at(0).texture_coords_uvas.at(0).at(v2).coord[0], geosets.at(0).texture_coords_uvas.at(0).at(v2).coord[1]);
      glVertex3f(vert.x, vert.y, vert.z);

      mindex = geosets.at(0).vertex_groups.at(v3);
      MatVecMult(vert, cur_transforms.geoset_matrices.at(mindex), vec3);  
      glTexCoord2f(geosets.at(0).texture_coords_uvas.at(0).at(v3).coord[0], geosets.at(0).texture_coords_uvas.at(0).at(v3).coord[1]);
      glVertex3f(vert.x, vert.y, vert.z);
    glEnd();
    }
  return true;
  }

void SimpleModel_MD::MDXGeoset::CalculateGroupMatrices(TransformInfo & current_transforms) const {
  Uint32 index = 0;
    
  for(Uint32 i = 0; i < group_counts.size(); ++i) {
      AccumulateBoneTransforms(current_transforms, i, group_counts.at(i), index);
      index += group_counts.at(i);
  }
}

void SimpleModel_MD::MDXGeoset::AccumulateBoneTransforms(TransformInfo & cur_trans, const Uint32 m_id, const Uint32 m_count, const Uint32 m_start) const {
    for(Uint32 i = 0; i < m_count; ++i) {
      Matrix4x4 temp = {{0.0}};
      Add(temp, cur_trans.geoset_matrices.at(m_id), cur_trans.bone_transforms.at(matrices.at(m_start + i)));
      cur_trans.geoset_matrices.at(m_id) = temp;
      }

    float scale = 1.0 / m_count;
    for(Uint32 i = 0; i < 16; ++i)
      cur_trans.geoset_matrices.at(m_id).data[i] *= scale;
}

void SimpleModel_MD::CalcTransforms(TransformInfo & cur_trans, const Matrix4x4 pmat, const Sint32 pid, const AnimationInfo & anim_info) const {
  for(vector<MDXBone>::const_iterator it = bones.begin(); it != bones.end(); ++it) {
    if(it->object.parent == pid) {
      Uint32 bone_id = it - bones.begin();
      it->CalcBoneTransform(cur_trans.bone_transforms.at(bone_id), pivots.at(it->object.object_id), pmat, anim_info);
      CalcTransforms(cur_trans, cur_trans.bone_transforms.at(bone_id), it->object.object_id, anim_info);
      }
    }
  }

void SimpleModel_MD::MDXBone::CalcBoneTransform(Matrix4x4 & res, const MDXVertex & center, const Matrix4x4 & pmat, const AnimationInfo & anim_info) const {
  MDXVertex translation;
  Quaternion rotation = {{0.0}};
  bool has_translation = false;
  bool has_rotation = false;

  if(object.type == 264) {
    //res = identity4x4;
    res = pmat;
    return;
    }

  if(object.translation_info.key_frames.size() > 0)
    has_translation = CalcBoneTranslation(translation, anim_info);

  if(object.rotation_info.key_frames.size() > 0)
    has_rotation = CalcBoneRotation(rotation, anim_info);

  Matrix4x4 pivot_mat;
  pivot_mat = identity4x4;
  pivot_mat.data[12] = center.x;
  pivot_mat.data[13] = center.y;
  pivot_mat.data[14] = center.z;

  Matrix4x4 pivot_matN;
  pivot_matN = identity4x4;
  pivot_matN.data[12] = -center.x;
  pivot_matN.data[13] = -center.y;
  pivot_matN.data[14] = -center.z;

  Matrix4x4 trans_mat;
  trans_mat = identity4x4;
  trans_mat.data[12] = translation.x;
  trans_mat.data[13] = translation.y;
  trans_mat.data[14] = translation.z;

  Matrix4x4 rot_mat;
  QuaternionToMatrix4x4(rot_mat, rotation);

  Matrix4x4 accum_mat1;
  Matrix4x4 accum_mat2;
  Matrix4x4 accum_mat3;

  accum_mat1 = identity4x4;
  accum_mat2 = identity4x4;
  accum_mat3 = identity4x4;
    
  if(has_rotation) {
    Multiply(accum_mat1, rot_mat, pivot_matN);
    Multiply(accum_mat2, pivot_mat, accum_mat1);
    }

  if(has_translation)
    Multiply(accum_mat3, trans_mat, accum_mat2);
  else
    accum_mat3 = accum_mat2;

  Multiply(res, pmat, accum_mat3);
  }

bool SimpleModel_MD::MDXBone::CalcBoneTranslation(MDXVertex & res, const AnimationInfo & anim_info) const {
  vector<MDXKeyFrameTS>::const_iterator start_frame;
  vector<MDXKeyFrameTS>::const_iterator end_frame;
  
  if(anim_info.cur_seq->start == (object.translation_info.key_frames.end() - 1)->frame) {
    res = (object.translation_info.key_frames.end() - 1)->point;
    return true;
    };

  for(vector<MDXKeyFrameTS>::const_iterator it = object.translation_info.key_frames.begin(); it != object.translation_info.key_frames.end(); ++it) {
    start_frame = it;
    end_frame = (it + 1);

    if(anim_info.cur_frame == start_frame->frame) {
      res = start_frame->point;
      return true;
      }

    if(anim_info.cur_frame == end_frame->frame) {
      res = end_frame->point;
      return true;
      }

    if(start_frame->frame < anim_info.cur_frame && anim_info.cur_frame < end_frame->frame) {
      if(anim_info.cur_seq->start > start_frame->frame && anim_info.cur_seq->end < end_frame->frame)
        return false;

      if(anim_info.cur_seq->start <= start_frame->frame && anim_info.cur_seq->end >= end_frame->frame) {
        float weight = float(anim_info.cur_frame - start_frame->frame) / (end_frame->frame - start_frame->frame);
        LERP(res, start_frame->point, end_frame->point, weight);
        }
      else
        res = start_frame->point;
      
      return true;
      }
    };

  return false;
  };

bool SimpleModel_MD::MDXBone::CalcBoneRotation(Quaternion & res, const AnimationInfo & anim_info) const {
  vector<MDXKeyFrameR>::const_iterator start_frame;
  vector<MDXKeyFrameR>::const_iterator end_frame;
  
  if(anim_info.cur_seq->start == (object.rotation_info.key_frames.end() - 1)->frame) {
    res = (object.rotation_info.key_frames.end() - 1)->quat;
    return true;
    };

  for(vector<MDXKeyFrameR>::const_iterator it = object.rotation_info.key_frames.begin(); it != object.rotation_info.key_frames.end(); ++it) {
    start_frame = it;
    end_frame = (it + 1);

    if(anim_info.cur_frame == start_frame->frame) {
      res = start_frame->quat;
      return true;
      }

    if(anim_info.cur_frame == end_frame->frame) {
      res = end_frame->quat;
      return true;
      }

    if(start_frame->frame < anim_info.cur_frame && anim_info.cur_frame < end_frame->frame) {
      if(anim_info.cur_seq->start > start_frame->frame && anim_info.cur_seq->end < end_frame->frame)
        return false;

      if(anim_info.cur_seq->start <= start_frame->frame && anim_info.cur_seq->end >= end_frame->frame) {
        float weight = float(anim_info.cur_frame - start_frame->frame) / (end_frame->frame - start_frame->frame);
        SLERP(res, start_frame->quat, end_frame->quat, weight);
        Normalize(res, res);
        }
      else
        res = start_frame->quat;
      
      return true;
      }
    };

  return false;
  };

int SimpleModel_MD::CalcBaseFrame(Uint32 cur_time, const vector<int> &anim,
    const vector<Uint32> &start_time, float &offset) const {
  int frame = 0;
  offset = 0.0;
  if(sequences.size() > 0) {
    if(anim.size() < 1 || start_time.size() < 1) {
      fprintf(stderr, "WARNING: Not enough anims/times sent to animate MD3.\n");
      return false;
      }
    int start = sequences.at(anim[0]).start;
    float fps = 120.0; //sequences[anim[0]].speed;    // FIXME: Not sure about this?
    float elapsed = cur_time - start_time[0];

    float disp = elapsed * fps / 1000.0;
    frame = start + (int)(disp);
    offset = disp - floorf(disp);
    }
  return frame;
  }

int SimpleModel_MD::NormalizeFrame(const vector<int> &anim, int frame) const {
  if(sequences.size() > 0) {
    if(anim.size() < 1) {
      fprintf(stderr, "WARNING: Not enough anims/times sent to animate MD.\n");
      return 0;
      }

    int end = sequences[anim[0]].end;
    bool loop = sequences[anim[0]].loop;

    if(frame >= end && loop == true) {
      frame -= sequences.at(anim.at(0)).end - sequences.at(anim.at(0)).start;
    } else if(frame >= end)
      frame = end - 1;
    }
  else {
    frame = 0;
    }
  return frame;
  }

void SimpleModel_MD::MDXBone::LERP(MDXVertex & res, const MDXVertex & v1, const MDXVertex & v2, const float factor) const {
	float k1 = 1.0 - factor;
	float k2 = factor;
			
	res.x = v1.x * k1 + v2.x * k2;
	res.y = v1.y * k1 + v2.y * k2;
	res.z = v1.z * k1 + v2.z * k2;
  }

void SimpleModel_MD::MatVecMult(MDXVertex & res, const Matrix4x4 & m, const MDXVertex & v) const {
  res.x = m.data[0] * v.x + m.data[4] * v.y + v.z * m.data[8] + m.data[12];
  res.y = m.data[1] * v.x + m.data[5] * v.y + v.z * m.data[9] + m.data[13];
  res.z = m.data[2] * v.x + m.data[6] * v.y + v.z * m.data[10] + m.data[14];
  };
