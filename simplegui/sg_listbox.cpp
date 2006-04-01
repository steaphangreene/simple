// *************************************************************************
//  This file is part of the SimpleGUI Example Module by Steaphan Greene
//
//  Copyright 2005-2006 Steaphan Greene <stea@cs.binghamton.edu>
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

SG_ListBox::SG_ListBox(const vector<SG_Widget*>& items, SimpleTexture desel,
			SimpleTexture sel, SimpleTexture click, SimpleTexture dis, unsigned int min, unsigned int max, bool vert,
			float border)
	: SG_Compound( (!vert?(items.size()==0?1:items.size()):1), (vert?(items.size()==0?1:items.size()):1), 0, 0), 
	  texdesel(desel), texsel(sel), texclick(click), texdisable(dis) {
  /* Copy the passed data into our class data */
  minsel = min;
  maxsel = max;
  vertical = vert;
  listsize=items.size();
  alignborder=border;
 	 
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

void SG_ListBox::AddItem( SG_Widget* w, int at ) {
  if( at != -1 && at < 0 ) return; //Invalid argument
  
  unsigned int selcnt;
  int selected_items[ selcnt=selhistory.size() ];
  
  SG_Alignment* a = new SG_Alignment(alignborder,alignborder);
  a->Enable();
  SG_StickyButton* sb = new SG_StickyButton("",texdesel,texdisable,texclick,texsel);
  a->SetBackground(sb);
  a->AddWidget(w);

  if( at == -1 || at >= (signed)listsize) {
    aligns.push_back(a);
    stickies.push_back(sb);
    if( vertical ) AddRow(listsize); else AddCol(listsize);
    listsize++;
    ptr2pos[ stickies[listsize-1] ] = listsize-1;
    if( vertical ) AddWidget(a, 0, listsize-1); else AddWidget(a, listsize-1,0); 
    } else {
    // Dump selection history into local array
    deque<int>::iterator itr = selhistory.begin();
    for(unsigned int i=0; i<selcnt; i++,++itr) {
      selected_items[ i ] = (*itr);
      if( selected_items[i] >= at ) selected_items[i]++; //Shift forward
      }
    //Clear selections
    while(Deselect(true));
	    
    //Otherwise... insert in middle
    aligns.insert(aligns.begin()+at,a);
    stickies.insert(stickies.begin()+at,sb);
    listsize++;
    for(unsigned int i=at; i<listsize; i++) ptr2pos[ stickies[i] ] = i;

    if( vertical ) {
      AddRow(at);
      AddWidget(a,0,at);
      } else {
      AddCol(at);
      AddWidget(a,at,0);
      }
    
    //Replace selection
    for(unsigned int i=0; i<selcnt; i++) {
      Select(selected_items[i]);
      }
    }
  
  }

  
bool SG_ListBox::RemoveItem( unsigned int item ) {
  unsigned int i;
	
  /* Range Check */
  if( item >= listsize ) return false;

  /* Minimum Selection Is Entire List Removal Impossible */
  if( selhistory.size() == minsel && listsize == minsel ) return false;

  /* Unselect Item (doesn't hurt if it was not selected) */
  Deselect( true, item );

  /* Remove Item */
  if( vertical ) {
    RemoveRow(item);
    } else {
    RemoveCol(item);	  
    }
	
  stickies.erase(stickies.begin()+item);
  aligns.erase(aligns.begin()+item);
  for(i=item; i<listsize; i++) {
    ptr2pos[ stickies[i] ] = i; 
    }
  listsize--;

  /* Ensure Minimum Selection */
  i=0;
  while( i < listsize && selhistory.size() < minsel ) {
    Select(i);
    i--;
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

