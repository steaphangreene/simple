// *************************************************************************
//  This file is part of the SimpleConnect Example Module by Steaphan Greene
//
//  Copyright 2005-2008 Steaphan Greene <stea@cs.binghamton.edu>
//
//  SimpleConnect is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 3 of the License, or
//  (at your option) any later version.
//
//  SimpleConnect is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SimpleConnect (see the file named "COPYING");
//  If not, see <http://www.gnu.org/licenses/>.
//  
// *************************************************************************

#ifndef SC_H
#define SC_H

#define SG_EVENT_CONNECTDONE	4000

#ifndef DEFAULT_PORT
#define DEFAULT_PORT 8942
#endif

#include <queue>
#include <string>
#include <vector>
#include <list>
#include <map>
using namespace std;

#include "SDL_thread.h"
#include "SDL_net.h"

#include "safecomm.h"

#include "sg_compound.h"

class SG_Button;
class SG_TextArea;
class SG_Editable;

enum SC_SlotType {
  SC_SLOT_NONE = 0,
  SC_SLOT_PLAYER,
  SC_SLOT_LOCALONLY,
  SC_SLOT_HUMANONLY,
  SC_SLOT_AIONLY,
  SC_SLOT_OPTPLAYER,
  SC_SLOT_OPTLOCALONLY,
  SC_SLOT_OPTHUMANONLY,
  SC_SLOT_OPTAIONLY,
  SC_SLOT_SPEC,
  SC_SLOT_MAX
  };

enum SC_PlayerType {
  SC_PLAYER_NONE = 0,
  SC_PLAYER_LOCAL,
  SC_PLAYER_REMOTE,
  SC_PLAYER_AI,
  SC_PLAYER_REMOTEAI,
  SC_PLAYER_SPEC,
  SC_PLAYER_MAX
  };

enum SC_Mode {
  SC_MODE_NONE = 0,
  SC_MODE_CONFIG,
  SC_MODE_SEARCH,
  SC_MODE_HOST,
  SC_MODE_SLAVE,
  SC_MODE_MAX
  };

#define SC_FLAG_COLOR
#define SC_FLAG_TEAM

struct SC_Host {
  SC_Host() { line = -1; };
  IPaddress address;
  string mapname;
  int line;
  bool changed;
  };

struct SlotData {
  Uint8 type;
  Uint8 ptype;
  Uint8 team;
  Uint8 color;
  Sint8 playername[16];
  Sint8 password[16];
  TCPsocket sock;
  };

struct SimpleConnections {
  SDLNet_SocketSet tcpset;
  vector<SlotData> slots;
  TCPsocket sock;
  };

class SimpleConnect : public SG_Compound {
public:
  class Connection {
    public:
	struct Conn
	{
		SlotData* data;
		unsigned int last_active;
	};

	struct Conn_Setup
	{
		TCPsocket tcp;	
		vector<Uint8> recv_buffer; // contains buffered data recieved over TCP
		vector<Uint8> send_buffer; // contains buffered sending data.
		SimpleConnections sc;
	};

	Connection() {};
	Connection(TCPsocket sock); //connection for client
	Connection(TCPsocket sock, SimpleConnections sconn); //connection for server

	~Connection(); //cleanup

	// add to send_buffer
	void Add(const Uint8&);
        void Add(const Uint16&);
	void Add(const Uint32&);
	void Add(const string&);

	// sends send_buffer over tcp and clears it.
	void Send();

	// these recieve data from the recv_buffer and take it off the queue.
	void Recv(Uint8&);
	void Recv(Uint16&);
	void Recv(Uint32&);
        void Recv(string&);

	// returns 1 if connection is active, 0 otherwise.
	int is_connected();

	void ClearBuffer();
    private:
	TCPsocket socket;
	SimpleConnections sc;
	static list<string> tokenize(const char*);
	vector<Uint8> recv_buffer; // contains buffered data recieved over TCP
	vector<Uint8> send_buffer; // contains buffered sending data.
	static int RunClient(void*); // client thread.
	static int RunServer(void*); // server thread.
	SDL_Thread* networking_thread;
  };

  SimpleConnect();
  virtual ~SimpleConnect();

  //  virtual bool SetDefaultCursor(GL_MODEL *cur);
  virtual bool ChildEvent(SDL_Event *event);

  void SetPort(Uint16 p) { port = p; };
  void SetTag(const string &tag);
  void SetColors(const vector<int> &cols);	//SG_Color List Used

  void SetSlots(const vector<SC_SlotType> &slts);
  void SetSlotColors(const vector<int> &cols);	//Color for each slot
  void SetSlotTeams(const vector<int> &cols);	//Team for each slot

  void Host();					//Host new game
  void Search();				//Find network games
  void Config();				//Local game, no networking
  void Connect(const IPaddress &location);	//Connect to active game
  void Reset();

  void SetPlayerName(const string &pln);
  const string &PlayerName();

  void SetMapName(const string &mp) { mapname = mp; };
  const string &MapName() { return mapname; };

  const SimpleConnections &ClaimConnections();
  
protected:
  virtual bool RenderSelf(unsigned long cur_time);

  void SetupDefaultColors();
  int NextFreeColor(int oldcolor);
  int NextTeam(int oldteam);
  vector<int> colors;

//  static GL_MODEL Default_Mouse_Cursor;
  SG_Editable *nameb;
  SG_TextArea *namep;
  SG_Button *scanb;
  SG_Button *startb;

  SimpleConnections conn;
  bool slots_dirty;
  bool slots_send;

  string mapname;
  string nettag;
  SC_Mode mode;
  unsigned int prop_flags, change_flags;

  void InitSlots();

  Uint16 port;
  IPaddress connect_to;
  bool rescan;
  map<SG_Widget*, IPaddress> joinmap;
  map<SG_Widget*, IPaddress> specmap;

  map<SG_Widget*, int> colmap;
  map<SG_Widget*, int> teammap;
  map<SG_Ranger2D*, int> pnamemap;

  static int search_thread_handler(void *me);
  int HandleSearchThread();
  static int host_thread_handler(void *me);
  int HandleHostThread();
  static int slave_thread_handler(void *me);
  int HandleSlaveThread();

  void StartNet();
  void CleanupNet();
  SDL_Thread *net_thread;

  SDL_mutex *net_mutex;
  bool exiting;
  bool starting;
  bool netclaimed;
  bool needtoscan;
  map<Uint64, SC_Host> hosts;

  struct Request { int size; Uint8 data[20]; };
  list<Request> reqs;	

  };

#endif // SC_H
