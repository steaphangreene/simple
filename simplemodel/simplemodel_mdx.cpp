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

#include "SDL.h"
#include "SDL_image.h"

#include <cmath>
#include <cstdio>
#include <cctype>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <iostream>
using namespace std;

#include "simplemodel_mdx.h"
#include "saferead.h"

SimpleModel_MDX::SimpleModel_MDX(const string &filenm,
	const string &modelnm, const vector<string> &skins) : SimpleModel_MD() {
  Load(filenm, modelnm, skins);
  }

SimpleModel_MDX::SimpleModel_MDX() : SimpleModel_MD() {
  }

SimpleModel_MDX::~SimpleModel_MDX() {
  }

static bool is_same_filename (const string &cf1, const string &cf2) {
  string f1 = cf1;
  string f2 = cf2;
  size_t loc;
  loc = f1.find_first_of("/\\");
  while(loc >= 0 && loc < f1.size()) {
    f1 = f1.substr(1);
    loc = f1.find_first_of("/\\");
    }
  loc = f2.find_first_of("/\\");
  while(loc >= 0 && loc < f2.size()) {
    f2 = f2.substr(1);
    loc = f2.find_first_of("/\\");
    }
  if(f1.length() < 1 || f2.length() < 1) {
    return false;
    }
  if(!(strcasecmp(f1.c_str(), f2.c_str()))) {
    return true;
    }
  return false;
  }

bool SimpleModel_MDX::Load(const string &filenm,
	const string &modelnm, const vector<string> &skins) {
//  fprintf(stderr, "Opening '%s'\n", filenm.c_str());
  string token;

  filename = filenm + "/" + modelnm;

  SDL_RWops *model = SDL_RWFromZZIP(filename.c_str(), "rb");
  if(!model) {
    fprintf(stderr, "WARNING: Unable to open model file '%s'!\n", filename.c_str());
    perror("WARNING");
    return false;
    }

  SDL_RWseek(model, 0, SEEK_END);
  file_size = SDL_RWtell(model);
  SDL_RWseek(model, 0, SEEK_SET);

  HandleMagicWord(model);

  while((token = GetNextToken(model)) != "EOB") {
//    fprintf(stderr, "Current token is '%s'.\n", token.c_str());
    if(token == "VERS") {
      if(HandleVersion(model) == false)
        return false;
      } 
    else if(token == "MODL") {
      if(HandleModel(model) == false)
        return false;
      } 
    else if(token == "SEQS") {
      if(HandleSequences(model) == false)
        return false;
      } 
    else if(token == "GLBS") {
      if(HandleGlobalSequences(model) == false)
        return false;
      } 
    else if(token == "TEXS") {
      if(HandleTextures(filenm, model) == false)
        return false;
      } 
    else if(token == "GEOS") {
      if(HandleGeosets(model) == false)
        return false;
      }
    else if(token == "MTLS") {
      if(HandleMaterials(model) == false)
        return false;
      }
    else if(token == "GEOA") {
      if(HandleGeosetAnims(model) == false)
        return false;
      }
    else if(token == "BONE") {
      if(HandleBones(model) == false)
        return false;
      }
    else if(token == "HELP") {
      if(HandleHelpers(model) == false)
        return false;
      }
    else if(token == "PIVT") {
      if(HandlePivots(model) == false)
        return false;
      }
    else if(token == "SKIP")
      SkipChunk(model); 
    else {
      fprintf(stderr, "WARNING: Unknown token '%s' in MDX file!\n", token.c_str());
      return false;
      }
    }

  for(unsigned int skin=0; skin < skins.size(); ++skin) {
    vector<MDXTexture>::const_iterator tex = textures.begin();
    for(; tex != textures.end(); ++tex) {
      if(is_same_filename(skins[skin], (char*)(tex->path))) {
	//fprintf(stderr, "Replacing texture %d (%s/%s)\n",
	//	tex - textures.begin(), tex->path, skins[skin].c_str());
	delete(texture[tex - textures.begin()]);
	if(skins[skin].c_str()[0] == '/') {
	  texture[tex - textures.begin()] = new SimpleTexture(skins[skin]);
	  }
	else {
	  texture[tex - textures.begin()]
		= new SimpleTexture(filenm + "/" + skins[skin]);
	  }
	//fprintf(stderr, "->%d\n", texture[tex - textures.begin()]->GLTexture());
	}
      else if(tex->replacable_id == 1
		&& SimpleTexture::IsColorName(skins[skin])) {
	//fprintf(stderr, "Replacing teamcolor %d (%s/%s)\n",
	//	tex - textures.begin(), tex->path, skins[skin].c_str());
	delete(texture[tex - textures.begin()]);
	texture[tex - textures.begin()]	= new SimpleTexture(skins[skin]);
	}
      }
    }

  SDL_RWclose(model);
  return true;
  }

bool SimpleModel_MDX::HandleMagicWord(SDL_RWops * model) {
  Uint8 magic[4];

  SDL_RWread(model, magic, 1, sizeof(magic));
  if(strncmp((char *)magic, "MDLX", 4)) {
    fprintf(stderr, "WARNING: File '%s' is not an MDX file!\n", filename.c_str());
    return false;
    }  

  return true;
  }

bool SimpleModel_MDX::HandleVersion(SDL_RWops * model) {
  Uint32 version = 0;

  // Skip 4 bytes of size information
  SDL_RWseek(model, 4, SEEK_CUR);

  freadLE(version, model);

  if(version != 800) {
    fprintf(stderr, "WARNING: File '%s' is not a version 800 MDX file!\n", filename.c_str());
    return false;
    }

  return true;
  }

bool SimpleModel_MDX::HandleModel(SDL_RWops * model) {
  SDL_RWseek(model, 4, SEEK_CUR);
  SDL_RWread(model, &model_info.name, 1, 0x150);
  freadLE(model_info.unknown, model);
  freadLE(model_info.bound_radius, model);

  freadLE(model_info.min_extents[0], model);
  freadLE(model_info.min_extents[1], model);
  freadLE(model_info.min_extents[2], model);

  freadLE(model_info.max_extents[0], model);
  freadLE(model_info.max_extents[1], model);
  freadLE(model_info.max_extents[2], model);  

  freadLE(model_info.blend_time, model);
  
  return true;
  }

bool SimpleModel_MDX::HandleSequences(SDL_RWops * model) {
  Uint32 num_bytes = 0;
  Uint32 num_sequences = 0;
  Uint32 loop = 0;

  freadLE(num_bytes, model);
  num_sequences = num_bytes / sizeof(MDXSequence);
  sequences.resize(num_sequences);

  for(vector<MDXSequence>::iterator it = sequences.begin(); it != sequences.end(); ++it) {
    SDL_RWread(model, &(it->name), 1, 0x50);
    freadLE(it->start, model);
    freadLE(it->end, model);
    freadLE(it->speed, model);
   
    freadLE(loop, model);
    it->loop = (loop == 0 ? true : false);
    
    freadLE(it->rarity, model);
    freadLE(it->unknown, model);
    freadLE(it->bound_radius, model);

    freadLE(it->min_extents[0], model);
    freadLE(it->min_extents[1], model);
    freadLE(it->min_extents[2], model);

    freadLE(it->max_extents[0], model);
    freadLE(it->max_extents[1], model);
    freadLE(it->max_extents[2], model);

    /*
    printf("Sequence name: %s.\n", it->name);
    printf("Sequence start: %d.\n", it->start);
    printf("Sequence end: %d.\n\n", it->end);
    */

    animations[(char *)it->name] = it - sequences.begin();
    }

  return true;
  }

bool SimpleModel_MDX::HandleGlobalSequences(SDL_RWops * model) {
  Uint32 num_bytes = 0;

  freadLE(num_bytes, model);
  global_sequences.durations.resize(num_bytes / 4);
  for(vector<Uint32>::iterator it = global_sequences.durations.begin(); it != global_sequences.durations.end(); ++it)
    freadLE(*it, model);

  return true;
  }

bool SimpleModel_MDX::HandleTextures(const string &filenm, SDL_RWops * model) {
  Uint32 num_bytes = 0;
  
  freadLE(num_bytes, model);
  textures.resize(num_bytes / sizeof(MDXTexture));
  
  for(vector<MDXTexture>::iterator it = textures.begin(); it != textures.end(); ++it) {
    freadLE(it->replacable_id, model);
    SDL_RWread(model, &(it->path), 1, 0x100);
    freadLE(it->unknown, model);
    freadLE(it->wrapping, model);

    string buffer;
    buffer = filenm + "/" + (char *)(it->path);
    if(it->replacable_id == 0) {
      texture.push_back(new SimpleTexture(buffer));
      }
    else if(it->replacable_id == 1) {	//Team Color (Default is Black)
      texture.push_back(new SimpleTexture(
	SimpleTexture::NewColor(0.0, 0.0, 0.0)
	));
      }
    else {
      texture.push_back(new SimpleTexture("not:real:texture"));
      }
//    fprintf(stderr, "[%d->%d] %s\n", it->replacable_id,
//	(*(texture.end() - 1))->GLTexture(), buffer.c_str());
    }

  return true;
  }

bool SimpleModel_MDX::HandleGeosets(SDL_RWops * model) {
  Uint32 chunk_size = 0;
  Uint32 file_offset = SDL_RWtell(model);
  Uint32 bytes_read = 0;

  freadLE(chunk_size, model);

  while(bytes_read <= chunk_size) {
    MDXGeoset new_geoset;

    SDL_RWseek(model, 4, SEEK_CUR);
    if(HandleVertices(model, new_geoset.vertices) == false)
      return false;

    if(HandleNormals(model, new_geoset.normals) == false)
      return false;

    if(HandlePrimTypes(model, new_geoset.prim_types) == false)
      return false;

    if(HandlePrimCounts(model, new_geoset.prim_counts) == false)
      return false;

    if(HandleIndices(model, new_geoset.indices) == false)
      return false;

    if(HandleVertexGroups(model, new_geoset.vertex_groups) == false)
      return false;

    if(HandleGroupCounts(model, new_geoset.group_counts) == false)
      return false;

    if(HandleMatrices(model, new_geoset.matrices) == false)
      return false;

    freadLE(new_geoset.material_id, model);
    freadLE(new_geoset.selection_group, model);
    freadLE(new_geoset.selectable, model);
    freadLE(new_geoset.bound_radius, model);

    freadLE(new_geoset.min_extents[0], model);
    freadLE(new_geoset.min_extents[1], model);
    freadLE(new_geoset.min_extents[2], model);

    freadLE(new_geoset.max_extents[0], model);
    freadLE(new_geoset.max_extents[1], model);
    freadLE(new_geoset.max_extents[2], model);

    if(HandleFrameInfo(model, new_geoset.animations) == false)
      return false;

    if(HandleUVAS(model, new_geoset.texture_coords_uvas) == false)
      return false;

    /*
    fprintf(stderr, "Number of vertices in geoset: %d.\n", new_geoset.vertices.size());
    fprintf(stderr, "Number of normals in geoset: %d.\n", new_geoset.normals.size());
    fprintf(stderr, "Number of indices in geoset: %d.\n", new_geoset.indices.size());
    for(Uint32 i = 0; i < new_geoset.indices.size(); ++i)
      fprintf(stderr, "%d ", new_geoset.indices.at(i));
    fprintf(stderr, "\n");
    fprintf(stderr, "Number of vertex groups in geoset: %d.\n", new_geoset.vertex_groups.size());
    fprintf(stderr, "Number of group counts in geoset: %d.\n", new_geoset.group_counts.size());
    fprintf(stderr, "Number of matrices in geoset: %d.\n\n", new_geoset.matrices.size());
    */
    /*
    for(Uint32 i = 0; i < new_geoset.matrices.size(); ++i)
      fprintf(stderr, "%d ", new_geoset.matrices.at(i));
    fprintf(stderr, "\n");
    */
    geosets.push_back(new_geoset);

    bytes_read = SDL_RWtell(model) - file_offset;
    }

//  fprintf(stderr, "Number of geosets %d.\n", geosets.size());

  return true;
  }

bool SimpleModel_MDX::HandleMaterials(SDL_RWops * model) {
  Uint32 chunk_size = 0;
  Uint32 file_offset = SDL_RWtell(model);
  Uint32 bytes_read = 0;

  freadLE(chunk_size, model);

  bytes_read = SDL_RWtell(model) - file_offset;
  while(bytes_read < chunk_size) {
    if(HandleMaterial(model) == false) return false;
    bytes_read = SDL_RWtell(model) - file_offset;
    }
  return true;
  }

bool SimpleModel_MDX::HandleMaterial(SDL_RWops * model) {
  Uint32 chunk_size = 0;
  Uint32 file_offset = SDL_RWtell(model);
  Uint32 bytes_read = 0;

  freadLE(chunk_size, model);

  MDXMaterial material;

  freadLE(material.priority_plane, model);
  freadLE(material.render_mode, model);

  Uint8 tag_name[4];
  SDL_RWread(model, &tag_name, 1, sizeof(tag_name));
  if(strncmp((char*)tag_name, "LAYS", 4)) {
    fprintf(stderr, "ERROR: Layer didn't start with 'LAYS' tag %.8X!\n", *((Uint32*)(tag_name)));
    return false;
    }

  Uint32 num_layers;
  freadLE(num_layers, model);

  bytes_read = SDL_RWtell(model) - file_offset;
  while(material.layers.size() < num_layers) {
    MDXLayer layer;
    Uint32 layer_size;
    freadLE(layer_size, model);
    SDL_RWseek(model, 8, SEEK_CUR);
    freadLE(layer.texture_id, model);
    SDL_RWseek(model, layer_size - 16, SEEK_CUR);
    bytes_read = SDL_RWtell(model) - file_offset;
    material.layers.push_back(layer);
    }
  materials.push_back(material);
  return true;
  }

bool SimpleModel_MDX::HandleGeosetAnims(SDL_RWops * model) {
  Uint32 chunk_size = 0;
  Uint32 file_offset = SDL_RWtell(model);
  Uint32 bytes_read = 0;

  freadLE(chunk_size, model);

  bytes_read = SDL_RWtell(model) - file_offset;
  while(bytes_read < chunk_size) {
    if(HandleGeosetAnim(model) == false) return false;
    bytes_read = SDL_RWtell(model) - file_offset;
    }
  return true;
  }

bool SimpleModel_MDX::HandleGeosetAnim(SDL_RWops * model) {
  Uint32 chunk_size = 0;
  Uint32 file_offset = SDL_RWtell(model);
  Uint32 bytes_read = 0;

  freadLE(chunk_size, model);

  MDXGeosetAnim geoanim;

  freadLE(geoanim.static_alpha, model);
  freadLE(geoanim.color_animation, model);
  freadLE(geoanim.color[0], model);
  freadLE(geoanim.color[1], model);
  freadLE(geoanim.color[2], model);
  freadLE(geoanim.geoset_id, model);

  bytes_read = SDL_RWtell(model) - file_offset;
  while(bytes_read < chunk_size) {
    Uint8 tag_name[4];

    SDL_RWread(model, &tag_name, 1, sizeof(tag_name));

    if(!strncmp((char *)tag_name, "KGAO", 4)) {
      MDXVisibilityInfo_KATV alpha;

      freadLE(alpha.nunks, model);
      freadLE(alpha.line_type, model);
      freadLE(alpha.global_seq_id, model);

      alpha.key_frames.resize(alpha.nunks);
      for(Uint32 key = 0; key < alpha.nunks; ++key) {
	freadLE(alpha.key_frames[key].frame, model);
	freadLE(alpha.key_frames[key].state, model);
	if(alpha.line_type > 1) {
	  SDL_RWseek(model, 8, SEEK_CUR);
	  }
	//fprintf(stderr, "KEY_FRAME[%d][%d] %d: %f [%d]\n",
	//	geoanim.geoset_id, alpha.global_seq_id,
	//	alpha.key_frames[key].frame, alpha.key_frames[key].state, alpha.line_type);
	}
      geoanim.alpha_info.push_back(alpha);
      }
    else if(!strncmp((char *)tag_name, "KGAC", 4)) {
      Uint32 num, type;
      freadLE(num, model);
      freadLE(type, model);
      SDL_RWseek(model, 4+(16+(24*(type > 0)))*num, SEEK_CUR);
      //FIXME: Really handle this!
      }
    else {
      fprintf(stderr, "ERROR: Invalid tag in GEOA %.8X!\n", *((Uint32*)(tag_name)));
      exit(0);
      }
    bytes_read = SDL_RWtell(model) - file_offset;
    }
  geosetanims.push_back(geoanim);
  return true;
  }

bool SimpleModel_MDX::HandleVertices(SDL_RWops * model, vector<MDXVertex> & vertices) {
  Uint32 num_verts = 0;
  Uint8 chunk_name[4];

  SDL_RWread(model, &chunk_name, 1, sizeof(chunk_name));
  if(strncmp((char *)chunk_name, "VRTX", 4)) {
    fprintf(stderr, "Expected VRTX magic word and instead found '%s'!\n", chunk_name);
    return false;
    }

  freadLE(num_verts, model);
  vertices.resize(num_verts);
  for(vector<MDXVertex>::iterator it = vertices.begin(); it != vertices.end(); ++it) {
    freadLE(it->x, model);
    freadLE(it->y, model);
    freadLE(it->z, model);
    }

  return true;
  }

bool SimpleModel_MDX::HandleNormals(SDL_RWops * model, vector<MDXNormal> & normals) {
  Uint32 num_norms = 0;
  Uint8 chunk_name[4];

  SDL_RWread(model, &chunk_name, 1, sizeof(chunk_name));
  if(strncmp((char *)chunk_name, "NRMS", 4)) {
    fprintf(stderr, "Expected NRMS magic word and instead found '%s'!\n", chunk_name);
    return false;
    }

  freadLE(num_norms, model);
  normals.resize(num_norms);
  for(vector<MDXNormal>::iterator it = normals.begin(); it != normals.end(); ++it) {
    freadLE(it->normal[0], model);
    freadLE(it->normal[1], model);
    freadLE(it->normal[2], model);
    }

  return true;
  }

bool SimpleModel_MDX::HandlePrimTypes(SDL_RWops * model, vector<Uint32> & prim_types) {
  Uint32 num_ptypes = 0;
  Uint8 chunk_name[4];

  SDL_RWread(model, &chunk_name, 1, sizeof(chunk_name));
  if(strncmp((char *)chunk_name, "PTYP", 4)) {
    fprintf(stderr, "Expected PTYP magic word and instead found '%s'!\n", chunk_name);
    return false;
    }

  freadLE(num_ptypes, model);
  prim_types.resize(num_ptypes);
  for(vector<Uint32>::iterator it = prim_types.begin(); it != prim_types.end(); ++it)
    freadLE(*it, model);

  return true;
  }

bool SimpleModel_MDX::HandlePrimCounts(SDL_RWops * model, vector<Uint32> & prim_counts) {
  Uint32 num_pcounts = 0;
  Uint8 chunk_name[4];

  SDL_RWread(model, &chunk_name, 1, sizeof(chunk_name));
  if(strncmp((char *)chunk_name, "PCNT", 4)) {
    fprintf(stderr, "Expected PCNT magic word and instead found '%s'!\n", chunk_name);
    return false;
    }

  freadLE(num_pcounts, model);
  prim_counts.resize(num_pcounts);
  for(vector<Uint32>::iterator it = prim_counts.begin(); it != prim_counts.end(); ++it)
    freadLE(*it, model);

  return true;
  }

bool SimpleModel_MDX::HandleIndices(SDL_RWops * model, vector<Uint16> & indices) {
  Uint32 num_indices = 0;
  Uint8 chunk_name[4];

  SDL_RWread(model, &chunk_name, 1, sizeof(chunk_name));
  if(strncmp((char *)chunk_name, "PVTX", 4)) {
    fprintf(stderr, "Expected PVTX magic word and instead found '%s'!\n", chunk_name);
    return false;
    }

  freadLE(num_indices, model);
  indices.resize(num_indices);
  for(vector<Uint16>::iterator it = indices.begin(); it != indices.end(); ++it)
    freadLE(*it, model);

  return true;
  }

bool SimpleModel_MDX::HandleVertexGroups(SDL_RWops * model, vector<Uint8> & vertex_groups) {
  Uint32 num_vgroups = 0;
  Uint8 chunk_name[4];

  SDL_RWread(model, &chunk_name, 1, sizeof(chunk_name));
  if(strncmp((char *)chunk_name, "GNDX", 4)) {
    fprintf(stderr, "Expected GNDX magic word and instead found '%s'!\n", chunk_name);
    return false;
    }

  freadLE(num_vgroups, model);
  vertex_groups.resize(num_vgroups);
  for(vector<Uint8>::iterator it = vertex_groups.begin(); it != vertex_groups.end(); ++it)
    freadLE(*it, model);

  return true;
  }

bool SimpleModel_MDX::HandleGroupCounts(SDL_RWops * model, vector<Uint32> & group_counts) {
  Uint32 num_gcounts = 0;
  Uint8 chunk_name[4];

  SDL_RWread(model, &chunk_name, 1, sizeof(chunk_name));
  if(strncmp((char *)chunk_name, "MTGC", 4)) {
    fprintf(stderr, "Expected MTGC magic word and instead found '%s'!\n", chunk_name);
    return false;
    }

  freadLE(num_gcounts, model);
  group_counts.resize(num_gcounts);
  for(vector<Uint32>::iterator it = group_counts.begin(); it != group_counts.end(); ++it)
    freadLE(*it, model);

  return true;
  }

bool SimpleModel_MDX::HandleMatrices(SDL_RWops * model, vector<Uint32> & matrices) {
  Uint32 num_matrices = 0;
  Uint8 chunk_name[4];

  SDL_RWread(model, &chunk_name, 1, sizeof(chunk_name));
  if(strncmp((char *)chunk_name, "MATS", 4)) {
    fprintf(stderr, "Expected MATS magic word and instead found '%s'!\n", chunk_name);
    return false;
    }

  freadLE(num_matrices, model);
  matrices.resize(num_matrices);
  for(vector<Uint32>::iterator it = matrices.begin(); it != matrices.end(); ++it)
    freadLE(*it, model);

  return true;
  }

bool SimpleModel_MDX::HandleFrameInfo(SDL_RWops * model, vector<MDXFrameInfo> & mdx_animations) {
  Uint32 num_anims = 0;

  freadLE(num_anims, model);
  mdx_animations.resize(num_anims);
  for(vector<MDXFrameInfo>::iterator it = mdx_animations.begin(); it != mdx_animations.end(); ++it) {
    freadLE(it->bound_radius, model);

    freadLE(it->min_extents[0], model);
    freadLE(it->min_extents[1], model);
    freadLE(it->min_extents[2], model);

    freadLE(it->max_extents[0], model);
    freadLE(it->max_extents[1], model);
    freadLE(it->max_extents[2], model);
    }

  return true;
  }

bool SimpleModel_MDX::HandleUVAS(SDL_RWops * model, vector<vector<MDXTextureCoord> > & uvas) {
  Uint32 num_tverts = 0;
  Uint8 chunk_name[4];

  SDL_RWread(model, &chunk_name, 1, sizeof(chunk_name));
  if(strncmp((char *)chunk_name, "UVAS", 4)) {
    fprintf(stderr, "Expected UVAS magic word and instead found '%s'!\n", chunk_name);
    return false;
    }

  freadLE(num_tverts, model);
  uvas.resize(num_tverts);
  for(vector<vector<MDXTextureCoord> >::iterator it = uvas.begin(); it != uvas.end(); ++it)
    HandleUVBS(model, *it);

  return true;
  }

bool SimpleModel_MDX::HandleUVBS(SDL_RWops * model, vector<MDXTextureCoord> & uvbs) {
  Uint32 num_verts = 0;
  Uint8 chunk_name[4];

  SDL_RWread(model, &chunk_name, 1, sizeof(chunk_name));
  if(strncmp((char *)chunk_name, "UVBS", 4)) {
    fprintf(stderr, "Expected UVBS magic word and instead found '%s'!\n", chunk_name);
    return false;
    }

  freadLE(num_verts, model);
  uvbs.resize(num_verts);
  for(vector<MDXTextureCoord>::iterator it = uvbs.begin(); it != uvbs.end(); ++it) {
    freadLE(it->coord[0], model);
    freadLE(it->coord[1], model);
    }

  return true;
  }

bool SimpleModel_MDX::HandleBones(SDL_RWops * model) {
  Uint32 chunk_size = 0;
  Uint32 file_offset = SDL_RWtell(model);
  Uint32 bytes_read = 0;

  freadLE(chunk_size, model);

  while(bytes_read <= chunk_size) {
    MDXBone new_bone;

    if(HandleObject(model, new_bone.object) == false)
      return false;

    freadLE(new_bone.geoset_id, model);
    freadLE(new_bone.geoset_anim_id, model);

    bones.push_back(new_bone);

//    fprintf(stderr, "Trans frame num: %d.\n", new_bone.object.translation_info.key_frames.size());
//    fprintf(stderr, "Rot frame num: %d.\n", new_bone.object.rotation_info.key_frames.size());
//    fprintf(stderr, "Scale frame num: %d.\n", new_bone.object.scaling_info.key_frames.size());
//    fprintf(stderr, "ID: %d\n", new_bone.object.object_id);
//    fprintf(stderr, "Parent: %d\n\n", new_bone.object.parent);

    bytes_read = SDL_RWtell(model) - file_offset;
    }

//  fprintf(stderr, "Number of bones %d.\n", bones.size());

  return true;  
  }

bool SimpleModel_MDX::HandleObject(SDL_RWops * model, MDXObject & object) {
  SDL_RWseek(model, 4, SEEK_CUR);
  SDL_RWread(model, object.name, 1, 0x50);
  freadLE(object.object_id, model);
  freadLE(object.parent, model);
  freadLE(object.type, model);
  
  if(NextTokenIs(model, "KGTR")) {
    if(HandleKGTR(model, object.translation_info) == false)
      return false;
    }    

  if(NextTokenIs(model, "KGRT")) {
    if(HandleKGRT(model, object.rotation_info) == false)
      return false;
    }    

  if(NextTokenIs(model, "KGSC")) {
    if(HandleKGSC(model, object.scaling_info) == false)
      return false;
    }    

  if(NextTokenIs(model, "KATV")) {
    if(HandleKATV(model, object.visibility_info) == false)
      return false;
    }    

  return true;
  }

bool SimpleModel_MDX::HandleKGTR(SDL_RWops * model, MDXTranslationInfo_KGTR & trans) {
  Uint32 nunks = 0;

  // Skip chunk name
  SDL_RWseek(model, 4, SEEK_CUR);

  freadLE(nunks, model);
  trans.key_frames.resize(nunks);
  freadLE(trans.line_type, model);
  freadLE(trans.global_seq_id, model);

  for(Uint32 i = 0; i < nunks; ++i) {
    freadLE(trans.key_frames.at(i).frame, model);
    freadLE(trans.key_frames.at(i).point.x, model);
    freadLE(trans.key_frames.at(i).point.y, model);
    freadLE(trans.key_frames.at(i).point.z, model);
    if(trans.line_type > InterpolationTypeLinear) {
      freadLE(trans.key_frames.at(i).in_tan[0], model);
      freadLE(trans.key_frames.at(i).in_tan[1], model);
      freadLE(trans.key_frames.at(i).in_tan[2], model);

      freadLE(trans.key_frames.at(i).out_tan[0], model);
      freadLE(trans.key_frames.at(i).out_tan[1], model);
      freadLE(trans.key_frames.at(i).out_tan[2], model);
      }
    }
  return true;
  };  

bool SimpleModel_MDX::HandleKGRT(SDL_RWops * model, MDXRotationInfo_KGRT & rot) {
  Uint32 nunks = 0;

  // Skip chunk name
  SDL_RWseek(model, 4, SEEK_CUR);

  freadLE(nunks, model);
  rot.key_frames.resize(nunks);
  freadLE(rot.line_type, model);
  freadLE(rot.global_seq_id, model);

  for(Uint32 i = 0; i < nunks; ++i) {
    freadLE(rot.key_frames.at(i).frame, model);
    freadLE(rot.key_frames.at(i).quat.data[0], model);
    freadLE(rot.key_frames.at(i).quat.data[1], model);
    freadLE(rot.key_frames.at(i).quat.data[2], model);
    freadLE(rot.key_frames.at(i).quat.data[3], model);
    rot.key_frames.at(i).quat.data[3] = -(rot.key_frames.at(i).quat.data[3]);
    if(rot.line_type > InterpolationTypeLinear) {
      freadLE(rot.key_frames.at(i).in_tan[0], model);
      freadLE(rot.key_frames.at(i).in_tan[1], model);
      freadLE(rot.key_frames.at(i).in_tan[2], model);
      freadLE(rot.key_frames.at(i).in_tan[3], model);

      freadLE(rot.key_frames.at(i).out_tan[0], model);
      freadLE(rot.key_frames.at(i).out_tan[1], model);
      freadLE(rot.key_frames.at(i).out_tan[2], model);
      freadLE(rot.key_frames.at(i).out_tan[3], model);
      }
    }
  return true;
  };  

bool SimpleModel_MDX::HandleKGSC(SDL_RWops * model, MDXScalingInfo_KGSC & scale) {
  Uint32 nunks = 0;

  // Skip chunk name
  SDL_RWseek(model, 4, SEEK_CUR);

  freadLE(nunks, model);
  scale.key_frames.resize(nunks);
  freadLE(scale.line_type, model);
  freadLE(scale.global_seq_id, model);

  for(Uint32 i = 0; i < nunks; ++i) {
    freadLE(scale.key_frames.at(i).frame, model);
    freadLE(scale.key_frames.at(i).point.x, model);
    freadLE(scale.key_frames.at(i).point.y, model);
    freadLE(scale.key_frames.at(i).point.z, model);
    if(scale.line_type > InterpolationTypeLinear) {
      freadLE(scale.key_frames.at(i).in_tan[0], model);
      freadLE(scale.key_frames.at(i).in_tan[1], model);
      freadLE(scale.key_frames.at(i).in_tan[2], model);

      freadLE(scale.key_frames.at(i).out_tan[0], model);
      freadLE(scale.key_frames.at(i).out_tan[1], model);
      freadLE(scale.key_frames.at(i).out_tan[2], model);
      }
    }
  return true;
  };

bool SimpleModel_MDX::HandleKATV(SDL_RWops * model, MDXVisibilityInfo_KATV & vis) {
  Uint32 nunks = 0;

  // Skip chunk name
  SDL_RWseek(model, 4, SEEK_CUR);

  freadLE(nunks, model);
  vis.key_frames.resize(nunks);
  freadLE(vis.line_type, model);
  freadLE(vis.global_seq_id, model);

  for(Uint32 i = 0; i < nunks; ++i) {
    freadLE(vis.key_frames.at(i).frame, model);
    freadLE(vis.key_frames.at(i).state, model);
    if(vis.line_type > InterpolationTypeLinear) {
      freadLE(vis.key_frames.at(i).in_tan, model);
      freadLE(vis.key_frames.at(i).out_tan, model);
      }
    }
  return true;
  };

bool SimpleModel_MDX::HandlePivots(SDL_RWops * model) {
  Uint32 chunk_size;

  freadLE(chunk_size, model);
  pivots.resize(chunk_size / (sizeof(float) * 3));

  for(Uint32 i = 0; i < pivots.size(); ++i) {
    freadLE(pivots.at(i).x, model);
    freadLE(pivots.at(i).y, model);
    freadLE(pivots.at(i).z, model);
    // fprintf(stderr, "%f, %f, %f\n", pivots.at(i).x, pivots.at(i).y, pivots.at(i).z);
    }

  return true;    
  };

bool SimpleModel_MDX::HandleHelpers(SDL_RWops * model) {
  Uint32 chunk_size = 0;
  Uint32 file_offset = SDL_RWtell(model);
  Uint32 bytes_read = 0;

  freadLE(chunk_size, model);

  while(bytes_read <= chunk_size) {
    MDXBone new_bone;

    if(HandleObject(model, new_bone.object) == false)
      return false;

    bones.push_back(new_bone);

//    fprintf(stderr, "Trans frame num: %d.\n", new_bone.object.translation_info.key_frames.size());
//    fprintf(stderr, "Rot frame num: %d.\n", new_bone.object.rotation_info.key_frames.size());
//    fprintf(stderr, "Scale frame num: %d.\n", new_bone.object.scaling_info.key_frames.size());
//    fprintf(stderr, "ID: %d\n", new_bone.object.object_id);
//    fprintf(stderr, "Parent: %d\n\n", new_bone.object.parent);

    bytes_read = SDL_RWtell(model) - file_offset;
    }

  return true;  
  };

string SimpleModel_MDX::GetNextToken(SDL_RWops * model) {
  Uint32 current_offset = SDL_RWtell(model);
  Uint8 token[4];
  string buffer;

  // If not at eob then read next four bytes and return it as a string
  if(current_offset + 4 <= file_size) {
    SDL_RWread(model, &token, 1, sizeof(token));
    buffer = token[0];
    buffer += token[1];
    buffer += token[2];
    buffer += token[3];
    if(IsIgnoredToken(buffer) == false) {
      return buffer;
      }
    else {
//      fprintf(stderr, "WARNING: Unhandled token '%s' in MDX file!\n", buffer.c_str());
      return "SKIP";
      }
    } 
  else {
//    fprintf(stderr, "Returning EOB.\n");
    return "EOB";
    }
  }

bool SimpleModel_MDX::IsIgnoredToken(const string & token) {
  if(token == "MDLX")
    return false;
  else if(token == "VERS")
    return false;
  else if(token == "MODL")
    return false;
  else if(token == "SEQS")
    return false;
  else if(token == "GLBS")
    return false;
  else if(token == "TEXS")
    return false;
  else if(token == "GEOS")
    return false;
  else if(token == "MTLS")
    return false;
  else if(token == "GEOA")
    return false;
  else if(token == "BONE")
    return false;
  else if(token == "PIVT")
    return false;
  else if(token == "HELP")
    return false;
  else
    return true;
  }

void SimpleModel_MDX::SkipChunk(SDL_RWops * model) {
  Uint32 size_of_chunk = 0;

  freadLE(size_of_chunk, model);
  SDL_RWseek(model, size_of_chunk, SEEK_CUR);
  }

bool SimpleModel_MDX::NextTokenIs(SDL_RWops * model, const string & str) {
  Uint8 token[4];
  string buffer;

  SDL_RWread(model, &token, 1, 4);
  buffer = token[0];
  buffer += token[1];
  buffer += token[2];
  buffer += token[3];

  SDL_RWseek(model, -4, SEEK_CUR);

  if(buffer == str)

    return true;
  else
    return false;
  }
