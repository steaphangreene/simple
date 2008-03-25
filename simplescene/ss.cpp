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
  const Coord pos = { 0.0, 0.0, 0.0 };
  const Object obj = { mod, skin, 1.0, 1.0, 1.0, 1.0, 0.0 };
  multimap<Coord, Object>::iterator newobj =
	objects.insert(pair<Coord, Object>(pos, obj));
  objlist[next_obj] = newobj;
  return next_obj++;
  }

void SimpleScene::ObjectAct(SS_Object obj,
	SS_Action act, Uint32 fin, Uint32 dur) {
  Action newact = { act, fin, dur };
  objlist[obj]->second.acts.push_back(newact);
  }

void SimpleScene::SetObjectSkin(SS_Object obj, SS_Skin skin) {
  objlist[obj]->second.skin = skin;
  }

void SimpleScene::SetObjectModel(SS_Object obj, SS_Model mod) {
  objlist[obj]->second.model = mod;
  }

void SimpleScene::SetObjectColor(SS_Object obj, float r, float g, float b) {
  objlist[obj]->second.r = r;
  objlist[obj]->second.g = g;
  objlist[obj]->second.b = b;
  }

void SimpleScene::SetObjectPosition(SS_Object obj, 
	float xp, float yp, float zp) {
  Coord pos = { xp, yp, zp };
  multimap<Coord, Object>::iterator tmp = objlist[obj];
  multimap<Coord, Object>::iterator newobj =
	objects.insert(pair<Coord, Object>(pos, tmp->second));
  objlist[obj] = newobj;
  objects.erase(tmp);
  }

void SimpleScene::SetObjectRotation(SS_Object obj, float ang) {
  objlist[obj]->second.ang = ang;
  }

void SimpleScene::SetObjectSize(SS_Object obj, float sz) {
  objlist[obj]->second.size = sz;
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
  }

bool SimpleScene::DrawObjects(Uint32 offset) {
  float xp = 0.0, yp = 0.0, zp = 0.0;

  glPushMatrix();
  multimap<Coord, Object>::const_iterator obj = objects.begin();
  for(; obj != objects.end(); ++obj) {
    if(obj->second.acts.size() > 0 && (
	offset >= obj->second.acts.begin()->finish
	|| offset + obj->second.acts.begin()->duration < obj->second.acts.begin()->finish)) {
      continue;
      }

    if(resx0 < resx1 && (obj->first.x < resx0 || obj->first.x >= resx1)) continue;
    if(resy0 < resy1 && (obj->first.y < resy0 || obj->first.y >= resy1)) continue;
    if(resz0 < resz1 && (obj->first.z < resz0 || obj->first.z >= resz1)) continue;

    if(obj->first.x != xp || obj->first.y != yp || obj->first.z != zp) {
      glPopMatrix();
      glPushMatrix();
      glTranslatef(obj->first.x, obj->first.y, obj->first.z);
      xp = obj->first.x;
      yp = obj->first.y;
      zp = obj->first.z;
      }
    if(obj->second.r != 1.0 || obj->second.g != 1.0 || obj->second.b != 1.0) {
      glColor4f(obj->second.r, obj->second.g, obj->second.b, 1.0);
      }
    if(obj->second.size != 1.0) {
      glPushMatrix();
      glScalef(obj->second.size, obj->second.size, obj->second.size);
      }
    if(obj->second.ang != 0.0) {
      glPushMatrix();
      glRotatef(obj->second.ang, 0.0, 0.0, 1.0);
      }

    if(obj->second.skin != SS_UNDEFINED_SKIN) {
      glBindTexture(GL_TEXTURE_2D, skins[obj->second.skin]->GLTexture());
      }
    if(obj->second.model != SS_UNDEFINED_MODEL) {
      models[obj->second.model]->Render(offset);
      }

    if(obj->second.ang != 0.0) {
      glPopMatrix();
      }
    if(obj->second.size != 1.0) {
      glPopMatrix();
      }
    if(obj->second.r != 1.0 || obj->second.g != 1.0 || obj->second.b != 1.0) {
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
