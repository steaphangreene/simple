// *************************************************************************
//  This file is part of the SimpleUtils Example Module by Steaphan Greene
//
//  Copyright 2005-2015 Steaphan Greene <steaphan@gmail.com>
//
//  SimpleUtils is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 3 of the License, or
//  (at your option) any later version.
//
//  SimpleUtils is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SimpleUtils (see the file named "COPYING");
//  If not, see <http://www.gnu.org/licenses/>.
// *************************************************************************

#ifndef SIMPLENETWORK_H
#define SIMPLENETWORK_H

#include <string>
#include <vector>
#include <list>
#include <map>
#include "safecomm.h"

#include "SDL_thread.h"
#include "SDL_net.h"

using namespace std;

enum SN_Status { SN_CONN_NONE = 0, SN_CONN_OK = 1, SN_CONN_RECON = 2 };

class SimpleNetwork {
 public:
  static SimpleNetwork *Current() { return current; };

  struct Data {
    string playername;
    string password;
    TCPsocket tcp;
    SDL_mutex *recv_mutex;
    vector<Uint8> recv_buffer;  // contains buffered data recieved over TCP
    vector<Uint8> send_buffer;  // contains buffered sending data.
    Uint32 last_active;
    Uint8 conn_status;
  };

  SimpleNetwork(Uint16 port = 4052);
  ~SimpleNetwork();  // cleanup

  // add to send_buffer
  void Add(int, const string &);
  template <class T>
  void Add(int slot, const T &ref) {
    Uint8 buf[sizeof(ref)];
    WriteNBO(ref, buf);
    for (unsigned int i = 0; i < sizeof(ref); ++i) {
      data[slot].send_buffer.push_back(buf[i]);
      fprintf(stderr, "Data Send: %d\n", buf[i]);
    }
  }

  // sends send_buffer over tcp and clears it.
  void Send(int);  // should this return int for some reason?

  // returns conection id, adds new connection
  // also for reconnection, with name and password.
  int Connect(IPaddress &, const string &name = "",
              const string &password = "");

  void StopAccepting();

  // internals: start this up when a connection drops, takes max amount to
  // accept.
  // when we recieve this amount, stop accepting
  void StartAccepting(int);

  // returns number of connections to the server.
  int NumConnections();

  void LockConnections();
  void UnlockConnections();

  // disconnects an individual client from the server given its slot.
  void Disconnect(int);

  // these recieve data from the recv_buffer and take it off the queue.
  void Recv(int, string &);
  template <class T>
  void Recv(int slot, T &ref) {
    vector<Uint8> *recv_buffer = &data[slot].recv_buffer;
    T tmp = T();
    unsigned int cur;

    SDL_mutexP(data[slot].recv_mutex);
    vector<Uint8>::iterator i;
    vector<Uint8>::iterator start = recv_buffer->begin();
    for (i = start, cur = 0; i != recv_buffer->end() && cur < sizeof(T);
         ++i, ++cur) {
      ((Uint8 *)(&tmp))[cur] = *i;
      fprintf(stderr, "Data Recv: %d\n", ((Uint8 *)(&tmp))[cur]);
    }
    ReadNBO(ref, ((Uint8 *)(&tmp)));
    recv_buffer->erase(start, i);
    SDL_mutexV(data[slot].recv_mutex);
  }

  // gets the size of the recv_buffer.
  int RecvBufferSize(int);

  // sets port to given value
  void SetPort(Uint16);

  // set the playername of the given id.
  void SetName(int, const string &);

  // set the password of the given id.
  void SetPassword(int, const string &);

  // gets the playername of the given id.
  string GetName(int);

  // returns the connection status of the player in given slot.
  SN_Status IsConnected(int);

 protected:
  Uint8 curr_slot;
  Uint16 port;
  map<Uint16, Data> data;
  SDLNet_SocketSet cnx_set;
  int accept_amount;  // number of accepted sockets left for StartAccepting()
  TCPsocket sd;       // server tcp socket.
  bool isserver;
  bool connections_locked;

  static int recv_handler(void *);  // client thread.
  int RunRecv();                    // client thread.

  static int accept_handler(void *);  // runs the accept routine.
  int RunAccept();                    // runs the accept routine.

  static int FindNull(const char *, int, int);

  static SimpleNetwork *current;

  SDL_Thread *accept_thread;
  SDL_Thread *recv_thread;
  SDL_mutex *accept_mutex;
};

#endif  // SIMPLENETWORK_H
