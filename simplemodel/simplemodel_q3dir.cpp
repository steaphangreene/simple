// *************************************************************************
//  This file is part of the SimpleModel Example Module by Steaphan Greene
//
//  Copyright 2005-2006 Steaphan Greene <stea@cs.binghamton.edu>
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
using namespace std;

#include "sm_q3anim.h"
#include "simplemodel_q3dir.h"

SimpleModel_Q3Dir::SimpleModel_Q3Dir(
	const string &filenm, const string &defskin) {
  head = NULL;
  torso = NULL;
  legs = NULL;
  weapon = NULL;
  Load(filenm, defskin);
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

bool SimpleModel_Q3Dir::Load(const string &filenm, const string &defskin) {
  filename = filenm;
  head = new SimpleModel_MD3(
	filename, filename + "/head.md3", defskin);
  torso = new SimpleModel_MD3(
	filename, filename + "/upper.md3", defskin);
  legs = new SimpleModel_MD3(
	filename, filename + "/lower.md3", defskin);

  if(!LoadCFG(filename + "/animation.cfg")) return false;

  return false;
  }

bool SimpleModel_Q3Dir::LoadCFG(const string &filenm) {
  SDL_RWops *cfg = SDL_RWFromFile(filenm.c_str(), "rb");
  if(!cfg) {
    fprintf(stderr, "WARNING: Could not open model animations file '%s'!\n",
	filenm.c_str());
    return false;
    }

  SDL_RWseek(cfg, 0, SEEK_END);
  int filesz = SDL_RWtell(cfg);
  SDL_RWseek(cfg, 0, SEEK_SET);
  char *filedata = new char[filesz+1]; //FIXME: Handle Error!
  char *fileptr = filedata;
  while(fileptr < (filedata+filesz)) {
    fileptr += SDL_RWread(cfg, fileptr, 1, filesz);
    //FIXME: Handle Error!
    }
  *fileptr = 0;
  SDL_RWclose(cfg);
  fileptr = filedata;

  int start=0, num=0, loop=0, fps=0;
  int torso_first = -1, legs_offset = -1;
  for(int anim = ANIM_START; anim < ANIM_MAX; ++anim) {
    while(sscanf(fileptr, "%d %d %d %d", &start, &num, &loop, &fps) < 4) {
      //Skip irrelevant lines
      while((*fileptr) && (*fileptr) != '\n') ++fileptr;
      if((*fileptr)) ++fileptr;
      }

    //Advance to next line
    while((*fileptr) && isspace(*fileptr)) ++fileptr;
    while((*fileptr) && isdigit(*fileptr)) ++fileptr;
    while((*fileptr) && isspace(*fileptr)) ++fileptr;
    while((*fileptr) && isdigit(*fileptr)) ++fileptr;
    while((*fileptr) && isspace(*fileptr)) ++fileptr;
    while((*fileptr) && isdigit(*fileptr)) ++fileptr;
    while((*fileptr) && isspace(*fileptr)) ++fileptr;
    while((*fileptr) && isdigit(*fileptr)) ++fileptr;
    while((*fileptr) && (*fileptr) != '\n') ++fileptr;

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

  delete [] filedata;
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
    glColor3f(1.0, 1.0, 1.0);
    glEnable(GL_TEXTURE_2D);
    glScalef(0.0625, 0.0625, 0.0625);
    glTranslatef(0.0, 0.0, 24.0);	//Adjust for floor height

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

void SimpleModel_Q3Dir::SetWeapon(SimpleModel_MD3 *weap) {
  weapon = weap;
  }

const vector<string> &SimpleModel_Q3Dir::GetSkinList() {
  return skins;
  }

void SimpleModel_Q3Dir::AddSkin(const string &skinnm) {
  skins.push_back(skinnm);
  }
