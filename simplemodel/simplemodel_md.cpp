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
  }

SimpleModel_MD::~SimpleModel_MD() {
  }

/*
  cur_transforms stores the bone_transforms and the geoset_transforms for this frame of rendering
*/
bool SimpleModel_MD::RenderSelf(Uint32 cur_time, const vector<int> & anim,
	const vector<Uint32> & start_time, Uint32 anim_offset) const {
  TransformInfo cur_transforms;
  AnimationInfo anim_info;
  float factor;  

  glPushMatrix();
  glScalef(1.0/32.0, 1.0/32.0, 1.0/32.0);

  anim_info.cur_frame = CalcBaseFrame(cur_time, anim, start_time, factor);
  anim_info.cur_frame = NormalizeFrame(anim, anim_info.cur_frame);
  anim_info.cur_seq = sequences.begin() + anim.at(0);
  anim_info.interpolation_weight = factor;

  cur_transforms.bone_transforms.resize(bones.size());

  //fprintf(stderr, "Start frame: %d\n", sequences.at(anim[0]).start);
  //fprintf(stderr, "End frame: %d\n", sequences.at(anim[0]).end);
  //fprintf(stderr, "Frame: %d.\n\n", anim_info.cur_frame);

  CalcTransforms(cur_transforms, identity4x4, -1, anim_info);
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

  glPopMatrix();
  return true;
  }

/*
  In this function we loop through the geosets group counts and build the matrices that will actually be used to transform the geoset's vertices at render time (these matrices are stored in current_transforms and are not to be confused with the vector<Uint32> matrices in an MDXGeoset). For every i-th element of group_count, we look in the geosets matrices vector from group_count[i - 1] to group_count[i] and accumulate the bone transforms referenced by those elements in matrices.
*/
void SimpleModel_MD::MDXGeoset::CalculateGroupMatrices(TransformInfo & current_transforms) const {
  Uint32 index = 0;
    
  current_transforms.geoset_matrices.resize(group_counts.size());
  for(Uint32 i = 0; i < group_counts.size(); ++i) {
      AccumulateBoneTransforms(current_transforms, i, group_counts.at(i), index);
      index += group_counts.at(i);
  }
}

/*
  This is where we accumulate the bone transforms. m_id refers to which transformation matrix were building. m_count refers to how many elements in the geoset's matrices vector that were going to look at. m_start is index saying at which element we start at. We basically loop and for every i that we look at in matrices, we add bone_transform[matrices[i]] to the transformation matrix for render time m_id. Once were done we scale it by m_count.
*/
void SimpleModel_MD::MDXGeoset::AccumulateBoneTransforms(TransformInfo & cur_trans, const Uint32 m_id, const Uint32 m_count, const Uint32 m_start) const {
    for(Uint32 i = 0; i < m_count; ++i)
      Add(cur_trans.geoset_matrices.at(m_id), cur_trans.geoset_matrices.at(m_id), cur_trans.bone_transforms.at(matrices.at(m_start + i)));
    
    float scale = 1.0 / m_count;
    Multiply(cur_trans.geoset_matrices.at(m_id), cur_trans.geoset_matrices.at(m_id), scale);
}

/*
  This is where we start building the bone transforms. pmat is the parent matrix (this is called from render and identity4x4 is passed) and pid is the parent id (bones without parents have -1 as a parent id). Basically, we loop through all of the bones, for every bone with a parent_id = pid we calculate its transform using the parent matrix that we currently have. Then we build it's children by calling this same function but, with the transform of the bone we just calculated as the parent matrix and its object_id as the parent id.
*/
void SimpleModel_MD::CalcTransforms(TransformInfo & cur_trans, const Matrix4x4 & pmat, const Sint32 pid, const AnimationInfo & anim_info) const {
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

  // If type isn't 0 or 256 then it's not really a bone and has something to do will billboard.
  // FIXME: Is this why animation doesn't work?
  if(object.type != 0 && object.type != 256) {
    res = identity4x4;
    //res = pmat;
    return;
    }
  
  if(object.translation_info.key_frames.size() > 0)
    has_translation = CalcBoneTranslation(translation, anim_info);

  if(object.rotation_info.key_frames.size() > 0)
    has_rotation = CalcBoneRotation(rotation, anim_info);

  
  Matrix4x4 pivot_mat = identity4x4;
  pivot_mat.data[12] = center.x;
  pivot_mat.data[13] = center.y;
  pivot_mat.data[14] = center.z;

  Matrix4x4 pivot_matN = identity4x4;
  pivot_matN.data[12] = -center.x;
  pivot_matN.data[13] = -center.y;
  pivot_matN.data[14] = -center.z;

  Matrix4x4 trans_mat = identity4x4;
  trans_mat.data[12] = translation.x;
  trans_mat.data[13] = translation.y;
  trans_mat.data[14] = translation.z;

  Matrix4x4 rot_mat;
  QuaternionToMatrix4x4(rot_mat, rotation);

  if(has_rotation && has_translation) {
    Multiply(res, pmat, trans_mat, pivot_mat, rot_mat, pivot_matN);
    }
  else if(has_rotation) {
    Multiply(res, pmat, pivot_mat, rot_mat, pivot_matN);
    }
  else if(has_translation) {
    Multiply(res, pmat, trans_mat);
    }
  else {
    res = pmat;
    }
  }

/*
  res is the resultant translation vector
*/
bool SimpleModel_MD::MDXBone::CalcBoneTranslation(MDXVertex & res, const AnimationInfo & anim_info) const {
  vector<MDXKeyFrameTS>::const_iterator start_frame;
  vector<MDXKeyFrameTS>::const_iterator end_frame;
  
  // If we're at the last frame of animation, set res to be the point at that frame
  if(anim_info.cur_seq->start == (object.translation_info.key_frames.end() - 1)->frame) {
    res = (object.translation_info.key_frames.end() - 1)->point;
    return true;
    };

  // Loop through the frames to find our match
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

    // Make sure current frame falls between start frame and end frame
    if(start_frame->frame < anim_info.cur_frame && anim_info.cur_frame < end_frame->frame) {
      // Make sure this sequence starts after start frame and ends before end frame
      if(anim_info.cur_seq->start > start_frame->frame && anim_info.cur_seq->end < end_frame->frame)
        return false;

      // If our current animation starts before start frame and ends after end frame, interpolate into res
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

/*
  Comments from MDXBone::CalcBoneTranslation apply to this func
*/
bool SimpleModel_MD::MDXBone::CalcBoneRotation(Quaternion & res, const AnimationInfo & anim_info) const {
  vector<MDXKeyFrameR>::const_iterator start_frame;
  vector<MDXKeyFrameR>::const_iterator end_frame;
  
  if(anim_info.cur_seq->start == (object.rotation_info.key_frames.end() - 1)->frame) {
    res = (object.rotation_info.key_frames.end() - 1)->quat;
    return true;
    };

  for(vector<MDXKeyFrameR>::const_iterator it = object.rotation_info.key_frames.begin(); it != object.rotation_info.key_frames.end() - 1; ++it) {
    start_frame = it;
    end_frame = (it + 1);

    if(anim_info.cur_frame == start_frame->frame) {
      res = start_frame->quat;
      return true;
      }
    else if(anim_info.cur_frame == end_frame->frame) {
      res = end_frame->quat;
      return true;
      }
    else if(start_frame->frame < anim_info.cur_frame && anim_info.cur_frame < end_frame->frame) {
      if(anim_info.cur_seq->start > start_frame->frame && anim_info.cur_seq->end < end_frame->frame)
        return false;

      if(anim_info.cur_seq->start <= start_frame->frame && anim_info.cur_seq->end >= end_frame->frame) {
        float weight = float(anim_info.cur_frame - start_frame->frame) / (end_frame->frame - start_frame->frame);
        SLERP(res, start_frame->quat, end_frame->quat, weight);
        //Normalize(res, res);	// This doesn't seem to be required.
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
    float fps = 960.0; //sequences[anim[0]].speed;    // FIXME: Not sure about this?
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
