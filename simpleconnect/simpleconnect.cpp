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

#define HEADER_SIZE 1

enum SCAct {
  SC_ACT_QUERYING = 0,
  SC_ACT_HOSTING,
  SC_ACT_LEAVING,
  SC_ACT_MAX
  };

struct DataPacket {
  Uint8 act;
  Sint8 mapname[32];
  Sint8 tag[1];
  };

SimpleConnect::SimpleConnect() : SG_Compound(8, HEADER_SIZE, 0.1, 0.1) {
  mode = SC_MODE_NONE;
  prop_flags = 0;
  change_flags = 0;

  background = new SG_Panel(SG_COL_FG);
  SG_Widget *labelb =
	new SG_TextArea("SimpleConnect", SG_COL_LOW);
  AddWidget(labelb, 0, 0, 6, 1);
  scanb = new SG_Button("Rescan", SG_COL_RAISED, SG_COL_LOW);
  AddWidget(scanb, 7, 0, 1, 1);

  port = DEFAULT_PORT;

  net_thread = NULL;
  net_mutex = NULL;
  exiting = false;
  netclaimed = false;
  }

SimpleConnect::~SimpleConnect() {
  CleanupNet();
  }

bool SimpleConnect::Render(unsigned long cur_time) {
  if(mode == SC_MODE_SEARCH) {
    SDL_mutexP(net_mutex);
    map<Uint64, SC_Host>::iterator host = hosts.begin();
    for(; host != hosts.end(); ++host) {
      if(host->second.changed) {
	host->second.changed = false;
	if(host->second.line < 0) {
	  host->second.line = (ysize - HEADER_SIZE);
	  Resize(xsize, ysize+HEADER_SIZE);
	  }
	else {			//Clear the old widgets
	  ClearRow(HEADER_SIZE + host->second.line);
	  }

	AddWidget(new SG_TextArea(SDLNet_ResolveIP(&(host->second.address))),
		0, HEADER_SIZE + host->second.line, 2, 1);
	AddWidget(new SG_TextArea(host->second.mapname),
		2, HEADER_SIZE + host->second.line, 4, 1);

	SG_Widget *but;
	but = new SG_Button("Join");
	AddWidget(but, 6, HEADER_SIZE + host->second.line);
	joinmap[but] = host->second.address;

	but = new SG_Button("Spec");
	AddWidget(but, 7, HEADER_SIZE + host->second.line);
	specmap[but] = host->second.address;
	}
      }
    SDL_mutexV(net_mutex);
    }

  else if(mode == SC_MODE_HOST || mode == SC_MODE_SLAVE) {
    SDL_mutexP(net_mutex);
    if(slots_dirty) InitSlots();
    slots_dirty = false;
    SDL_mutexV(net_mutex);
    }

  else if(mode == SC_MODE_CONFIG) {
    if(slots_dirty) InitSlots();
    slots_dirty = false;
    }

  return SG_Compound::Render(cur_time);
  }

void SimpleConnect::SetTag(const string &tag) {
  nettag = tag;
  }

void SimpleConnect::SetSlots(const vector<SC_SlotType> &slts) {
  SlotData data = { SC_SLOT_NONE, 0 };
  conn.slots.clear();

  bool placed_local = false;
  vector<SC_SlotType>::const_iterator slot = slts.begin();
  for(; slot != slts.end(); ++slot) {
    data.type = (*slot);
    if((!placed_local) && (data.type < SC_SLOT_AIONLY)) {
      data.ptype = SC_PLAYER_LOCAL;
      placed_local = true;
      }
    else if(data.type < SC_SLOT_OPTPLAYER) {
      data.ptype = SC_PLAYER_AI;
      }
    else {
      data.ptype = SC_PLAYER_NONE;
      }
    sprintf((char*)(data.playername), "Player %d%c", slot-slts.begin()+1, 0);
    conn.slots.push_back(data);
    }
  }

void SimpleConnect::InitSlots() {
  Resize(8, HEADER_SIZE);		//Clear any list widgets
  Resize(8, HEADER_SIZE + conn.slots.size());

  for(unsigned int slot = 0; slot < conn.slots.size(); ++slot) {
    SG_StickyButton *meb = new SG_StickyButton("Me");
    SG_TextArea *name = NULL;
    if(conn.slots[slot].ptype == SC_PLAYER_LOCAL) {
      meb->TurnOn();
      name = new SG_TextArea("Local Player");
      }
    else if(conn.slots[slot].ptype == SC_PLAYER_REMOTE) {
      name = new SG_TextArea("Remote Player");
      }
    else if(conn.slots[slot].ptype == SC_PLAYER_AI) {
      name = new SG_TextArea("AI Player");
      }
    else if(conn.slots[slot].ptype == SC_PLAYER_NONE) {
      name = new SG_TextArea("<Open>");
      }
    else {
      name = new SG_TextArea("<Unknown>");
      }
    AddWidget(name, 4, slot+HEADER_SIZE, 4, 1);
    AddWidget(meb, 0, slot+HEADER_SIZE, 1, 1);

    AddWidget(new SG_TextArea((char*)(conn.slots[slot].playername)),
	1, slot+HEADER_SIZE, 3, 1);
    }
  }

void SimpleConnect::Host() {
  CleanupNet();
  if(nettag.length() == 0) {
    fprintf(stderr, "ERROR: Tag not set before SimpleConnect::Host()\n");
    exit(1);
    }
  if(conn.slots.size() == 0) {
    fprintf(stderr, "ERROR: Slots not set before SimpleConnect::Host()\n");
    exit(1);
    }
  mode = SC_MODE_HOST;
  slots_dirty = true;
  slots_send = true;
  StartNet();
  }

void SimpleConnect::Search() {
  CleanupNet();
  if(nettag.length() == 0) {
    fprintf(stderr, "ERROR: Tag not set before SimpleConnect::Search()\n");
    exit(1);
    }
  mode = SC_MODE_SEARCH;
  Resize(8, HEADER_SIZE);		//Clear any list widgets
  StartNet();
  }

void SimpleConnect::Connect(const IPaddress &location) {
  CleanupNet();
  if(nettag.length() == 0) {
    fprintf(stderr, "ERROR: Tag not set before SimpleConnect::Connect()\n");
    exit(1);
    }
  connect_to = location;
  mode = SC_MODE_SLAVE;
  Resize(8, HEADER_SIZE);		//Clear any list widgets
  ClearRow(0);		//Clear header if present
  AddWidget(new SG_TextArea("Connecting..."), 1, 0, 6, 1);
  StartNet();
  }

void SimpleConnect::Config() {
  CleanupNet();
  if(conn.slots.size() == 0) {
    fprintf(stderr, "ERROR: Slots not set before SimpleConnect::Config()\n");
    exit(1);
    }
  mode = SC_MODE_CONFIG;
  slots_dirty = true;
  }

void SimpleConnect::Reset() {
  CleanupNet();
  conn.slots.clear();
  mode = SC_MODE_NONE;
  Resize(8, HEADER_SIZE);		//Clear any list widgets
  }

const SimpleConnections &SimpleConnect::ClaimConnections() {
  static SimpleConnections ret;
  if(mode != SC_MODE_NONE) {	//Only reset once - can be called multiple times
    ret = conn;
    netclaimed = true;
    Reset();
    netclaimed = false;
    }
  return ret;
  }

bool SimpleConnect::ChildEvent(SDL_Event *event) {
  if(event->user.code == SG_EVENT_BUTTONPRESS) {
    if(joinmap.count((SG_Widget*)(event->user.data1)) > 0) {
      Connect(joinmap[(SG_Widget*)(event->user.data1)]);
      }
    else if(event->user.data1 == (void *)(SG_Widget*)(scanb)) {
      event->user.code = SG_EVENT_NEEDTORENDER;
      event->user.data1 = NULL;
      event->user.data2 = NULL;
      rescan = true;
      return 1;
      }
    }
  return 0; // Silence children doing other things
  }

//  bool SimpleConnect::SetDefaultCursor(GL_MODEL *cur);
  
//  static GL_MODEL SimpleConnect::Default_Mouse_Cursor = NULL;

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

  rescan = true;

  while(!exiting) {
    if(rescan) {
      rescan = false;
      if(SDLNet_UDP_Send(udpsock, -1, outpacket) < 1) {
	fprintf(stderr, "ERROR: SDLNet_UDP_Send Failed: %s\n", SDLNet_GetError());
	exiting = true;
	return 1;
	}
      }
    while(SDLNet_UDP_Recv(udpsock, inpacket) > 0) {
      if(!strcmp((char*)indata->tag, nettag.c_str())) {
	if(indata->act == SC_ACT_HOSTING) {
	  SDL_mutexP(net_mutex);
	  Uint64 entry = ((Uint64)(inpacket->address.host)) << 16
		| ((Uint64)(inpacket->address.port));
	  hosts[entry].address = inpacket->address;
	  hosts[entry].mapname = (char*)indata->mapname;
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
  conn.tcpset = SDLNet_AllocSocketSet(tcpset_cap);

  TCPsocket serversock = NULL;
  IPaddress serverip;
  SDLNet_ResolveHost(&serverip, NULL, port);	// Listener socket
  serversock=SDLNet_TCP_Open(&serverip);
  if(!serversock) {
    printf("ERROR: SDLNet_TCP_Open Failed: %s\n", SDLNet_GetError());
    exiting = true;
    return 1;
    }

  IPaddress broadcast_address = {0};
  SDLNet_ResolveHost(&broadcast_address, "255.255.255.255", port);

  while(!exiting) {
    while(SDLNet_UDP_Recv(udpsock, inpacket) > 0) {
      if(!strcmp((char*)indata->tag, nettag.c_str())) {
	if(indata->act == SC_ACT_QUERYING) {
	  outpacket->address = inpacket->address;
	  outdata->act = SC_ACT_HOSTING;
	  strncpy((char*)outdata->mapname, mapname.c_str(), 31);
	  outdata->mapname[31] = 0;
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
      SDL_mutexP(net_mutex);

      slots_send = true;

      tcpset.insert(tmpsock);
      if((int)(tcpset.size()) <= tcpset_cap) {
	SDLNet_TCP_AddSocket(conn.tcpset, tmpsock);
	}
      else {	// Enlarge SDL Socket Set
	SDLNet_FreeSocketSet(conn.tcpset);
	tcpset_cap *= 2;
	conn.tcpset = SDLNet_AllocSocketSet(tcpset_cap);
	set<TCPsocket>::iterator sock = tcpset.begin();
	for(; sock != tcpset.end(); ++sock) {
	  SDLNet_TCP_AddSocket(conn.tcpset, (*sock));
	  }
	}

      fprintf(stderr, "Finding a slot!\n");
      vector<SlotData>::iterator slot = conn.slots.begin();
      for(; slot != conn.slots.end(); ++slot) {
	if((slot->ptype == SC_PLAYER_NONE || slot->ptype == SC_PLAYER_AI) && (
		slot->type == SC_SLOT_PLAYER ||
		slot->type == SC_SLOT_HUMANONLY ||
		slot->type == SC_SLOT_OPTPLAYER ||
		slot->type == SC_SLOT_OPTHUMANONLY)
		) {
	  slot->ptype = SC_PLAYER_REMOTE;
	  slot->sock = tmpsock;
	  slots_dirty = true;
	  fprintf(stderr, "Found slot!\n");
	  break;
	  }
	}

      SDL_mutexV(net_mutex);

      tmpsock = SDLNet_TCP_Accept(serversock);	// Get the next one (if present)
      }

    set<TCPsocket> toremove;
    set<TCPsocket>::iterator sock;

    SDL_mutexP(net_mutex);
    if(slots_send) {
      Uint8 num = conn.slots.size();
      for(sock = tcpset.begin(); sock != tcpset.end(); ++sock) {
	SDLNet_TCP_Send(*sock, &num, 1);
	}
      Uint8 *data = new Uint8[conn.slots.size() * 20];
      for(unsigned int slot = 0; slot < conn.slots.size(); ++slot) {
	data[slot*20 + 0] = conn.slots[slot].type;
	if(conn.slots[slot].ptype == SC_PLAYER_LOCAL)
	  data[slot*20 + 1] = SC_PLAYER_REMOTE;
	else 
	  data[slot*20 + 1] = conn.slots[slot].ptype;
	data[slot*20 + 2] = conn.slots[slot].team;
	data[slot*20 + 3] = conn.slots[slot].color;
	strncpy((char*)(data + slot*20 + 4),
		(char*)(conn.slots[slot].playername), 15);
	data[slot*20 + 19] = 0;
	}
      for(sock = tcpset.begin(); sock != tcpset.end(); ++sock) {
	for(unsigned int slot = 0; slot < conn.slots.size(); ++slot)
	  if(conn.slots[slot].sock == *sock) data[slot*20 + 1] = SC_PLAYER_LOCAL;
	int res = SDLNet_TCP_Send(*sock, data, conn.slots.size() * 20);
	if(res != (int)(conn.slots.size() * 20)) {
	  SDLNet_TCP_Close(*sock);
	  toremove.insert(*sock);
	  fprintf(stderr, "WARNING: A socket that was connected failed!\n");
	  continue;
	  }
	for(unsigned int slot = 0; slot < conn.slots.size(); ++slot)
	  if(conn.slots[slot].sock == *sock) data[slot*20 + 1] = SC_PLAYER_REMOTE;
	}
      delete [] data;
      slots_send = false;
      }
    SDL_mutexV(net_mutex);

    for(sock = toremove.begin(); sock != toremove.end(); ++sock) {
      tcpset.erase(*sock);
      SDLNet_TCP_DelSocket(conn.tcpset, *sock);
      SDLNet_TCP_Close(*sock);
      }

    if(tcpset.size() > 0) {
      SDLNet_CheckSockets(conn.tcpset, 10);

      for(sock = tcpset.begin(); sock != tcpset.end(); ++sock) {
	if(SDLNet_SocketReady(*sock)) {
	  toremove.insert(*sock);
	  fprintf(stderr, "DEBUG: Got data from socket (and closed it for now)\n");
	  }
	}

      for(sock = toremove.begin(); sock != toremove.end(); ++sock) {
	tcpset.erase(*sock);
	SDLNet_TCP_DelSocket(conn.tcpset, *sock);
	SDLNet_TCP_Close(*sock);
	}
      }
    else {
      SDL_Delay(10);
      }
    }

  if(!netclaimed) {	// Aborting, not connecting
    set<TCPsocket>::iterator sock = tcpset.begin();
    for(; sock != tcpset.end(); ++sock) {
      SDLNet_TCP_Close(*sock);
      }
    tcpset.clear();

    SDLNet_FreeSocketSet(conn.tcpset);
    conn.tcpset = NULL;
    }

  SDLNet_TCP_Close(serversock);

  SDLNet_UDP_Close(udpsock);

  SDLNet_FreePacket(inpacket);
  SDLNet_FreePacket(outpacket);
  return 0;
  }

int SimpleConnect::HandleSlaveThread() {
  { TCPsocket sock = NULL;

    sock=SDLNet_TCP_Open(&connect_to);
    if(!sock) {
      printf("ERROR: SDLNet_TCP_Open Failed: %s\n", SDLNet_GetError());
      exiting = true;
      return 1;
      }
    conn.sock = sock;
    }

  conn.tcpset = SDLNet_AllocSocketSet(1);
  SDLNet_TCP_AddSocket(conn.tcpset, conn.sock);

  while(!exiting) {
    SDLNet_CheckSockets(conn.tcpset, 10);
    if(SDLNet_SocketReady(conn.sock)) {
      Uint8 num_slots;
      SDLNet_TCP_Recv(conn.sock, &num_slots, 1);
      SDL_mutexP(net_mutex);
      conn.slots.clear();
      conn.slots.resize(num_slots);
      for(int sl = 0; sl < num_slots; ++sl) {
	int comp = 0;
	comp += SDLNet_TCP_Recv(conn.sock, &(conn.slots[sl].type), 1);
	comp += SDLNet_TCP_Recv(conn.sock, &(conn.slots[sl].ptype), 1);
	comp += SDLNet_TCP_Recv(conn.sock, &(conn.slots[sl].team), 1);
	comp += SDLNet_TCP_Recv(conn.sock, &(conn.slots[sl].color), 1);
	comp += SDLNet_TCP_Recv(conn.sock, conn.slots[sl].playername, 16);
	if(comp != 20) {
	  printf("ERROR: SDLNet_TCP_Recv Failed: %s\n", SDLNet_GetError());
	  exiting = true;
	  SDL_mutexV(net_mutex);
	  return 1;
	  }
	}
      slots_dirty = true;
      SDL_mutexV(net_mutex);
      }
    }

  if(!netclaimed) {	// Aborting, not connecting
    SDLNet_FreeSocketSet(conn.tcpset);
    conn.tcpset = NULL;

    SDLNet_TCP_Close(conn.sock);
    }
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
  rescan = false;
  slots_dirty = false;
  slots_send = false;
  if(net_thread) SDL_WaitThread(net_thread, NULL);
  net_thread = NULL;
  if(net_mutex) SDL_DestroyMutex(net_mutex);
  net_mutex = NULL;
  hosts.clear();
  joinmap.clear();
  specmap.clear();
  }

void SimpleConnect::StartNet() {
  exiting = false;
  rescan = false;
  net_mutex = SDL_CreateMutex();
  if(mode == SC_MODE_SEARCH)
    net_thread = SDL_CreateThread(search_thread_handler, (void*)(this));
  else if(mode == SC_MODE_HOST)
    net_thread = SDL_CreateThread(host_thread_handler, (void*)(this));
  else if(mode == SC_MODE_SLAVE)
    net_thread = SDL_CreateThread(slave_thread_handler, (void*)(this));
  }
