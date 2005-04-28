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

#include <set>
using namespace std;

#include "SDL_opengl.h"

#include "simpleconnect.h"
#include "../simplegui/sg_events.h"
#include "../simplegui/sg_stickybutton.h"

enum SCAct {
  SC_ACT_QUERYING = 0,
  SC_ACT_HOSTING,
  SC_ACT_LEAVING,
  SC_ACT_MAX
  };

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

bool SimpleConnect::Render(unsigned long cur_time) {
  if(mode == SC_MODE_SEARCH) {
    SDL_mutexP(net_mutex);
    bool changed = false;
    map<Uint64, SC_Host>::iterator host = hosts.begin();
    for(; host != hosts.end(); ++host) {
      if(host->second.changed) {
	host->second.changed = false;
	if(!changed) {
	  int ysz = ysize;
	  Resize(8, 1);		//Clear the discovery widgets
	  Resize(8, ysz);
	  changed = true;
	  }
	if(host->second.line < 0) {
	  host->second.line = (ysize - 1);
	  Resize(xsize, ysize+1);
	  }
	AddWidget(new SG_TextArea(SDLNet_ResolveIP(&(host->second.address))),
		0, 1 + host->second.line, 8, 1);
	}
      }
    SDL_mutexV(net_mutex);
    }
  return SG_Compound::Render(cur_time);
  }

void SimpleConnect::Host(const string &tag, const vector<SC_SlotType> &slts) {
  CleanupNet();
  slots = slts;
  nettag = tag;
  mode = SC_MODE_HOST;
  Resize(8, 1);		//Clear any list widgets
  StartNet();
  }

void SimpleConnect::Search(const string &tag) {
  CleanupNet();
  nettag = tag;
  mode = SC_MODE_SEARCH;
  Resize(8, 1);		//Clear any list widgets
  StartNet();
  }

void SimpleConnect::Config(const vector<SC_SlotType> &slts) {
  CleanupNet();
  slots = slts;
  mode = SC_MODE_CONFIG;
  Resize(8, 1);		//Clear any list widgets
  }

void SimpleConnect::Reset() {
  CleanupNet();
  slots.clear();
  mode = SC_MODE_NONE;
  Resize(8, 1);		//Clear any list widgets
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
  Uint8 act;
  Sint8 tag[1];
  };

int SimpleConnect::HandleSearchThread() {
  UDPpacket *inpacket =
	SDLNet_AllocPacket(sizeof(DataPacket) + nettag.length());
  DataPacket *indata = (DataPacket*)(inpacket->data);

  UDPpacket *outpacket =
	SDLNet_AllocPacket(sizeof(DataPacket) + nettag.length());
  DataPacket *outdata = (DataPacket*)(outpacket->data);
  outpacket->len = sizeof(DataPacket) + nettag.length();
  strcpy((char*)outdata->tag, nettag.c_str());

  UDPsocket udpsock = NULL;
  udpsock = SDLNet_UDP_Open(0);
  if(!udpsock) {
    fprintf(stderr, "ERROR: SDLNet_UDP_Open Failed: %s\n", SDLNet_GetError());
    exiting = true;
    return 1;
    }

  IPaddress broadcast_address = {0};
  SDLNet_ResolveHost(&broadcast_address, "255.255.255.255", port);

  outpacket->address = broadcast_address;
  outdata->act = SC_ACT_QUERYING;
  if(SDLNet_UDP_Send(udpsock, -1, outpacket) < 1) {
    fprintf(stderr, "ERROR: SDLNet_UDP_Send Failed: %s\n", SDLNet_GetError());
    exiting = true;
    return 1;
    }

  while(!exiting) {
    while(SDLNet_UDP_Recv(udpsock, inpacket) > 0) {
      if(!strcmp((char*)indata->tag, nettag.c_str())) {
	if(indata->act == SC_ACT_HOSTING) {
	  SDL_mutexP(net_mutex);
	  Uint64 entry = ((Uint64)(inpacket->address.host)) << 16
		| ((Uint64)(inpacket->address.port));
	  hosts[entry].address = inpacket->address;
	  hosts[entry].map = "Unknown";
	  hosts[entry].changed = true;
	  SDL_mutexV(net_mutex);
	  }
	}
      }
    SDL_Delay(10);
    }

  SDLNet_UDP_Close(udpsock);

  SDLNet_FreePacket(inpacket);
  SDLNet_FreePacket(outpacket);
  return 0;
  }

int SimpleConnect::HandleHostThread() {
  UDPpacket *inpacket =
	SDLNet_AllocPacket(sizeof(DataPacket) + nettag.length());
  DataPacket *indata = (DataPacket*)(inpacket->data);

  UDPpacket *outpacket =
	SDLNet_AllocPacket(sizeof(DataPacket) + nettag.length());
  DataPacket *outdata = (DataPacket*)(outpacket->data);
  outpacket->len = sizeof(DataPacket) + nettag.length();
  strcpy((char*)outdata->tag, nettag.c_str());

  UDPsocket udpsock = NULL;
  udpsock = SDLNet_UDP_Open(port);
  if(!udpsock) {
    fprintf(stderr, "ERROR: SDLNet_UDP_Open Failed: %s\n", SDLNet_GetError());
    exiting = true;
    return 1;
    }

  int tcpset_cap = 16;
  set<TCPsocket> tcpset;
  SDLNet_SocketSet tcpset_sdl = NULL;
  tcpset_sdl = SDLNet_AllocSocketSet(tcpset_cap);

  TCPsocket serversock = NULL;
  IPaddress serverip;
  SDLNet_ResolveHost(&serverip, NULL, port);	// Listener socket
  serversock=SDLNet_TCP_Open(&serverip);
  if(!serversock) {
    printf("ERROR: SDLNet_TCP_Open Failed: %s\n", SDLNet_GetError());
    return 1;
    }

  IPaddress broadcast_address = {0};
  SDLNet_ResolveHost(&broadcast_address, "255.255.255.255", port);

  outdata->act = SC_ACT_HOSTING;

  while(!exiting) {
    while(SDLNet_UDP_Recv(udpsock, inpacket) > 0) {
      if(!strcmp((char*)indata->tag, nettag.c_str())) {
	if(indata->act == SC_ACT_QUERYING) {
	  outpacket->address = inpacket->address;
	  if(SDLNet_UDP_Send(udpsock, -1, outpacket) < 1) {
	    fprintf(stderr, "ERROR: SDLNet_UDP_Send Failed: %s\n", SDLNet_GetError());
	    exiting = true;
	    return 1;
	    }
	  }
	}
      }

    TCPsocket tmpsock = SDLNet_TCP_Accept(serversock);
    while(tmpsock) {
      tcpset.insert(tmpsock);
      if((int)(tcpset.size()) <= tcpset_cap) {
	SDLNet_TCP_AddSocket(tcpset_sdl, tmpsock);
	}
      else {	// Enlarge SDL Socket Set
	SDLNet_FreeSocketSet(tcpset_sdl);
	tcpset_cap *= 2;
	tcpset_sdl = SDLNet_AllocSocketSet(tcpset_cap);
	set<TCPsocket>::iterator sock = tcpset.begin();
	for(; sock != tcpset.end(); ++sock) {
	  SDLNet_TCP_AddSocket(tcpset_sdl, (*sock));
	  }
	}
      tmpsock = SDLNet_TCP_Accept(serversock);	// Get the next one (if present)
      }

    if(tcpset.size() > 0) {
      set<TCPsocket> toremove;
      SDLNet_CheckSockets(tcpset_sdl, 10);
      set<TCPsocket>::iterator sock;

      for(sock = tcpset.begin(); sock != tcpset.end(); ++sock) {
	if(SDLNet_SocketReady(*sock)) {
	  toremove.insert(*sock);
	  fprintf(stderr, "DEBUG: Got data from socket (and closed it for now)\n");
	  }
	}

      for(sock = toremove.begin(); sock != toremove.end(); ++sock) {
	tcpset.erase(*sock);
	SDLNet_TCP_DelSocket(tcpset_sdl, *sock);
	SDLNet_TCP_Close(*sock);
	}
      }
    else {
      SDL_Delay(10);
      }
    }

  set<TCPsocket>::iterator sock = tcpset.begin();
  for(; sock != tcpset.end(); ++sock) {
    SDLNet_TCP_Close(*sock);
    }
  tcpset.clear();

  SDLNet_FreeSocketSet(tcpset_sdl);
  SDLNet_TCP_Close(serversock);

  SDLNet_UDP_Close(udpsock);

  SDLNet_FreePacket(inpacket);
  SDLNet_FreePacket(outpacket);
  return 0;
  }

int SimpleConnect::HandleSlaveThread() {
  UDPpacket *inpacket =
	SDLNet_AllocPacket(sizeof(DataPacket) + nettag.length());
  DataPacket *indata = (DataPacket*)(inpacket->data);

  UDPpacket *outpacket =
	SDLNet_AllocPacket(sizeof(DataPacket) + nettag.length());
  DataPacket *outdata = (DataPacket*)(outpacket->data);
  outpacket->len = sizeof(DataPacket) + nettag.length();
  strcpy((char*)outdata->tag, nettag.c_str());

  UDPsocket udpsock = NULL;
  if(mode == SC_MODE_SEARCH) udpsock = SDLNet_UDP_Open(0);
  else if(mode == SC_MODE_HOST) udpsock = SDLNet_UDP_Open(port);
  else {
    fprintf(stderr, "ERROR: Invalid mode in SimpleConnect network handler!\n");
    }
  if(!udpsock) {
    fprintf(stderr, "ERROR: SDLNet_UDP_Open Failed: %s\n", SDLNet_GetError());
    exiting = true;
    return 1;
    }

  int tcpset_cap = 16;
  set<TCPsocket> tcpset;
  SDLNet_SocketSet tcpset_sdl = NULL;

  TCPsocket serversock = NULL;
  if(mode == SC_MODE_HOST) {
    IPaddress serverip;
    SDLNet_ResolveHost(&serverip, NULL, port);	// Listener socket
    serversock=SDLNet_TCP_Open(&serverip);
    if(!serversock) {
      printf("ERROR: SDLNet_TCP_Open Failed: %s\n", SDLNet_GetError());
      return 1;
      }
    tcpset_sdl = SDLNet_AllocSocketSet(tcpset_cap);
    }

  IPaddress broadcast_address = {0};
  SDLNet_ResolveHost(&broadcast_address, "255.255.255.255", port);

  //Temporary!
  if(mode == SC_MODE_SEARCH) {
    outpacket->address = broadcast_address;
    outdata->act = SC_ACT_QUERYING;
    if(SDLNet_UDP_Send(udpsock, -1, outpacket) < 1) {
      fprintf(stderr, "ERROR: SDLNet_UDP_Send Failed: %s\n", SDLNet_GetError());
      exiting = true;
      return 1;
      }
    }
  else if(mode == SC_MODE_HOST) {
    outdata->act = SC_ACT_HOSTING;
    }

  while(!exiting) {
    while(SDLNet_UDP_Recv(udpsock, inpacket) > 0) {
      if(!strcmp((char*)indata->tag, nettag.c_str())) {
	if(mode == SC_MODE_HOST && indata->act == SC_ACT_QUERYING) {
	  outpacket->address = inpacket->address;
	  if(SDLNet_UDP_Send(udpsock, -1, outpacket) < 1) {
	    fprintf(stderr, "ERROR: SDLNet_UDP_Send Failed: %s\n", SDLNet_GetError());
	    exiting = true;
	    return 1;
	    }
	  }
	else if(mode == SC_MODE_SEARCH && indata->act == SC_ACT_HOSTING) {
	  SDL_mutexP(net_mutex);
	  Uint64 entry = ((Uint64)(inpacket->address.host)) << 16
		| ((Uint64)(inpacket->address.port));
	  hosts[entry].address = inpacket->address;
	  hosts[entry].map = "Unknown";
	  hosts[entry].changed = true;
	  SDL_mutexV(net_mutex);
	  }
	}
      }
    if(mode == SC_MODE_HOST && serversock) {
      TCPsocket tmpsock = SDLNet_TCP_Accept(serversock);
      while(tmpsock) {
	tcpset.insert(tmpsock);
	if((int)(tcpset.size()) <= tcpset_cap) {
	  SDLNet_TCP_AddSocket(tcpset_sdl, tmpsock);
	  }
	else {	// Enlarge SDL Socket Set
	  SDLNet_FreeSocketSet(tcpset_sdl);
	  tcpset_cap *= 2;
	  tcpset_sdl = SDLNet_AllocSocketSet(tcpset_cap);
	  set<TCPsocket>::iterator sock = tcpset.begin();
	  for(; sock != tcpset.end(); ++sock) {
	    SDLNet_TCP_AddSocket(tcpset_sdl, (*sock));
	    }
	  }
	tmpsock = SDLNet_TCP_Accept(serversock);
	}
      }
    if(tcpset.size() > 0) {
      set<TCPsocket> toremove;
      SDLNet_CheckSockets(tcpset_sdl, 10);
      set<TCPsocket>::iterator sock;

      for(sock = tcpset.begin(); sock != tcpset.end(); ++sock) {
	if(SDLNet_SocketReady(*sock)) {
	  toremove.insert(*sock);
	  fprintf(stderr, "DEBUG: Got data from socket (and closed it for now)\n");
	  }
	}

      for(sock = toremove.begin(); sock != toremove.end(); ++sock) {
	tcpset.erase(*sock);
	SDLNet_TCP_DelSocket(tcpset_sdl, *sock);
	SDLNet_TCP_Close(*sock);
	}
      }
    else {
      SDL_Delay(10);
      }
    }

  set<TCPsocket>::iterator sock = tcpset.begin();
  for(; sock != tcpset.end(); ++sock) {
    SDLNet_TCP_Close(*sock);
    }
  tcpset.clear();

  if(tcpset_sdl) SDLNet_FreeSocketSet(tcpset_sdl);
  if(serversock) SDLNet_TCP_Close(serversock);

  SDLNet_UDP_Close(udpsock);

  SDLNet_FreePacket(inpacket);
  SDLNet_FreePacket(outpacket);
  return 0;
  }

int SimpleConnect::search_thread_handler(void *me) {
  return ((SimpleConnect*)(me))->HandleSearchThread();
  }

int SimpleConnect::host_thread_handler(void *me) {
  return ((SimpleConnect*)(me))->HandleHostThread();
  }

int SimpleConnect::slave_thread_handler(void *me) {
  return ((SimpleConnect*)(me))->HandleSlaveThread();
  }

void SimpleConnect::CleanupNet() {
  exiting = true;
  if(net_thread) SDL_WaitThread(net_thread, NULL);
  net_thread = NULL;
  if(net_mutex) SDL_DestroyMutex(net_mutex);
  net_mutex = NULL;
  hosts.clear();
  }

void SimpleConnect::StartNet() {
  exiting = false;
  net_mutex = SDL_CreateMutex();
  if(mode == SC_MODE_SEARCH)
    net_thread = SDL_CreateThread(search_thread_handler, (void*)(this));
  else if(mode == SC_MODE_HOST)
    net_thread = SDL_CreateThread(host_thread_handler, (void*)(this));
  else if(mode == SC_MODE_SLAVE)
    net_thread = SDL_CreateThread(slave_thread_handler, (void*)(this));
  }
