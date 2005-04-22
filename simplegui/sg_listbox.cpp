// *************************************************************************
//  This file is part of the SimpleGUI Example Module by Steaphan Greene
//
//  Copyright 2005 Steaphan Greene <stea@cs.binghamton.edu>
//
//  SimpleGUI is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  SimpleGUI is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SimpleGUI (see the file named "COPYING");
//  if not, write to the the Free Software Foundation, Inc.,
//  59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
// *************************************************************************

#include "SDL_opengl.h"

#include "sg_listbox.h"
#include "sg_panel.h"
#include "sg_button.h"
#include "sg_events.h"

SG_ListBox::SG_ListBox(const vector<SG_Widget*>& items, SG_Texture desel,
			SG_Texture sel, SG_Texture click, SG_Texture dis, unsigned int min, unsigned int max, bool vert,
			float border)
	: SG_Compound( (!vert?(items.size()==0?1:items.size()):1), (vert?(items.size()==0?1:items.size()):1), 0, 0) {
  /* Copy the passed data into our class data */
  minsel = min;
  maxsel = max;
  vertical = vert;
  listsize=items.size();
		
  /* Sanity Check */
  if( min > max ) {
    fprintf(stderr,"SG_ListBox Constructor: Invalid min %d because it is greater than max %d\n",min,max);
    exit(1);
  }
		
  /* Add SG_Widget* to table slots */
  SG_Alignment* align;
  for(unsigned int i=0; i<listsize; i++) {
    stickies.push_back( new SG_StickyButton("",desel,dis,click,sel) );
    
    align = new SG_Alignment(border,border);
    align->Enable(); //Listen for mouse clicks

    align->SetBackground(stickies[i]);
    
    //Add it to the vector of SG_Alignments
    aligns.push_back(align);	
    if( i < minsel ) {	  
      Select(i);
      }
    
    align->AddWidget(items[i]);
    ptr2pos[stickies[i]]=i;
    AddWidget(align); 
    }
  }

SG_ListBox::~SG_ListBox() {
  }

bool SG_ListBox::ChildEvent(SDL_Event *event) {

  if(event->user.code == SG_EVENT_STICKYOFF) {
    if( ptr2pos.count((SG_StickyButton*)(event->user.data1)) == 1 ) {
      int ind = ptr2pos[ (SG_StickyButton*)(event->user.data1) ];
      if(!Deselect(false,ind)) {
	//Unselection failed to abide by the rules, reselect it
	((SG_StickyButton*)(event->user.data1))->TurnOn();
	return 0;
        } 
      else {
	event->user.code = SG_EVENT_UNSELECT_ITEM;
	event->user.data1= (void*)(SG_StickyButton*)this;
	event->user.data2= NULL;
	return 1;
        }		
      }
    }
  
  if(event->user.code == SG_EVENT_STICKYON) {
    if( ptr2pos.count((SG_StickyButton*)(event->user.data1)) == 1 ) {
      int ind = ptr2pos[ (SG_StickyButton*)(event->user.data1) ];
      Select(ind);

      event->user.code = SG_EVENT_SELECT_ITEM;
      event->user.data1=(void*)(SG_StickyButton*)this;
      event->user.data2=NULL;
      return 1;
      }
    }

  return 0; // Silence children doing other things
  }

bool SG_ListBox::SetSelection(const set<int>& toselect) {
  if( toselect.size() < minsel || toselect.size() > maxsel ) return false;

  //unselect everything
  while ( Deselect(true) );
  
  //select the items in the set
  set<int>::iterator iter = toselect.begin();
  for(;iter!=toselect.end(); ++iter) {
    Select( (*iter) );
    }
  
  return true;
  }

//  bool SG_ListBox::SetDefaultCursor(GL_MODEL *cur);

void SG_ListBox::Select(int ind) {
  if( ind >= 0 || ind < (signed)listsize ) {
    //If the maximum is already selected remove one
    if( selhistory.size() == maxsel ) Deselect(true);	
    stickies[ind]->TurnOn();
    selhistory.push_back(ind);
    }
  else {
    fprintf(stderr,"Invalid Select(%d) call... not in [0-%d] bounds\n",ind,listsize);
    exit(1);
    }
  }

bool SG_ListBox::Deselect(bool override, int ind) {
  //Check to make sure there is room to remove one
  if( !override && selhistory.size() == minsel ) return false;
  
  if( ind == -1 && selhistory.size() > 0 ) {
    //Remove the oldest selected item
    ind=selhistory.front();
    selhistory.pop_front();
    stickies[ind]->TurnOff();
    return true;
    } else if( ind == -1 ) return false;
  
  if( ind >= 0 || ind < (signed)listsize ) {
    selhistory.erase(remove( selhistory.begin(), selhistory.end(), ind));
    stickies[ind]->TurnOff();
    return true;
    }
  else {
    fprintf(stderr,"Invalid Deselect(%d) cal... not in [0-%d] bounds\n",ind,listsize);
    exit(1);
    //Redudant, yes.
    return false;
    }
  }
//  static GL_MODEL SG_ListBox::Default_Mouse_Cursor = NULL;

