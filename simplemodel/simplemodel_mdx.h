// *************************************************************************
//  This file is part of the SimpleModel Example Module by Steaphan Greene
//
//  Copyright 2005-2008 Steaphan Greene <stea@cs.binghamton.edu>
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

#ifndef SIMPLEMODEL_MDX_H
#define SIMPLEMODEL_MDX_H

#include "simplemodel_md.h"

class SimpleModel_MDX : public SimpleModel_MD {
public:
  SimpleModel_MDX(const string &filenm,
	const string &modelnm = "", const string &defskin = "");
  virtual ~SimpleModel_MDX();

  virtual bool Load(const string &filenm,
	const string &modelnm, const string &defskin);

protected:
  SimpleModel_MDX();

  bool HandleMagicWord(SDL_RWops *);
  bool HandleModel(SDL_RWops *);
  bool HandleVersion(SDL_RWops *);
  bool HandleSequences(SDL_RWops *);
  bool HandleGlobalSequences(SDL_RWops *);
  bool HandleTextures(const string &filenm, SDL_RWops *);
  bool HandleGeosets(SDL_RWops *);
  bool HandleBones(SDL_RWops *);
  bool HandlePivots(SDL_RWops *);
  bool HandleHelpers(SDL_RWops *);
  string GetNextToken(SDL_RWops *);    // Returns the identifier for the next chunk
  bool IsIgnoredToken(const string &);
  void SkipChunk(SDL_RWops *);
  bool HandleVertices(SDL_RWops *, vector<MDXVertex> &);
  bool HandleNormals(SDL_RWops *, vector<MDXNormal> &);
  bool HandlePrimTypes(SDL_RWops *, vector<Uint32> &);
  bool HandlePrimCounts(SDL_RWops *, vector<Uint32> &);
  bool HandleIndices(SDL_RWops *, vector<Uint16> &);
  bool HandleVertexGroups(SDL_RWops *, vector<Uint8> &);
  bool HandleGroupCounts(SDL_RWops *, vector<Uint32> &);
  bool HandleMatrices(SDL_RWops *, vector<Uint32> &);
  bool HandleFrameInfo(SDL_RWops *, vector<MDXFrameInfo> &);
  bool HandleUVAS(SDL_RWops *, vector<vector<MDXTextureCoord> > &);
  bool HandleUVBS(SDL_RWops *, vector<MDXTextureCoord> &);
  bool HandleObject(SDL_RWops *, MDXObject &);
  bool HandleKGTR(SDL_RWops *, MDXTranslationInfo_KGTR &);
  bool HandleKGRT(SDL_RWops *, MDXRotationInfo_KGRT &);
  bool HandleKGSC(SDL_RWops *, MDXScalingInfo_KGSC &);
  bool HandleKATV(SDL_RWops *, MDXVisibilityInfo_KATV &);
  bool NextTokenIs(SDL_RWops *, const string &);

  Uint32 file_size;
  };

#endif  //SIMPLEMODEL_MDX_H
