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

#ifndef SDL_DEBUG		// Just define SDL_DEBUG to add debug
#define SDL_DEBUG	0	// options to SDL_Init()
#endif	//SDL_DEBUG		// such as SDL_NO_PARACHUTE

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>

#ifndef M_PI
#define M_PI		3.14159265358979323846  /* pi */
#endif
#define DEG2RAD(d)	((d)*M_PI/180.0)

using namespace std;

#include "ss.h"
#include "simpletexture.h"
#include "simplemodel.h"
#include "simplevideo.h"

SimpleScene *SimpleScene::current = NULL;

SimpleScene::SimpleScene() {
  if(current) {
    fprintf(stderr, "ERROR: Created mulitple SimpleScene instances!\n");
    exit(1);
    }
  resx0 = 0.0; resx1 = 0.0;
  resy0 = 0.0; resy1 = 0.0;
  resz0 = 0.0; resz1 = 0.0;
  next_obj = 1;
  current = this;
  }

SimpleScene::~SimpleScene() {
  //at_exit handles all this
  }

bool SimpleScene::Render(Uint32 offset) {
  glDisable(GL_LIGHTING);	//FIXME: Real Scene Lighting
  if(SimpleVideo::Current() != NULL) {	//FIXME: What if neither way is defined?
    SimpleVideo::Current()->GetViewLimits(
	xlim0, ylim0, zlim0, xlim1, ylim1, zlim1);
    }
  if(resx0 < resx1) {
    if(xlim0 < resx0) xlim0 = resx0;
    if(xlim1 > resx1) xlim1 = resx1;
    }
  if(resy0 < resy1) {
    if(ylim0 < resy0) ylim0 = resy0;
    if(ylim1 > resy1) ylim1 = resy1;
    }
  if(resz0 < resz1) {
    if(zlim0 < resz0) zlim0 = resz0;
    if(zlim1 > resz1) zlim1 = resz1;
    }
  if(!DrawObjects(offset)) return false;
  return DrawParticles(offset);
  }

SS_Model SimpleScene::AddModel(SimpleModel *mod) {
  models.push_back(mod);
  return (SS_Model)(models.size() - 1);
  }

SS_Skin SimpleScene::AddSkin(SimpleTexture *skin) {
  skins.push_back(skin);
  return (SS_Skin)(skins.size() - 1);
  }

SS_Object SimpleScene::AddObject(SS_Model mod, SS_Skin skin) {
  const Object obj = { mod, skin, 1.0 };
  objects[next_obj] = obj;
  return next_obj++;
  }

void SimpleScene::ObjectAct(SS_Object obj,
	SS_Action act, Uint32 fin, Uint32 dur) {
  Action newact = { obj, act, fin, dur };
  objects[obj].acts.push_back(newact);
  }

void SimpleScene::ShowObject(SS_Object obj, Uint32 tm) {
  objects[obj].show.push_back(tm);
  }

void SimpleScene::HideObject(SS_Object obj, Uint32 tm) {
  objects[obj].show.push_back(tm);
  }

void SimpleScene::ColorObject(SS_Object obj,
	float r, float g, float b, Uint32 tm) {
  Color c = { r, g, b };
  objects[obj].col.push_back(pair<Color, Uint32>(c, tm));
  }

void SimpleScene::SetObjectSkin(SS_Object obj, SS_Skin skin) {
  objects[obj].skin = skin;
  }

void SimpleScene::SetObjectModel(SS_Object obj, SS_Model mod) {
  objects[obj].model = mod;
  }

void SimpleScene::SetObjectColor(SS_Object obj, float r, float g, float b) {
  Color c = { r, g, b };
  objects[obj].col.push_back(pair<Color, Uint32>(c, 0));
  }

void SimpleScene::MoveObject(SS_Object obj, float xp, float yp, float zp,
	Uint32 end, Uint32 dur) {
  Coord pos = { xp, yp, zp };
  Action act = { obj, SS_ACT_MOVE, end, dur };
  moves.push_front(pair<Coord, Action>(pos, act));
  }

void SimpleScene::TurnObject(SS_Object obj, float ang, Uint32 end, Uint32 dur) {
  Action act = { obj, SS_ACT_TURN, end, dur };
  objects[obj].turns.push_front(pair<float, Action>(ang, act));
  }

void SimpleScene::SetObjectSize(SS_Object obj, float sz) {
  objects[obj].size = sz;
  }

void SimpleScene::SetObjectTarget(SS_Object obj, float xt, float yt, float zt) {
  //FIXME: Implement!
  }

SS_PType SimpleScene::AddPType() {
  PType ptype;
  ptypes.push_back(ptype);
  return (SS_PType)(ptypes.size() - 1);
  }

void SimpleScene::SetPTypeTexture(SS_PType tp, SimpleTexture *tex) {
  ptypes[(int)(tp)].tex = tex;
  }

void SimpleScene::SetPTypeColor0(
	SS_PType tp, float cr, float cg, float cb, float ca) {
  ptypes[(int)(tp)].cr0 = cr;
  ptypes[(int)(tp)].cg0 = cg;
  ptypes[(int)(tp)].cb0 = cb;
  ptypes[(int)(tp)].ca0 = ca;
  }

void SimpleScene::SetPTypeColor1(
	SS_PType tp, float cr, float cg, float cb, float ca) {
  ptypes[(int)(tp)].cr1 = cr;
  ptypes[(int)(tp)].cg1 = cg;
  ptypes[(int)(tp)].cb1 = cb;
  ptypes[(int)(tp)].ca1 = ca;
  }

void SimpleScene::SetPTypeColor(
	SS_PType tp, float cr, float cg, float cb, float ca) {
  ptypes[(int)(tp)].cr0 = cr;
  ptypes[(int)(tp)].cg0 = cg;
  ptypes[(int)(tp)].cb0 = cb;
  ptypes[(int)(tp)].ca0 = ca;
  ptypes[(int)(tp)].cr1 = cr;
  ptypes[(int)(tp)].cg1 = cg;
  ptypes[(int)(tp)].cb1 = cb;
  ptypes[(int)(tp)].ca1 = ca;
  }

void SimpleScene::SetPTypeVelocity(SS_PType tp, float xv, float yv, float zv) {
  ptypes[(int)(tp)].xv = xv;
  ptypes[(int)(tp)].yv = yv;
  ptypes[(int)(tp)].zv = zv;
  }

void SimpleScene::SetPTypeSize(SS_PType tp, float sz) {
  ptypes[(int)(tp)].sz0 = sz;
  ptypes[(int)(tp)].sz1 = sz;
  }

void SimpleScene::SetPTypeSize0(SS_PType tp, float sz) {
  ptypes[(int)(tp)].sz0 = sz;
  }

void SimpleScene::SetPTypeSize1(SS_PType tp, float sz) {
  ptypes[(int)(tp)].sz1 = sz;
  }

void SimpleScene::SetPTypeDuration(SS_PType tp, Uint32 dur) {
  ptypes[(int)(tp)].dur = dur;
  }

SS_Particle SimpleScene::AddParticle(SS_PType tp) {
  Particle part = { tp };
  parts.push_back(part);
  return (SS_Particle)(parts.size() - 1);
  }

void SimpleScene::SetParticleType(SS_Particle part, SS_PType tp) {
  parts[(int)(part)].type = tp;
  }

void SimpleScene::SetParticlePosition(SS_Particle part, float xp, float yp, float zp) {
  parts[(int)(part)].xp = xp;
  parts[(int)(part)].yp = yp;
  parts[(int)(part)].zp = zp;
  }

void SimpleScene::SetParticleTime(SS_Particle part, Uint32 start) {
  parts[(int)(part)].start = start;
  }

void SimpleScene::Clear() {
  ptypes.clear();
  parts.clear();
  models.clear();
  skins.clear();
  moves.clear();
  objects.clear();
  next_obj = 1;
  }

bool SimpleScene::DrawObjects(Uint32 offset) {
  float xp = 0.0, yp = 0.0, zp = 0.0;

  set<SS_Object> done;
  map<SS_Object, float> prog;
  map<SS_Object, Coord> toward;
  glColor4f(1.0, 1.0, 1.0, 1.0);
  glPushMatrix();
  list<pair<Coord, Action> >::const_iterator obj = moves.begin();
  for(; obj != moves.end(); ++obj) {
    if(done.count(obj->second.obj) > 0) continue;	// Already Drawn

    if(offset < obj->second.finish) {
      if(offset + obj->second.duration < obj->second.finish) continue;
      toward[obj->second.obj] = obj->first;
      prog[obj->second.obj] =
	(float)(offset + obj->second.duration - obj->second.finish)
		/ (float)(obj->second.duration);
      continue;
      }

    done.insert(obj->second.obj);			// Mark as Drawn

    Coord pos = obj->first;
    vector<int> anims;
    vector<Uint32> times;

    anims.push_back(0);
    anims.push_back(0);
    times.push_back(0);
    times.push_back(0);

    anims[0] = models[objects[obj->second.obj].model]->LookUpAnimation("LEGS_IDLE");
    if(anims[0] < 0) anims[0] = models[objects[obj->second.obj].model]->LookUpAnimation("STAND");

    anims[1] = models[objects[obj->second.obj].model]->LookUpAnimation("TORSO_STAND");
    if(anims[1] < 0) anims[1] = models[objects[obj->second.obj].model]->LookUpAnimation("STAND");

    float ang = 0.0;
    { float oang = 0.0, aprog = 0.0;
      list<pair<float, Action> >::const_iterator turn =
	objects[obj->second.obj].turns.begin();
      for(; turn != objects[obj->second.obj].turns.end(); ++turn) {
	if(offset < turn->second.finish) {
	  if(offset + turn->second.duration < turn->second.finish) {
	    continue;
	    }
	  oang = turn->first;
	  aprog = (float)(offset + turn->second.duration - turn->second.finish)
		/ (float)(turn->second.duration);
	  continue;
	  }
	ang = turn->first;
	if(aprog > 0.0) {				// Turning
	  ang = ang * (1.0 - aprog) + oang * aprog;
	  anims[0] = models[objects[obj->second.obj].model]->LookUpAnimation("LEGS_TURN");
	  if(anims[0] < 0) anims[0] = models[objects[obj->second.obj].model]->LookUpAnimation("TURN");
	  }
	break;
	}
      }

    if(prog.count(obj->second.obj) > 0) {		// Moving
      pos.x = (pos.x * (1.0 - prog[obj->second.obj]))
	+ (toward[obj->second.obj].x * prog[obj->second.obj]);
      pos.y = (pos.y * (1.0 - prog[obj->second.obj]))
	+ (toward[obj->second.obj].y * prog[obj->second.obj]);
      pos.z = (pos.z * (1.0 - prog[obj->second.obj]))
	+ (toward[obj->second.obj].z * prog[obj->second.obj]);

      anims[0] = models[objects[obj->second.obj].model]->LookUpAnimation("LEGS_WALK");
      if(anims[0] < 0) anims[0] = models[objects[obj->second.obj].model]->LookUpAnimation("LEGS_RUN");
      if(anims[0] < 0) anims[0] = models[objects[obj->second.obj].model]->LookUpAnimation("WALK");
      if(anims[0] < 0) anims[0] = models[objects[obj->second.obj].model]->LookUpAnimation("RUN");
      }

    vector<Uint32>::const_iterator show;
    show=objects[obj->second.obj].show.begin();
    if(!objects[obj->second.obj].show.empty()) {
      for(; show != objects[obj->second.obj].show.end(); ++show) {
	if(offset >= (*show)) break;
	}
      if(show == objects[obj->second.obj].show.end()) continue;
      }

    if(xlim0 < xlim1 && (
	(pos.x + objects[obj->second.obj].size) < xlim0
	|| (pos.x - objects[obj->second.obj].size) >= xlim1
	)) continue;
    if(ylim0 < ylim1 && (
	(pos.y + objects[obj->second.obj].size) < ylim0
	|| (pos.y - objects[obj->second.obj].size) >= ylim1
	)) continue;
    if(zlim0 < zlim1 && (
	(pos.z + objects[obj->second.obj].size) < zlim0
	|| (pos.z - objects[obj->second.obj].size) >= zlim1
	)) continue;

    if(pos.x != xp || pos.y != yp || pos.z != zp) {
      glPopMatrix();
      glPushMatrix();
      glTranslatef(pos.x, pos.y, pos.z);
      xp = pos.x;
      yp = pos.y;
      zp = pos.z;
      }

    Color col = { 1.0, 1.0, 1.0 };
    if(!objects[obj->second.obj].col.empty()) {
      list<pair<Color, Uint32> >::const_reverse_iterator icol
		= objects[obj->second.obj].col.rbegin();
      for(; icol != objects[obj->second.obj].col.rend(); ++icol) {
	if(offset >= icol->second) {
	  col = icol->first;
	  break;
	  }
	}
      }

    if(col.r != 1.0 || col.g != 1.0 || col.b != 1.0) {
      glColor4f(col.r, col.g, col.b, 1.0);
      }
    if(objects[obj->second.obj].size != 1.0) {
      glPushMatrix();
      glScalef(objects[obj->second.obj].size, objects[obj->second.obj].size, objects[obj->second.obj].size);
      }
    if(ang != 0.0) {
      glPushMatrix();
      glRotatef(ang, 0.0, 0.0, 1.0);
      }

    if(objects[obj->second.obj].skin != SS_UNDEFINED_SKIN) {
      glBindTexture(GL_TEXTURE_2D, skins[objects[obj->second.obj].skin]->GLTexture());
      }
    if(objects[obj->second.obj].model != SS_UNDEFINED_MODEL) {
      models[objects[obj->second.obj].model]->Render(offset, anims, times);
      }

    if(ang != 0.0) {
      glPopMatrix();
      }
    if(objects[obj->second.obj].size != 1.0) {
      glPopMatrix();
      }
    if(col.r != 1.0 || col.g != 1.0 || col.b != 1.0) {
      glColor4f(1.0, 1.0, 1.0, 1.0);
      }
    }  
  glPopMatrix();
  return true;
  }

bool SimpleScene::DrawParticles(Uint32 offset) {
  if(parts.size() < 1) { return true; }
  GLuint tex = ptypes[parts[0].type].tex->GLTexture();
  glBindTexture(GL_TEXTURE_2D, tex);

  //Prep for billboard transformation
  float view[16];
  glGetFloatv(GL_MODELVIEW_MATRIX, view);

  glDisable(GL_LIGHTING);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  glDepthMask(GL_FALSE);
  glBegin(GL_QUADS);

  vector<Particle>::iterator part = parts.begin();
  for(; part != parts.end(); ++part) {
    PType *type = &(ptypes[part->type]);
    if(part->start <= offset && (part->start + type->dur) > offset) {
      float tm, sz, cr, cg, cb, ca, xp, yp, zp, xxo, xyo, yxo, yyo, zxo, zyo;
      tm = (offset - part->start) / float(type->dur);

      sz = type->sz1 * tm + type->sz0 * (1.0 - tm);
      cr = type->cr1 * tm + type->cr0 * (1.0 - tm);
      cg = type->cg1 * tm + type->cg0 * (1.0 - tm);
      cb = type->cb1 * tm + type->cb0 * (1.0 - tm);
      ca = type->ca1 * tm + type->ca0 * (1.0 - tm);

      xp = part->xp + type->xv * (offset - part->start) / 1000.0;
      yp = part->yp + type->yv * (offset - part->start) / 1000.0;
      zp = part->zp + type->zv * (offset - part->start) / 1000.0;
      xxo = sz * view[0];  yxo = sz * view[4];  zxo = sz * view[8];
      xyo = sz * view[1];  yyo = sz * view[5];  zyo = sz * view[9];

      if(tex != type->tex->GLTexture()) {
	glEnd();
	tex = type->tex->GLTexture();
	glBindTexture(GL_TEXTURE_2D, tex);
	glBegin(GL_QUADS);
        }

      glColor4f(cr, cg, cb, ca);
      glTexCoord2f(type->tex->ScaleX(1.0), type->tex->ScaleY(0.0));
      glVertex3f(xp + xxo - xyo, yp + yxo - yyo, zp + zxo - zyo);
      glTexCoord2f(type->tex->ScaleX(1.0), type->tex->ScaleY(1.0));
      glVertex3f(xp + xxo + xyo, yp + yxo + yyo, zp + zxo + zyo);
      glTexCoord2f(type->tex->ScaleX(0.0), type->tex->ScaleY(1.0));
      glVertex3f(xp - xxo + xyo, yp - yxo + yyo, zp - zxo + zyo);
      glTexCoord2f(type->tex->ScaleX(0.0), type->tex->ScaleY(0.0));
      glVertex3f(xp - xxo - xyo, yp - yxo - yyo, zp - zxo - zyo);
      }
    }

  glEnd();
  glDepthMask(GL_TRUE);
  glDisable(GL_BLEND);

  return true;
  }

void SimpleScene::RestrictX(float x0, float x1) {
  resx0 = x0;
  resx1 = x1;
  }

void SimpleScene::RestrictY(float y0, float y1) {
  resy0 = y0;
  resy1 = y1;
  }

void SimpleScene::RestrictZ(float z0, float z1) {
  resz0 = z0;
  resz1 = z1;
  }
