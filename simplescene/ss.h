// *************************************************************************
//  This file is part of the SimpleScene Example Module by Steaphan Greene
//
//  Copyright 2005-2008 Steaphan Greene <stea@cs.binghamton.edu>
//
//  SimpleScene is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 3 of the License, or
//  (at your option) any later version.
//
//  SimpleScene is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SimpleScene (see the file named "COPYING");
//  If not, see <http://www.gnu.org/licenses/>.
//  
// *************************************************************************

#ifndef SS_H
#define SS_H

#include "SDL.h"
#include "SDL_opengl.h"

#include <map>
#include <list>
#include <vector>
using namespace std;

class SimpleModel;
class SimpleTexture;

typedef int SS_PType;
const SS_PType SS_UNDEFINED_PARTICLETYPE = -1;
typedef int SS_Particle;
typedef int SS_Skin;
const SS_Skin SS_UNDEFINED_SKIN = -1;
typedef int SS_Model;
const SS_Model SS_UNDEFINED_MODEL = -1;
typedef int SS_Object;
typedef int SS_Action;

class SimpleScene {
public:
  SimpleScene();
  ~SimpleScene();

  void RestrictX(float x0, float y0);
  void RestrictY(float z0, float x1);
  void RestrictZ(float y1, float z1);
  bool Render(Uint32 offset);

  static SimpleScene *Current() { return current; };

  SS_Skin AddSkin(SimpleTexture *skin);

  SS_Model AddModel(SimpleModel *mod);
  const SimpleModel *GetModel(SS_Model which) { return models[which].model; }

  SS_Object AddObject(
	SS_Model mod = SS_UNDEFINED_MODEL, SS_Skin skin = SS_UNDEFINED_SKIN);
  void ModelObject(SS_Object obj, SS_Model mod, Uint32 fin = 0, Uint32 dur = 0);
  void SkinObject(SS_Object obj, SS_Model skin, Uint32 fin = 0, Uint32 dur = 0);
  void SizeObject(SS_Object obj, float sz, Uint32 end = 0, Uint32 dur = 0);
  void MoveObject(SS_Object obj, float xp, float yp, float zp,
	Uint32 end = 0, Uint32 dur = 0);
  void TurnObject(SS_Object obj, float ang, Uint32 end = 0, Uint32 dur = 0);
  void TargetObject(SS_Object obj, float xt, float yt, float zt,
	Uint32 end = 0, Uint32 dur = 0);
  void ShowObject(SS_Object obj, Uint32 fin);
  void HideObject(SS_Object obj, Uint32 fin);
  void ColorObject(SS_Object obj, float r, float g, float b, Uint32 tm = 0);
  void ActObject(SS_Object obj, SS_Action act, Uint32 fin, Uint32 dur);

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
  struct ActionData {
    SS_Object obj;
    Uint32 finish, duration;
    };

  struct ActionTime {
    Uint32 finish, duration;
    };

  struct Color {
    float r, g, b;
    };

  struct Coord {
    float x, y, z;
    //These are so set<> and map<> can sort by these, and for basic comparisons.
    bool operator < (const SimpleScene::Coord &b) const {
      return (z < b.z || (z == b.z && (y < b.y || (y == b.y && x < b.x))));
      }
    bool operator > (const SimpleScene::Coord &b) const {
      return (z > b.z || (z == b.z && (y > b.y || (y == b.y && x > b.x))));
      }
    bool operator != (const SimpleScene::Coord &b) const {
      return (z != b.z || y != b.y || x != b.x);
      }
    bool operator == (const SimpleScene::Coord &b) const {
      return (z != b.z || y != b.y || x != b.x);
      }
    };

  struct Model {
    SimpleModel *model;
    };

  struct Skin {
    SimpleTexture *tex;
    };

  struct Object {
    list<pair<SS_Model, ActionTime> > model;
    list<pair<SS_Skin, ActionTime> > skin;
    list<pair<float, ActionTime> > size;
    list<Uint32> show;
    list<pair<Color, Uint32> > col;
    list<pair<float, ActionTime> > turns;
    list<pair<Coord, ActionTime> > targets;
    list<pair<SS_Action, ActionTime> > acts;
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

  bool DrawObjects(Uint32 offset);
  bool DrawParticles(Uint32 offset);

  vector<Skin> skins;
  vector<Model> models;
  list<pair<Coord, ActionData> > moves;
  map<SS_Object, Object> objects;
  vector<PType> ptypes;
  vector<Particle> parts;
  SS_Object next_obj;

  static SimpleScene *current;

  float resx0, resy0, resz0;
  float resx1, resy1, resz1;
  float xlim0, ylim0, zlim0;
  float xlim1, ylim1, zlim1;
  };

#endif // SS_H
