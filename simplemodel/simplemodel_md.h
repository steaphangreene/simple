// *************************************************************************
//  This file is part of the SimpleModel Example Module by Steaphan Greene
//
//  Copyright 2005-2008 Steaphan Greene <stea@cs.binghamton.edu>
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

#ifndef SIMPLEMODEL_MD_H
#define SIMPLEMODEL_MD_H

#include "simplemodel.h"

class SimpleModel_MD : public SimpleModel {
public:
  SimpleModel_MD(const string &filenm);
  virtual ~SimpleModel_MD();

  virtual bool Load(const string &filenm) {return false;};

protected:
  virtual bool RenderSelf(Uint32 cur_time,
	const vector<int> &anim = vector<int>(),
	const vector<Uint32> &start_time = vector<Uint32>(),
	Uint32 anim_offset = 0) const;

  enum InterpolationTypes {InterpolationNone = 0, InterpolationTypeLinear = 1, InterpolationTypeHermite = 2, InterpolationTypeBezier = 3};

  struct MDXVertex {
    MDXVertex() {x = 0.0; y = 0.0; z = 0.0;};
    float x;
    float y;
    float z;
    };

  // Interpolation info for translation and scale
  struct MDXKeyFrameTS {
    Uint32 frame;
    MDXVertex point;
    float in_tan[3];
    float out_tan[3];
    };

  // Interpolation info for rotation
  struct MDXKeyFrameR {
    Uint32 frame;
    Quaternion quat;
    float in_tan[4];
    float out_tan[4];
    };

  // Interpolation info for visibility
  struct MDXKeyFrameV {
    Uint32 frame;
    float state;
    float in_tan;
    float out_tan;
    };

  struct MDXTranslationInfo_KGTR {
    Uint32 nunks;
    Uint32 line_type;       // dont = 0, linear = 1, hermite = 2, bezier = 3
    Sint32 global_seq_id;   // -1 if none 
    vector<MDXKeyFrameTS> key_frames;
    };

  struct MDXRotationInfo_KGRT {
    Uint32 nunks;
    Uint32 line_type;       // dont = 0, linear = 1, hermite = 2, bezier = 3
    Sint32 global_seq_id;
    vector<MDXKeyFrameR> key_frames;
    };
  
  struct MDXScalingInfo_KGSC {
    Uint32 nunks;
    Uint32 line_type;       // dont = 0, linear = 1, hermite = 2, bezier = 3
    Sint32 global_seq_id;   // -1 if none 
    vector<MDXKeyFrameTS> key_frames;
    };

  struct MDXVisibilityInfo_KATV {
    Uint32 nunks;
    Uint32 line_type;
    Sint32 global_seq_id;
    vector<MDXKeyFrameV> key_frames;    
    };

  struct MDXObject {
    Uint8 name[0x50];
    Uint32 object_id;
    Sint32 parent;      // default = 0, none = -1, FIXME: Not too sure about this
    Uint32 type;
    MDXTranslationInfo_KGTR translation_info;
    MDXRotationInfo_KGRT rotation_info;
    MDXScalingInfo_KGSC scaling_info;
    MDXVisibilityInfo_KATV visibility_info;
    };

  struct MDXModel {
    Uint8 name[0x150];
    Uint32 unknown;
    float bound_radius;
    float min_extents[3];
    float max_extents[3];
    Uint32 blend_time;
    };

  struct MDXAttachment {
    MDXObject object;
    Uint8 path[0x100];
    Uint32 unknown;
    Uint32 attchment_id;
    // FIXME: MDXVisibilityInfo_KATV visibility_info;
    };

  struct MDXSequence {
    Uint8 name[0x50];
    Uint32 start;
    Uint32 end;
    float speed;
    bool loop;          // In the file it's a long with 0 meaning loop and 1 meaning no loop
    float rarity;
    Uint32 unknown;
    float bound_radius;
    float min_extents[3];
    float max_extents[3];
    };

  struct AnimationInfo {
    vector<MDXSequence>::const_iterator cur_seq;
    Uint32 cur_frame;
    float interpolation_weight;
    };

  struct MDXBone {
    void CalcBoneTransform(Matrix4x4 &, const MDXVertex &, const Matrix4x4 &, const AnimationInfo &) const;
    bool CalcBoneTranslation(MDXVertex &, const AnimationInfo &) const;
    bool CalcBoneRotation(Quaternion &, const AnimationInfo &) const;
    void LERP(MDXVertex &, const MDXVertex &, const MDXVertex &, const float) const;

    MDXObject object;
    Uint32 geoset_id;
    Uint32 geoset_anim_id;
    };

  struct MDXGeosetAnim {
    float AlphaAt(const AnimationInfo &anim) const;

    float static_alpha;         // If static_alpha = 1.0 then we use the KGAO struct
    Uint32 color_animation;     // none = 0, drop shadow = 1, color = 2, both = 3, default is 1
    float color[3];             // R, G, B
    Uint32 geoset_id;
    vector<MDXVisibilityInfo_KATV> alpha_info;
    // FIXME: MDXColorInfo_KGAC color_info;
    };

  struct MDXNormal {
    float normal[3];
    };

  struct MDXFrameInfo {
    float bound_radius;
    float min_extents[3];
    float max_extents[3];
    };

  struct MDXTextureCoord {
    float coord[2];
    };

  struct TransformInfo {
    vector<Matrix4x4> bone_transforms;
    vector<Matrix4x4> geoset_matrices;
    };

  struct MDXLayer {
    Uint32 filter_mode;
    Uint32 shading;
    Uint32 texture_id;
    Uint32 vert_anim_id;
    Uint32 coord_id;
    float alpha;
    };

  struct MDXMaterial {
    Uint32 priority_plane;
    Uint32 render_mode;
    vector<MDXLayer> layers;
    };

  struct MDXGeoset {
    void CalculateGroupMatrices(TransformInfo &) const;
    void AccumulateBoneTransforms(TransformInfo &, const Uint32, const Uint32, const Uint32) const;  

    vector<MDXVertex> vertices;
    vector<MDXNormal> normals;
    vector<Uint32> prim_types;       
    vector<Uint32> prim_counts;
    vector<Uint16> indices;
    vector<Uint8> vertex_groups;
    vector<Uint32> group_counts;
    vector<Uint32> matrices;
    Uint32 material_id;
    Uint32 selection_group;
    Uint32 selectable;                  // Doesn't seem relevant to us, none = 0, 4 = unselectable
    float bound_radius;
    float min_extents[3];
    float max_extents[3];
    vector<MDXFrameInfo> animations;
    vector<vector<MDXTextureCoord> > texture_coords_uvas;                
    vector<MDXTextureCoord> texture_coords_uvbs;                
    };

  struct MDXGlobalSequence {
    vector<Uint32> durations;    
    };

  struct MDXTexture {
    Uint32 replacable_id;   // FIXME: Not sure about this
    Uint8 path[0x100];
    Uint32 unknown;
    Uint32 wrapping;        // wrap width = 1, wrap height = 2, both = 3
    };

  SimpleModel_MD();
  void CalcTransforms(TransformInfo &, const Matrix4x4 &, const Sint32, const AnimationInfo &) const;

  int NormalizeFrame(const vector<int> &anim, int frame) const;
  int CalcBaseFrame(Uint32 cur_time, const vector<int> &anim,
    const vector<Uint32> &start_time, float &offset) const;
  void MatVecMult(MDXVertex & res, const Matrix4x4 & m, const MDXVertex & v) const;

  MDXModel model_info;
  vector<MDXSequence> sequences;
  MDXGlobalSequence global_sequences;
  vector<MDXTexture> textures;
  vector<MDXGeoset> geosets;
  vector<MDXGeosetAnim> geosetanims;
  vector<MDXMaterial> materials;
  vector<MDXBone> bones;
  vector<MDXVertex> pivots;
  };

#endif  //SIMPLEMODEL_MD_H
