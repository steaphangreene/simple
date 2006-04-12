// *************************************************************************
//  This file is part of the SimpleModel Example Module by Steaphan Greene
//
//  Copyright 2005-2006 Steaphan Greene <stea@cs.binghamton.edu>
//
//  3DS support written by Noah Schwartz with chunk parsing code based on Damiano Vitulli's 3DS loading
//  tutorial at http://spacesimulator.net/tut4_3dsloader.html
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

#include <cstdio>
#include <cstdlib>
#include <iostream>
using namespace std;

#include "simplemodel_3ds.h"
#include "saferead.h"

SimpleModel_3DS::SimpleModel_3DS(const string &filenm, const string &texnm) {
  Load(filenm, texnm);
  }

SimpleModel_3DS::SimpleModel_3DS() {
  }

SimpleModel_3DS::~SimpleModel_3DS() {
  }

bool SimpleModel_3DS::Load(const string &filenm, const string &texnm) {
  SDL_RWops *model = SDL_RWFromFile(filenm.c_str(), "rb");
  Sint32 filelen = 0;
  Uint16 chunk_id = 0;		// Chunk's ID (3DS files are structured in chunks. We only want some of them.)
  Uint32 chunk_length = 0;	// Length of a chunk
  Uint8 temp_char = 0;
  Uint16 chunk_qty= 0;		// Quantity of whatever the chunk we're in contains
  Uint16 face_flags = 0;
  Uint32 index = 0;
  MAXMesh current_mesh;
  current_mesh.has_texture = false;
  bool first_mesh = true;
  
  if(model == NULL) {
    fprintf(stderr, "WARNING: Unable to open model file '%s'!\n",
	filenm.c_str());
    perror("WARNING");
    return false;
    }
  
  SDL_RWseek(model, 0, SEEK_END);
  filelen = SDL_RWtell(model);
  SDL_RWseek(model, 0, SEEK_SET);
  while(SDL_RWtell(model) < filelen) {
  	freadLE(chunk_id, model);
	freadLE(chunk_length, model);
	
	switch(chunk_id) {
	  case 0x4d4d:
	    break;
	  
	  case 0x3d3d:
	    break;
		
	  case 0x4000:
	    index = 0;
		do {
		  freadLE(temp_char, model);
		  current_mesh.name[index] = temp_char;
		  ++index;
		  } while(temp_char != '\0' && index < 20);
	  	break;
		
	  // A triangle mesh chunk. This means we're working with a new mesh.
      case 0x4100:
		if(first_mesh == true)
		  first_mesh = false;
		else {
		  mesh.push_back(current_mesh);
		  current_mesh.vertex.clear();
		  current_mesh.polygon.clear();
		  current_mesh.tex_coord.clear();
		  current_mesh.has_texture = false;
		  for(index = 0; index < 20; ++index)
		    current_mesh.name[index] = '\0';
		  }	
	    break;

	  case 0x4110:
		freadLE(chunk_qty, model);
		current_mesh.vertex.resize(chunk_qty);
		for(index = 0; index < chunk_qty; ++index) {
		  freadLE(current_mesh.vertex[index].x, model);
		  freadLE(current_mesh.vertex[index].y, model);
		  freadLE(current_mesh.vertex[index].z, model);
		  }
		break;
		
	  case 0x4120:
		freadLE(chunk_qty, model);
		current_mesh.polygon.resize(chunk_qty);
		for(index = 0; index < chunk_qty; ++index) {
		  freadLE(current_mesh.polygon[index].v1, model);
		  freadLE(current_mesh.polygon[index].v2, model);
		  freadLE(current_mesh.polygon[index].v3, model);
		  freadLE(face_flags, model);	// Don't really need these, just reading them in
		  }
		break;
		
	  case 0x4140:
		current_mesh.has_texture = true;
	    freadLE(chunk_qty, model);
		current_mesh.tex_coord.resize(chunk_qty);
		for(index = 0; index < chunk_qty; ++index) {
		  freadLE(current_mesh.tex_coord[index].u, model);
		  freadLE(current_mesh.tex_coord[index].v, model);
		  }
		break;
	  
	  default:
	    SDL_RWseek(model, chunk_length - 6, SEEK_CUR);		
	  }
    }
  SDL_RWclose(model);
  // Push on the last mesh
  mesh.push_back(current_mesh);
  
  //mesh.texture = new SimpleTexture(texnm);
  //mesh.texture->Update();
  
  Uint32 mesh_index = 0;
  // Create the display list	
  display_list = glGenLists(1);
  glNewList(display_list, GL_COMPILE);
  glDisable(GL_TEXTURE_2D);
  glBegin(GL_TRIANGLES);
  for(mesh_index = 0; mesh_index < mesh.size(); ++mesh_index) {
    for(index=0; index < mesh[mesh_index].polygon.size(); ++index) {
	  //----------------- FIRST VERTEX -----------------
	  // Texture coordinates of the first vertex
	  if(mesh[mesh_index].has_texture == true)
	    glTexCoord2f(mesh[mesh_index].tex_coord[mesh[mesh_index].polygon[index].v1].u, mesh[mesh_index].tex_coord[mesh[mesh_index].polygon[index].v1].v);
	  // Coordinates of the first vertex
	  glVertex3f(mesh[mesh_index].vertex[mesh[mesh_index].polygon[index].v1].x, mesh[mesh_index].vertex[mesh[mesh_index].polygon[index].v1].y, mesh[mesh_index].vertex[mesh[mesh_index].polygon[index].v1].z);

	  //----------------- SECOND VERTEX -----------------
	  // Texture coordinates of the second vertex
	  if(mesh[mesh_index].has_texture == true)
   	    glTexCoord2f(mesh[mesh_index].tex_coord[mesh[mesh_index].polygon[index].v2].u, mesh[mesh_index].tex_coord[mesh[mesh_index].polygon[index].v2].v);
	  // Coordinates of the first vertex
	  glVertex3f(mesh[mesh_index].vertex[mesh[mesh_index].polygon[index].v2].x, mesh[mesh_index].vertex[mesh[mesh_index].polygon[index].v2].y, mesh[mesh_index].vertex[mesh[mesh_index].polygon[index].v2].z);
	
	  //----------------- THIRD VERTEX -----------------
	  // Texture coordinates of the third vertex
	  if(mesh[mesh_index].has_texture == true)
	    glTexCoord2f(mesh[mesh_index].tex_coord[mesh[mesh_index].polygon[index].v3].u, mesh[mesh_index].tex_coord[mesh[mesh_index].polygon[index].v3].v);
	  // Coordinates of the first vertex
	  glVertex3f(mesh[mesh_index].vertex[mesh[mesh_index].polygon[index].v3].x, mesh[mesh_index].vertex[mesh[mesh_index].polygon[index].v3].y, mesh[mesh_index].vertex[mesh[mesh_index].polygon[index].v3].z);
      }
	}
  glEnd();
  glEndList();
  return true;
  }

bool SimpleModel_3DS::Render(Uint32 cur_time, const vector<int> &anim,
	const vector<Uint32> &start_time) const {
  glCallList(display_list);
  return true;
  }
