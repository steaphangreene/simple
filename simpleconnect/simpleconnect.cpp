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

#include "simpleconnect.h"
#include "../simplegui/sg_events.h"
#include "../simplegui/sg_stickybutton.h"

SimpleConnect::SimpleConnect() : SG_Compound(8, 1, 0.1, 0.1) {
  mode = SC_MODE_NONE;
  prop_flags = 0;
  change_flags = 0;

  background = new SG_Panel(SG_COL_FG);
  for(unsigned int slot = 0; slot < slots.size(); ++slot) {
    SG_StickyButton *meb = new SG_StickyButton("Me");
    SG_TextArea *name = NULL;
    if(slot == 0) {
      meb->TurnOn();
      name = new SG_TextArea("Local Player");
      }
    else {
      name = new SG_TextArea("AI Player");
      }
    AddWidget(name, 4, slot+1, 4, 1);
    AddWidget(meb, 0, slot+1, 1, 1);
    }
  SG_Widget *labelb =
	new SG_TextArea("SimpleConnect", SG_COL_LOW);
  AddWidget(labelb, 0, 0, 6, 1);
  okb = new SG_Button("Ok", SG_COL_RAISED, SG_COL_LOW);
  AddWidget(okb, 7, 0, 1, 1);

  port = DEFAULT_PORT;

  net_thread = NULL;
  net_mutex = NULL;
  exiting = false;
  }

SimpleConnect::~SimpleConnect() {
  CleanupNet();
  }

void SimpleConnect::Host(const string &tag, const vector<SC_SlotType> &slts) {
  CleanupNet();
  slots = slts;
  nettag = tag;
  mode = SC_MODE_HOST;
  StartNet();
  }

void SimpleConnect::Search(const string &tag) {
  CleanupNet();
  nettag = tag;
  mode = SC_MODE_SEARCH;
  StartNet();
  }

void SimpleConnect::Config(const vector<SC_SlotType> &slts) {
  CleanupNet();
  slots = slts;
  mode = SC_MODE_CONFIG;
  }

void SimpleConnect::Reset() {
  CleanupNet();
  slots.clear();
  mode = SC_MODE_NONE;
  }

bool SimpleConnect::ChildEvent(SDL_Event *event) {
  if(event->user.code == SG_EVENT_BUTTONPRESS) {
    if(event->user.data1 == (void *)(okb)) {
      event->user.code = SG_EVENT_OK;
      event->user.data1 = (void*)this;
      event->user.data2 = NULL;
      return 1;
      }
    }
  return 0; // Silence children doing other things
  }

//  bool SimpleConnect::SetDefaultCursor(GL_MODEL *cur);
  
//  static GL_MODEL SimpleConnect::Default_Mouse_Cursor = NULL;

struct DataPacket {
  Uint32 act;
  };

int SimpleConnect::HandleNetThread() {
  UDPpacket *inpacket = SDLNet_AllocPacket(sizeof(DataPacket));
  DataPacket *indata = (DataPacket*)(inpacket->data);
  UDPpacket *outpacket = SDLNet_AllocPacket(sizeof(DataPacket));
  DataPacket *outdata = (DataPacket*)(outpacket->data);
  outpacket->len = sizeof(DataPacket);

  UDPsocket udpsock;
  if(mode == SC_MODE_SEARCH) udpsock = SDLNet_UDP_Open(0);
  else if(mode == SC_MODE_HOST) udpsock = SDLNet_UDP_Open(port);
  else {
    fprintf(stderr, "ERROR: Invalid mode in SimpleConnect network handler!\n");
    }

  if(!udpsock) {
    fprintf(stderr, "ERROR: SDLNet_UDP_Open Failed: %s\n", SDLNet_GetError());
    exiting = true;
    return 1;
//    exit(1);
    }

  IPaddress broadcast_address = {0};
//  SDLNet_ResolveHost(&broadcast_address, "255.255.255.255", port);
  SDLNet_ResolveHost(&broadcast_address, "10.0.0.10", port);

  //Temporary!
  if(mode == SC_MODE_SEARCH) {
    outpacket->address = broadcast_address;
    outdata->act = 0;
    if(SDLNet_UDP_Send(udpsock, -1, outpacket) < 1) {
      fprintf(stderr, "ERROR: SDLNet_UDP_Send Failed: %s\n", SDLNet_GetError());
      exiting = true;
      return 1;
//      exit(1);
      }
    }
  else if(mode == SC_MODE_HOST) {
    outdata->act = 1;
    }

  while(!exiting) {
    while(SDLNet_UDP_Recv(udpsock, inpacket) > 0) {
      fprintf(stderr, "Got packet type %d\n", indata->act);
      }
    SDL_Delay(10);
    }

  SDLNet_UDP_Close(udpsock);

  SDLNet_FreePacket(inpacket);
  SDLNet_FreePacket(outpacket);
  return 0;
  }

int SimpleConnect::thread_handler(void *me) {
  return ((SimpleConnect*)(me))->HandleNetThread();
  }

void SimpleConnect::CleanupNet() {
  exiting = true;
  if(net_thread) SDL_WaitThread(net_thread, NULL);
  net_thread = NULL;
  if(net_mutex) SDL_DestroyMutex(net_mutex);
  net_mutex = NULL;
  }

void SimpleConnect::StartNet() {
  exiting = false;
  net_mutex = SDL_CreateMutex();
  net_thread = SDL_CreateThread(thread_handler, (void*)(this));
  }
