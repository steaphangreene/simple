// *************************************************************************
//  This file is part of the SimpleModel Example Module by Steaphan Greene
//
//  Copyright 2005 Steaphan Greene <stea@cs.binghamton.edu>
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

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include <cstdio>
#include <cstdlib>
using namespace std;

#include "sm_q3anim.h"
#include "simplemodel_q3dir.h"

SimpleModel_Q3Dir::SimpleModel_Q3Dir(const string &filenm) {
  head = NULL;
  torso = NULL;
  legs = NULL;
  weapon = NULL;
  Load(filenm);
  }

SimpleModel_Q3Dir::SimpleModel_Q3Dir() {
  head = NULL;
  torso = NULL;
  legs = NULL;
  weapon = NULL;
  }

SimpleModel_Q3Dir::~SimpleModel_Q3Dir() {
  if(head) delete head;
  if(torso) delete torso;
  if(legs) delete legs;

  if(weapon) weapon = NULL;	// Not mine!
  }

bool SimpleModel_Q3Dir::Load(const string &filenm) {
  filename = filenm;
  head = new SimpleModel_MD3(
	filename, filename + "/head.md3", filename + "/head_default.skin");
  torso = new SimpleModel_MD3(
	filename, filename + "/upper.md3", filename + "/upper_default.skin");
  legs = new SimpleModel_MD3(
	filename, filename + "/lower.md3", filename + "/lower_default.skin");

  if(!LoadCFG(filename + "/animation.cfg")) return false;

  SetAnimation(TORSO_STAND);
  SetAnimation(LEGS_IDLE);

  return false;
  }

bool SimpleModel_Q3Dir::LoadCFG(const string &filenm) {
    FILE *cfg = fopen(filenm.c_str(), "r");
    if(!cfg) {
      fprintf(stderr, "WARNING: Could not open model animations file '%s'!\n",
	filenm.c_str());
      return false;
      }

    int start=0, num=0, loop=0, fps=0;
    int torso_first = -1, legs_offset = -1;
    for(int anim = ANIM_START; anim < ANIM_MAX; ++anim) {
      while(fscanf(cfg, "%d %d %d %d %*[^\n]", &start, &num, &loop, &fps) < 4) {
	fscanf(cfg, "%*[^\n]");	//Skip irrelevant lines
	}
      if(anim < BOTH_MAX) {
        torso->AddAnimation(start, start + num, loop, fps);
	legs->AddAnimation(start, start + num, loop, fps);
        }
      else if(anim < TORSO_MAX) {
	if(torso_first < 0) torso_first = start;
        torso->AddAnimation(start, start + num, loop, fps);
        }
      else if(anim < LEGS_MAX) {
	if(legs_offset < 0) legs_offset = start - torso_first;
	start -= legs_offset;
	legs->AddAnimation(start, start + num, loop, fps);
        }
      else {
	fprintf(stderr, "WARNING: Too many animations for Q3 in '%s'!\n",
		filenm.c_str());
	}
      }
    fclose(cfg);
  return true;
  }

bool SimpleModel_Q3Dir::Render(Uint32 cur_time, int anim, Uint32 start_time) {
  //FIXME: Interpolate Models Using Animations
  //FIXME: Interpolate Tags
  if(legs) {
    glPushMatrix();
    glScalef(0.0625, 0.0625, 0.0625);
    legs->Render(cur_time);
    if(torso && legs->MoveToTag("tag_torso")) {
      torso->Render(cur_time);
      glPushMatrix();
      if(head && torso->MoveToTag("tag_head")) {
	head->Render(cur_time);
	}
      glPopMatrix();
      if(weapon && torso->MoveToTag("tag_weapon")) {
	weapon->Render(cur_time);
	}
      }
    glPopMatrix();
    return true;
    }
  return false;
  }

void SimpleModel_Q3Dir::SetAnimation(int anim) {
  if(anim < BOTH_MAX) {
    torso->SetAnimation(anim);
    legs->SetAnimation(anim);
    }
  else if(anim < TORSO_MAX) {
    torso->SetAnimation(anim);
    if(legs->GetAnimation() < BOTH_MAX)
	legs->SetAnimation(LEGS_IDLE - (LEGS_START - TORSO_START));
    }
  else if(anim < LEGS_MAX) {
    anim -= (LEGS_START - TORSO_START);
    legs->SetAnimation(anim);
    if(torso->GetAnimation() < BOTH_MAX) torso->SetAnimation(TORSO_STAND);
    }
  }

int SimpleModel_Q3Dir::GetAnimation() {
  return 0;
  }

void SimpleModel_Q3Dir::SetWeapon(SimpleModel_MD3 *weap) {
  weapon = weap;
  }
