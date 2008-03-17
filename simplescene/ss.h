// *************************************************************************
//  This file is part of the SimpleScene Example Module by Steaphan Greene
//
//  Copyright 2005-2008 Steaphan Greene <stea@cs.binghamton.edu>
//
//  SimpleScene is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  SimpleScene is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SimpleScene (see the file named "COPYING");
//  if not, write to the the Free Software Foundation, Inc.,
//  59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
// *************************************************************************

#ifndef SS_H
#define SS_H

#include "SDL.h"
#include "SDL_opengl.h"

#include <vector>
using namespace std;

class SimpleModel;
class SimpleTexture;

typedef int SS_Particle;
typedef int SS_PType;
const SS_PType SS_UNDEFINED_PARTICLETYPE = -1;
typedef int SS_Object;
typedef int SS_Model;
const SS_Model SS_UNDEFINED_MODEL = -1;

class SimpleScene {
public:
  SimpleScene();
  ~SimpleScene();

  bool Render(Uint32 offset);

  static SimpleScene *Current() { return current; };

  SS_Model AddModel(SimpleModel *mod);

  SS_Object AddObject(SS_Model mod = SS_UNDEFINED_MODEL);
  void SetObjectModel(SS_Model mod);

  SS_PType AddPType();
  void SetPTypeTexture(SS_PType type, SimpleTexture *tex);
  void SetPTypeColor0(SS_PType type, float cr, float cg, float cb, float ca);
  void SetPTypeColor1(SS_PType type, float cr, float cg, float cb, float ca);
  void SetPTypeColor(SS_PType type, float cr, float cg, float cb, float ca);
  void SetPTypeVelocity(SS_PType type, float xv, float yv, float zv);
  void SetPTypeSize(SS_PType type, float sz);
  void SetPTypeSize0(SS_PType type, float sz);
  void SetPTypeSize1(SS_PType type, float sz);
  void SetPTypeDuration(SS_PType type, Uint32 dur);

  SS_Particle AddParticle(SS_PType type = SS_UNDEFINED_PARTICLETYPE);
  void SetParticleType(SS_Particle part, SS_PType type);
  void SetParticlePosition(SS_Particle part, float xp, float yp, float zp);
  void SetParticleTime(SS_Particle part, Uint32 start);

  void Clear();

protected:
  struct Object {
    SS_Model mod;
    };

  struct PType {
    SimpleTexture *tex;
    float cr0, cg0, cb0, ca0;
    float cr1, cg1, cb1, ca1;
    float xv, yv, zv;
    float sz0, sz1;
    Uint32 dur;
    };

  struct Particle {
    SS_PType type;
    float xp, yp, zp;
    Uint32 start;
    };

  bool DrawParticles(Uint32 offset);

  vector<SimpleModel *> models;
  vector<Object> objects;
  vector<PType> ptypes;
  vector<Particle> parts;

  static SimpleScene *current;
  };

#endif // SS_H
