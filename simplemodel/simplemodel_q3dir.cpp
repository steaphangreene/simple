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

bool SimpleModel_Q3Dir::Render(Uint32 cur_time, const vector<int> &anim,
	const vector<Uint32> &start_time) const {
  //FIXME: Interpolate Models Using Animations
  //FIXME: Interpolate Tags

  if(anim.size() < 2 || start_time.size() < 2) {
    fprintf(stderr, "WARNING: Not enough anims/times sent to Q3 renderer.\n");
    return false;
    }

  vector<int> leganim(1);
  vector<Uint32> legtime(1);
  vector<int> torsoanim(1);
  vector<Uint32> torsotime(1);

  leganim[0] = anim[0];
  legtime[0] = start_time[0];
  torsoanim[0] = anim[1];
  torsotime[0] = start_time[1];

  //Do some auto-correction for not-quite valid inputs
  if(torsotime[0] >= legtime[0] && torsoanim[0] < BOTH_MAX) { 
    leganim[0] = torsoanim[0];
    legtime[0] = torsotime[0];
    }
  else if(legtime[0] >= torsotime[0] && leganim[0] < BOTH_MAX) {
    torsoanim[0] = leganim[0];
    torsotime[0] = legtime[0];
    }
  else if(torsoanim[0] < BOTH_MAX) { 
    torsoanim[0] = TORSO_STAND;
    torsotime[0] = legtime[0];
    }
  else if(leganim[0] < BOTH_MAX) {
    leganim[0] = LEGS_IDLE;
    legtime[0] = torsotime[0];
    }

  if(leganim[0] >= BOTH_MAX) {	//Adjust for Leg Offset
    leganim[0] -= (LEGS_START - TORSO_START);
    }

  if(legs) {
    glPushMatrix();
    glTranslatef(0.0, 0.0, 1.5);	//Adjust for floor height
    glScalef(0.0625, 0.0625, 0.0625);

    legs->Render(cur_time, leganim, legtime);

    if(torso && legs->MoveToTag("tag_torso", cur_time, leganim, legtime)) {
      torso->Render(cur_time, torsoanim, torsotime);

      glPushMatrix();
      if(head && torso->MoveToTag("tag_head", cur_time, torsoanim, torsotime)) {
	head->Render(cur_time);	//No animations
	}
      glPopMatrix();

      if(weapon && torso->MoveToTag("tag_weapon", cur_time, torsoanim, torsotime)) {
	weapon->Render(cur_time); //No animations
	}
      }
    glPopMatrix();
    return true;
    }
  return false;
  }

void SimpleModel_Q3Dir::SetAnimation(int anim) {
  fprintf(stderr, "WARNING: Calling unsupported old function SetAnimation()\n");
  }

int SimpleModel_Q3Dir::GetAnimation() {
  fprintf(stderr, "WARNING: Calling unsupported old function GetAnimation()\n");
  return 0;
  }

void SimpleModel_Q3Dir::SetWeapon(SimpleModel_MD3 *weap) {
  weapon = weap;
  }
