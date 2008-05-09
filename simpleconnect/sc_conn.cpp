#if 0
#!/bin/sh
g++ -Wall `sdl-config --cflags` sc_conn.cpp  `sdl-config --libs` -lSDL_net

exit
#endif
// *************************************************************************
//  This file is part of the SimpleConnect Example Module by Steaphan Greene
//
//  Copyright 2005-2008 Steaphan Greene <stea@cs.binghamton.edu>
//
//  SimpleConnect is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  SimpleConnect is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SimpleConnect (see the file named "COPYING");
//  if not, write to the the Free Software Foundation, Inc.,
//  59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
// *************************************************************************

#include "sc_conn.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <list>
#include <vector>
#include <time.h>
#include <SDL/SDL_thread.h>
#include "SDL_net.h"

#define OP_SIZE 5
#define TIMEOUT 5000
#define MAXSOCKETS 100
#define MAXLEN 1024
#define MSG_SIZE 1024
using namespace std;

// constructer for creating a server connection.
Connection::Connection(Uint16 port)
{
	this->port = port;
	recv_thread = NULL;
	accept_thread = NULL;
	recv_running = false;
	accept_running = false;
	amount_mutex = NULL;
	data_mutex = SDL_CreateMutex();
	curr_slot = 0;
	connections_locked = false;

	accept_amount = 0;
	// allocate socketset.
	if( (cnx_set = SDLNet_AllocSocketSet(MAXSOCKETS)) == NULL) {
		printf("SDLNet_AllocSocketSet: %s\n", SDLNet_GetError());
		exit(1); //most of the time this is a major error, but do what you want.
	}
}

Connection::~Connection()
{
	SDL_DestroyMutex(data_mutex);
}

void Connection::Add(int slot, const Uint8& ref)
{
	Uint8 buf[sizeof(ref)];
	WriteNBO(ref, buf);

	for (unsigned int i = 0 ; i < sizeof(ref); ++i)
	{
		data[slot].send_buffer.push_back(buf[i]);
	}
}

void Connection::Add(int slot,const Uint16& ref)
{
	Uint8 buf[sizeof(ref)];
	WriteNBO(ref,buf);

	for (unsigned int i = 0 ; i < sizeof(ref); ++i)
	{
		data[slot].send_buffer.push_back(buf[i]);
	}
}

void Connection::Add(int slot,const Uint32& ref)
{
	Uint8 buf[sizeof(ref)];
	WriteNBO(ref,buf);

	for (unsigned int i = 0 ; i < sizeof(ref); ++i)
	{
		data[slot].send_buffer.push_back(buf[i]);
	}
}

void Connection::Add(int slot, const string& ref)
{
	for (unsigned int i = 0 ; i <= ref.length(); ++i)
	{
		data[slot].send_buffer.push_back(ref[i]);
	}
}

// orders the sending of Add()ed values.
void Connection::Send(int slot)
{
	vector<Uint8>* send_buffer = &data[slot].send_buffer;
	Uint8 tmp_buffer[send_buffer->size()];

	// copies send_buffer to a Uint8 array of equal size for accurate sending.
	vector<Uint8>::iterator i;
	int count;
	for (count = 0, i = send_buffer->begin(); i != send_buffer->end(); ++i, ++count)
		tmp_buffer[count] = *i;

	SDLNet_TCP_Send(data[slot].tcp, (void*) &tmp_buffer, send_buffer->size());
	send_buffer->clear();
}

void Connection::Recv(int slot, Uint8& ref)
{
	vector<Uint8>* recv_buffer = &data[slot].recv_buffer;
	unsigned int size = 1;
	Uint8 tmp[size];
	unsigned int current;
	
	SDL_mutexP(data[slot].recv_mutex);
	vector<Uint8>::iterator i;
	vector<Uint8>::iterator start = recv_buffer->begin();
	for (i = start, current = 0; i != recv_buffer->end() && current < size; ++i, ++current)
	{
		fprintf(stderr, " %X  ", *i);
		tmp[current] = *i;
	}
	fprintf(stderr, "\n");
	ReadNBO(ref,tmp);
	recv_buffer->erase(start,i);
	SDL_mutexV(data[slot].recv_mutex);
}

void Connection::Recv(int slot, Uint16& ref)
{
	vector<Uint8>* recv_buffer = &data[slot].recv_buffer;
	unsigned int size = 2;
	Uint8 tmp[size];
	unsigned int current;

	SDL_mutexP(data[slot].recv_mutex);
	vector<Uint8>::iterator i;
	vector<Uint8>::iterator start = recv_buffer->begin();
	for (i = start, current = 0; i != recv_buffer->end() && current < size; ++i, ++current)
	{
		fprintf(stderr, " %X  ", *i);
		tmp[current] = *i;
	}
	
	fprintf(stderr, "\n");
	ReadNBO(ref,tmp);
	recv_buffer->erase(start,i);
	SDL_mutexV(data[slot].recv_mutex);
}

void Connection::Recv(int slot, Uint32& ref)
{
	vector<Uint8>* recv_buffer = &data[slot].recv_buffer;
	unsigned int size = 4;
	Uint8 tmp[size];
	unsigned int current;
	
	SDL_mutexP(data[slot].recv_mutex);
	vector<Uint8>::iterator i;
	vector<Uint8>::iterator start = recv_buffer->begin();
	for (i = start, current = 0; i != recv_buffer->end() && current < size; ++i, ++current)
	{
		fprintf(stderr, " %X  ", *i);
		tmp[current] = *i;
	}
	fprintf(stderr, "\n");
	ReadNBO(ref,tmp);
	recv_buffer->erase(start,i);
	SDL_mutexV(data[slot].recv_mutex);
}

void Connection::Recv(int slot, string& ref)
{
	vector<Uint8>* recv_buffer = &data[slot].recv_buffer;
	string s = "";

	vector<Uint8>::iterator i;

	SDL_mutexP(data[slot].recv_mutex);
	for (i = recv_buffer->begin(); i != recv_buffer->end() && *i != 0x00; ++i)
	{
		s.append( 1, (char) *i);
	}
	fprintf(stderr, "\ninside recv:string, after loop, *i=%d\n", *i);
	if (i != recv_buffer->end())
	{
		s.append( 1, (char) *i); 
	}
	ref = s;
	if (i != recv_buffer->end())
		++i;

	recv_buffer->erase(recv_buffer->begin(),i);
	SDL_mutexV(data[slot].recv_mutex);
}


// returns 1 if it is connected, 0 otherwise.
int Connection::IsConnected(int slot)
{
	return data[slot].conn_status;
}

int Connection::RecvBufferSize(int slot)
{
	SDL_mutexP(data[slot].recv_mutex);
	int ret = data[slot].recv_buffer.size();
	SDL_mutexV(data[slot].recv_mutex);
	return ret;
}

int Connection::NumConnections()
{
	return data.size();
}

// Disconnectes from the server by sending it the QUIT operator which will force the removal of the socket.
void Connection::Disconnect(int slot)
{
	if (data[slot].conn_status == CONN_OK)
	{
		data[slot].conn_status = connections_locked ? CONN_RECON : CONN_NONE;
	}

	if (data[slot].tcp)
	{
		fprintf(stderr,"tcp_exists\n");
		SDLNet_TCP_Send(data[slot].tcp, (void*)"QUIT",OP_SIZE);
		if (isserver)
		{
			SDLNet_TCP_Close(data[slot].tcp);
		}
	}
}

void Connection::SetPort(Uint16 port)
{
	this->port = port;
}

void Connection::SetName(int slot, const string& str)
{
	data[slot].playername = str;
}

void Connection::SetPassword(int slot, const string& str)
{
	data[slot].password = str;
}

string Connection::GetName(int slot)
{
	return (data[slot].conn_status == CONN_NONE) ? "" : data[slot].playername;
}

// returns the slot number on success, -1 on failure.
int Connection::Connect(IPaddress& ip, const string& name, const string& password)
{
	data[curr_slot].playername = name;
	data[curr_slot].password = password;

	// open stuff....
	if (!(data[curr_slot].tcp = SDLNet_TCP_Open(&ip)))
	{
		fprintf(stderr, "TCP_OPEN");
		return -1;
	}

	// send the name and password to the server on connection.
	Uint8 bytes[password.length() + name.length() + 2];
	const char* s = name.c_str();
	const char* s2 = password.c_str();
	memcpy(bytes, s, name.length()+1);
	memcpy(bytes+name.length()+1, s2, password.length()+1);
	SDLNet_TCP_Send(data[curr_slot].tcp, (void*) bytes, password.length() + name.length() + 2);

	data[curr_slot].conn_status = CONN_OK;
	data[curr_slot].recv_mutex = SDL_CreateMutex();

	// add this new connection to the connection set.
	// make sure runclient is running for this connection.
	if (!recv_running)
	{
		recv_thread = SDL_CreateThread(RunRecv, (void*) this);
		recv_running = true;
	}
	fprintf(stderr, "cursl: %d\n", curr_slot);
	return curr_slot++;
}

void Connection::UnlockConnections()
{
	connections_locked = false;
}

void Connection::LockConnections()
{
	connections_locked = true;	
}

void Connection::StopAccepting()
{
	if (accept_running)
	{
		accept_amount = 0;
		SDL_KillThread(accept_thread);
		accept_running = false;
	}
}

//if not already accepting, this starts a thread which accepts 'amount number of
//new sockets.
void Connection::StartAccepting(int amount)
{
	IPaddress ip;
	isserver = true;
	// allocate our socket set iff server_ss hasn't already been allocated.

	if (amount_mutex == NULL)
		amount_mutex =	SDL_CreateMutex();

	// Resolving the host using NULL make network interface to listen
	if (SDLNet_ResolveHost(&ip, NULL, port) < 0)
	{
		fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	// Open a connection with the IP provided (listen on the host's port)
	if (!accept_running)
	{
		if (!(sd = SDLNet_TCP_Open(&ip)))
		{
			fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
			exit(EXIT_FAILURE);
		}

		accept_amount = amount;
		accept_thread = SDL_CreateThread(RunAccept, (void*) this);
		accept_running = false;
	}
	else {	
		accept_amount += amount;
	}

	// start up runserver if it isn't already running.
	if (!recv_running)
	{
		recv_thread = SDL_CreateThread(RunRecv, (void*) this);
		recv_running = true;
	}
}

// accepts and adds new sockets to data which have been accepted into
// the listened-to things.
// s represents a pointer to a Connection class which has an open server
// descripter.
int Connection::RunAccept(void* s)
{
	Connection* conn = (Connection*) s;
	TCPsocket csd;
	int cnx_amt;
	int amt = conn->accept_amount;
	char msg[MSG_SIZE];
	int nullch, nullch2;

	while (amt)
	{
		if ( (csd = SDLNet_TCP_Accept(conn->sd)) )
		{
			if( (cnx_amt = SDLNet_TCP_AddSocket(conn->cnx_set, csd)) == -1)
			{
				fprintf(stderr,"SDLNet_AddSocket: %s\n", SDLNet_GetError());
				// perhaps restart the set and make it bigger...
				SDLNet_TCP_Send(csd, (void*)"QUIT", OP_SIZE);
				fprintf(stderr,"%d sockets in set (fail)\n", cnx_amt);
				SDLNet_TCP_Close(csd);
				conn->accept_amount = 0;
			} else {
				fprintf(stderr, "%d sockets in set (Server)\n", cnx_amt);
			
				int result = SDLNet_TCP_Recv(csd, msg, MSG_SIZE);

				// debugging code...
					fprintf(stderr, "ret: %d\n", result);
	
					// for debugging code
					fprintf(stderr, "Recieved Bytes (ACCEPT): ");
					for (int c = 0; c < result; ++c)
						fprintf(stderr,"%X  ", msg[c]);
					fprintf(stderr, "\n");
					fprintf(stderr, "msg: %s\n", msg);
				// ... end debugging code

				nullch = FindNull(msg, 0, result);
				nullch2 = FindNull(msg, nullch+1, result);
				if (nullch == result || msg[nullch2] != 0)
				{
					fprintf(stderr, "Invalid login message.\n");
					SDLNet_TCP_Send(csd, (void*)"QUIT", OP_SIZE);
					SDLNet_TCP_DelSocket(conn->cnx_set, csd);
					SDLNet_TCP_Close(csd);
					continue;
				}

				if (conn->connections_locked == false)
				{
					fprintf(stderr, "adding data\n");
					SDL_mutexP(conn->data_mutex);
					conn->data[conn->curr_slot].recv_mutex = SDL_CreateMutex();
					conn->data[conn->curr_slot].tcp = csd;
					conn->data[conn->curr_slot].last_active = time(NULL);
					conn->data[conn->curr_slot].conn_status = CONN_OK;
					conn->data[conn->curr_slot].password = msg;
					conn->data[conn->curr_slot].playername = msg+nullch+1;

					SDL_mutexP(conn->data[conn->curr_slot].recv_mutex);
					for (int i = nullch2+1; i < result; ++i)
					{
						if (i == nullch2+1) fprintf(stderr, "Adding to recv_buffer: ");
						fprintf(stderr,"%X  ", msg[i]);
						conn->data[conn->curr_slot].recv_buffer.push_back(msg[i]);
					}
					fprintf(stderr,"\n");
					SDL_mutexV(conn->data[conn->curr_slot].recv_mutex);
					
					++(conn->curr_slot);
					--(conn->accept_amount);
					SDL_mutexV(conn->data_mutex);
				}
				else // we are reconnecting
				{
					fprintf(stderr, "reconning\n");
					map<Uint16, Data>::iterator i;
					bool found = false;

					SDL_mutexP(conn->data_mutex);
					for (i = conn->data.begin(); i != conn->data.end(); ++i)
					{
						Data * d = &((*i).second);
						if (d->conn_status == CONN_RECON)
						{
							if (strcmp(d->playername.c_str(), msg) &&
							    strcmp(d->password.c_str(), msg+1+nullch))
							{
								d->conn_status = CONN_OK;
								d->tcp = csd;
								d->last_active = time(NULL);
								--(conn->accept_amount);
								found = true;
								break;
							}
						}
					}

					if (!found)
					{
						fprintf(stderr, "Invalid playername and password.\n");
						SDLNet_TCP_Send(csd, (void*)"QUIT", OP_SIZE);
						SDLNet_TCP_DelSocket(conn->cnx_set, csd);
						SDLNet_TCP_Close(csd);
					}
					SDL_mutexV(conn->data_mutex);
				}
			}

			SDL_mutexP(conn->amount_mutex);
			amt = conn->accept_amount;
			if (!amt) {
				conn->accept_running = false;
			}
			SDL_mutexV(conn->amount_mutex);
		}
		
	}

	SDLNet_TCP_Close(conn->sd);
	return EXIT_SUCCESS;
}

// runs the client thread which recieves information from the server if they send something
// and adds the recieved values to a queue which can be taken from using recv().
int Connection::RunRecv(void* s)
{
	Connection* conn = (Connection*) s;
	Uint8 msg [MSG_SIZE];
	char* cstr; // for debugging purposes
	int result;

	while (true)
	{
		if ((result = SDLNet_CheckSockets(conn->cnx_set, TIMEOUT)) < 0)
		{
			exit(EXIT_FAILURE);
		}

		SDL_mutexP(conn->data_mutex);
		fprintf(stderr,"runnaccept %d, res: %d\n", conn->data.size(), result);
		map<Uint16, Data>::iterator i = conn->data.begin();
		for (; i != conn->data.end(); ++i)
		{
			// get a pointer to the Data in the vector data.
			Data* d = &((*i).second);

			if (d->conn_status != CONN_OK)
				continue;

			// if its not ready we want to make sure its non-idle, assuming we are the server.
			if (!SDLNet_SocketReady(d->tcp))
				continue;

			result = SDLNet_TCP_Recv(d->tcp, msg, MSG_SIZE);
			if(result <= 0)
			{
				fprintf(stderr, "Terminating socket due to error.\n");
//				fprintf(stderr, "SDLNet_TCP_RECV: %s\n", SDLNet_GetError());
				d->conn_status = (conn->connections_locked) ? CONN_RECON : CONN_NONE;
				fprintf(stderr, "Terminating aft conn_status.\n");
				SDLNet_TCP_DelSocket(conn->cnx_set, d->tcp);
				fprintf(stderr, "Terminating aft delsock.\n");
				SDLNet_TCP_Close(d->tcp);
				fprintf(stderr, "Terminating end.\n");
				continue;
			}
			if (conn->isserver)
				d->last_active = time(NULL);

			// debugging code...
				fprintf(stderr, "ret: %d\n", result);

				// for debugging code
				fprintf(stderr, "Recieved Bytes (RECV): ");
				for (int c = 0; c < result; ++c)
					fprintf(stderr,"%d  ", msg[c]);
				fprintf(stderr, "\n");
				if(conn->isserver)
					fprintf(stderr, "SERVER\n");
			// ... end debugging code

			cstr = (char*)msg;

			fprintf(stderr, "Received String: %s\n",msg);

			if(strcmp(cstr, "QUIT") == 0)
			{
				if (!conn->isserver)
				{
					fprintf(stderr, "recieved QUIT request.\n");
					SDLNet_TCP_DelSocket(conn->cnx_set, d->tcp);
					SDLNet_TCP_Close(d->tcp);
					d->conn_status = (conn->connections_locked) ? CONN_RECON : CONN_NONE;
				}
				else
				{
					fprintf(stderr, "client quitting\n");
					SDLNet_TCP_Send(d->tcp, (void*)"QUIT", OP_SIZE);
					SDLNet_TCP_DelSocket(conn->cnx_set, d->tcp);
					SDLNet_TCP_Close(d->tcp);
					d->conn_status = (conn->connections_locked) ? CONN_RECON : CONN_NONE;
				}
			}
			else if (!(conn->isserver) && strcmp(cstr,"NOOP") == 0)
			{
				if (SDLNet_TCP_Send(d->tcp, (void*)"OKOP", OP_SIZE) < OP_SIZE)
				{
					fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
					d->conn_status = (conn->connections_locked) ? CONN_RECON : CONN_NONE;
				}
			}
			else if (strcmp(cstr,"OKOP") != 0) // do nothing if OKOP otherwise this.
			{
				fprintf(stderr, "Added to recv_buffer\n");
				SDL_mutexP(d->recv_mutex);
				for (int i = 0; i < result; ++i)
				{
					d->recv_buffer.push_back(msg[i]);
				}
				SDL_mutexV(d->recv_mutex);
			}
		}
		SDL_mutexV(conn->data_mutex);
	}

	fprintf(stderr, "Ending RunRecv.\n");
	conn->recv_running = false;
	return EXIT_SUCCESS;
}

int Connection::FindNull(const char* packet, int offset, int maxoffset)
{
	int i = 0+offset;
	for (; i < maxoffset && packet[i] != 0x00; ++i)
	{}
	return i;
}


