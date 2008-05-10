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

#include <cassert>
#include "simplenetwork.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <list>
#include <vector>
#include <time.h>
#include "SDL_thread.h"
#include "SDL_net.h"

#define OP_SIZE 5
#define TIMEOUT 2500
#define MAXSOCKETS 100
#define MAXLEN 1024
#define MSG_SIZE 1024
using namespace std;

SimpleNetwork *SimpleNetwork::current = NULL;

// constructer for creating a server connection.
SimpleNetwork::SimpleNetwork(Uint16 port)
{
	if(current)
	{
		fprintf(stderr, "ERROR: Created mulitple SimpleNetwork instances!\n");
		exit(1);
	}
	current = this;
	this->port = port;
	recv_thread = NULL;
	accept_thread = NULL;
	recv_running = false;
	accept_running = false;
	accept_mutex = NULL;
	curr_slot = 0;
	connections_locked = false;
	isserver = false;
	sd = NULL;

	accept_amount = 0;
	// allocate socketset.
	if( (cnx_set = SDLNet_AllocSocketSet(MAXSOCKETS)) == NULL) {
		printf("SDLNet_AllocSocketSet: %s\n", SDLNet_GetError());
		exit(1); //most of the time this is a major error, but do what you want.
	}

	if (!recv_running)
	{
		recv_running = true;
		recv_thread = SDL_CreateThread(recv_handler, (void*) this);
	}
}

SimpleNetwork::~SimpleNetwork()
{
	if (recv_running)
	{
		recv_running = false;
		SDL_WaitThread(recv_thread, NULL);
	}
}

void SimpleNetwork::Add(int slot, const Uint8& ref) {
	Uint8 buf[sizeof(ref)];
	WriteNBO(ref, buf);

	for (unsigned int i = 0 ; i < sizeof(ref); ++i)
	{
		data[slot].send_buffer.push_back(buf[i]);
	}
}

void SimpleNetwork::Add(int slot,const Uint16& ref)
{
	Uint8 buf[sizeof(ref)];
	WriteNBO(ref,buf);

	for (unsigned int i = 0 ; i < sizeof(ref); ++i)
	{
		data[slot].send_buffer.push_back(buf[i]);
	}
}

void SimpleNetwork::Add(int slot,const Uint32& ref)
{
	Uint8 buf[sizeof(ref)];
	WriteNBO(ref,buf);

	for (unsigned int i = 0 ; i < sizeof(ref); ++i)
	{
		data[slot].send_buffer.push_back(buf[i]);
	}
}

void SimpleNetwork::Add(int slot, const string& ref)
{
	for (unsigned int i = 0 ; i <= ref.length(); ++i)
	{
		data[slot].send_buffer.push_back(ref[i]);
	}
}

// orders the sending of Add()ed values.
void SimpleNetwork::Send(int slot)
{
	if (data.count(slot) > 0 && data[slot].conn_status == SN_CONN_OK)
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
}

void SimpleNetwork::Recv(int slot, Uint8& ref)
{
	vector<Uint8>* recv_buffer = &data[slot].recv_buffer;
	unsigned int size = 1;
	Uint8 tmp[size];
	unsigned int cur;
	
	SDL_mutexP(data[slot].recv_mutex);
	vector<Uint8>::iterator i;
	vector<Uint8>::iterator start = recv_buffer->begin();
	for (i = start, cur= 0; i != recv_buffer->end() && cur< size; ++i, ++cur)
	{
		fprintf(stderr, " %X  ", *i);
		tmp[cur] = *i;
	}
	fprintf(stderr, "\n");
	ReadNBO(ref,tmp);
	recv_buffer->erase(start,i);
	SDL_mutexV(data[slot].recv_mutex);
}

void SimpleNetwork::Recv(int slot, Uint16& ref)
{
	vector<Uint8>* recv_buffer = &data[slot].recv_buffer;
	unsigned int size = 2;
	Uint8 tmp[size];
	unsigned int cur;

	SDL_mutexP(data[slot].recv_mutex);
	vector<Uint8>::iterator i;
	vector<Uint8>::iterator start = recv_buffer->begin();
	for (i = start, cur= 0; i != recv_buffer->end() && cur< size; ++i, ++cur)
	{
		fprintf(stderr, " %X  ", *i);
		tmp[cur] = *i;
	}
	
	fprintf(stderr, "\n");
	ReadNBO(ref,tmp);
	recv_buffer->erase(start,i);
	SDL_mutexV(data[slot].recv_mutex);
}

void SimpleNetwork::Recv(int slot, Uint32& ref)
{
	vector<Uint8>* recv_buffer = &data[slot].recv_buffer;
	unsigned int size = 4;
	Uint8 tmp[size];
	unsigned int cur;
	
	SDL_mutexP(data[slot].recv_mutex);
	vector<Uint8>::iterator i;
	vector<Uint8>::iterator start = recv_buffer->begin();
	for (i = start, cur = 0; i != recv_buffer->end() && cur < size; ++i, ++cur)
	{
		fprintf(stderr, " %X  ", *i);
		tmp[cur] = *i;
	}
	fprintf(stderr, "\n");
	ReadNBO(ref,tmp);
	recv_buffer->erase(start,i);
	SDL_mutexV(data[slot].recv_mutex);
}

void SimpleNetwork::Recv(int slot, string& ref)
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
SN_Status SimpleNetwork::IsConnected(int slot)
{
	if(data.count(slot) < 1)
	{
		return SN_CONN_NONE;
	}
	return (SN_Status)data[slot].conn_status;
}

int SimpleNetwork::RecvBufferSize(int slot)
{
	int ret = 0;
	if(data.count(slot) > 0)
	{
		SDL_mutexP(data[slot].recv_mutex);
		ret = data[slot].recv_buffer.size();
		SDL_mutexV(data[slot].recv_mutex);
	}
	return ret;
}

int SimpleNetwork::NumConnections()
{
	return data.size();
}

// Disconnectes from the server by sending it the QUIT operator which will force the removal of the socket.
void SimpleNetwork::Disconnect(int slot)
{
	if (data.count(slot) > 0 && data[slot].conn_status == SN_CONN_OK)
	{
		data[slot].conn_status = connections_locked ? SN_CONN_RECON : SN_CONN_NONE;
		assert(data[slot].tcp);
		SDLNet_TCP_DelSocket(cnx_set, data[slot].tcp);
		SDLNet_TCP_Send(data[slot].tcp, (void*)"QUIT",OP_SIZE);
		SDLNet_TCP_Close(data[slot].tcp);
	}
}

void SimpleNetwork::SetPort(Uint16 port)
{
	this->port = port;
}

void SimpleNetwork::SetName(int slot, const string& str)
{
	data[slot].playername = str;
}

void SimpleNetwork::SetPassword(int slot, const string& str)
{
	data[slot].password = str;
}

string SimpleNetwork::GetName(int slot)
{
	return (data[slot].conn_status == SN_CONN_NONE) ? "" : data[slot].playername;
}

// returns the slot number on success, -1 on failure.
int SimpleNetwork::Connect(IPaddress& ip, const string& name, const string& password)
{
	int cnx_amt;

	data[curr_slot].playername = name;
	data[curr_slot].password = password;

	// open stuff....
	if (!(data[curr_slot].tcp = SDLNet_TCP_Open(&ip)))
	{
		fprintf(stderr, "TCP_OPEN");
		data.erase(curr_slot);
		return -1;
	}

	if( (cnx_amt = SDLNet_TCP_AddSocket(cnx_set, data[curr_slot].tcp)) == -1)
	{
		fprintf(stderr,"SDLNet_AddSocket: %s\n", SDLNet_GetError());
		// perhaps restart the set and make it bigger...
		SDLNet_TCP_Send(data[curr_slot].tcp, (void*)"QUIT", OP_SIZE);
		fprintf(stderr,"%d sockets in set (fail)::Connect()\n", cnx_amt);
		SDLNet_TCP_Close(data[curr_slot].tcp);
		data.erase(curr_slot);
		return -1;
	}

	// send the name and password to the server on connection.
	Uint8 bytes[password.length() + name.length() + 2];
	const char* s = name.c_str();
	const char* s2 = password.c_str();
	memcpy(bytes, s, name.length()+1);
	memcpy(bytes+name.length()+1, s2, password.length()+1);
	SDLNet_TCP_Send(data[curr_slot].tcp, (void*) bytes, password.length() + name.length() + 2);

	data[curr_slot].conn_status = SN_CONN_OK;
	data[curr_slot].recv_mutex = SDL_CreateMutex();
	data[curr_slot].last_active = time(NULL);


	fprintf(stderr, "connect() %d\n", curr_slot);
	// add this new connection to the connection set.
	// make sure runclient is running for this connection.

	return curr_slot++;
}

void SimpleNetwork::UnlockConnections()
{
	connections_locked = false;
}

void SimpleNetwork::LockConnections()
{
	connections_locked = true;	
}

void SimpleNetwork::StopAccepting()
{
	SDL_mutexP(accept_mutex);
	if (accept_running)
	{
		accept_amount = 0;
		accept_running = false;
		SDL_WaitThread(accept_thread, NULL);
		SDLNet_TCP_Close(sd);
	}
	SDL_mutexV(accept_mutex);
}

//if not already accepting, this starts a thread which accepts 'amount number of
//new sockets.
void SimpleNetwork::StartAccepting(int amount)
{
	IPaddress ip;
	// allocate our socket set iff server_ss hasn't already been allocated.

	if (accept_mutex == NULL)
		accept_mutex =	SDL_CreateMutex();

	// Resolving the host using NULL make network interface to listen
	if (SDLNet_ResolveHost(&ip, NULL, port) < 0)
	{
		fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	// Open a connection with the IP provided (listen on the host's port)
	SDL_mutexP(accept_mutex);	
	if (!accept_running)
	{
//		if (!isserver)
//		{
			fprintf(stderr, "Open server");
			isserver = true;
			if (!(sd = SDLNet_TCP_Open(&ip)))
			{
				fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
				exit(EXIT_FAILURE);
			}
//		}
	
		accept_amount = amount;
		accept_running = true;
		accept_thread = SDL_CreateThread(accept_handler, (void*) this);
	}
	else {	
		accept_amount += amount;
	}
	SDL_mutexV(accept_mutex);
}

// accepts and adds new sockets to data which have been accepted into
// the listened-to things.
// s represents a pointer to a SimpleNetwork class which has an open server
// descripter.
int SimpleNetwork::accept_handler(void* s)
{
	return ((SimpleNetwork*)(s))->RunAccept();
}

int SimpleNetwork::RunAccept()
{
	fprintf(stderr, "RUNACCEPT()\n");
	TCPsocket csd;
	int cnx_amt;
	int amt = accept_amount;
	char msg[MSG_SIZE];
	int nullch, nullch2;

	while (amt && accept_running)
	{
		if ( (csd = SDLNet_TCP_Accept(sd)) )
		{
			fprintf(stderr, "New Connection!\n");
			if( (cnx_amt = SDLNet_TCP_AddSocket(cnx_set, csd)) == -1)
			{
				fprintf(stderr,"SDLNet_AddSocket: %s\n", SDLNet_GetError());
				// perhaps restart the set and make it bigger...
				SDLNet_TCP_Send(csd, (void*)"QUIT", OP_SIZE);
				fprintf(stderr,"%d sockets in set (fail)\n", cnx_amt);
				SDLNet_TCP_Close(csd);
				accept_amount = 0;
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
				fprintf (stderr, "nullch: %d, nch2: %d, res:  %d\n", nullch, nullch2, result);
				if (nullch == result-1 || msg[nullch2] != 0x00)
				{
					fprintf(stderr, "Invalid login message.\n");
					SDLNet_TCP_Send(csd, (void*)"QUIT", OP_SIZE);
					SDLNet_TCP_DelSocket(cnx_set, csd);
					SDLNet_TCP_Close(csd);
					continue;
				}

				if (connections_locked == false)
				{
					fprintf(stderr, "ADDING NEW CONNECTION\n");
					data[curr_slot].recv_mutex = SDL_CreateMutex();
					data[curr_slot].tcp = csd;
					data[curr_slot].last_active = time(NULL);
					data[curr_slot].conn_status = SN_CONN_OK;
					data[curr_slot].password = msg;
					data[curr_slot].playername = msg+nullch+1;

					SDL_mutexP(data[curr_slot].recv_mutex);
					for (int i = nullch2+1; i < result; ++i)
					{
						if (i == nullch2+1) fprintf(stderr, "Adding to recv_buffer: ");
						fprintf(stderr,"%X  ", msg[i]);
						data[curr_slot].recv_buffer.push_back(msg[i]);
					}
					fprintf(stderr,"\n");
					SDL_mutexV(data[curr_slot].recv_mutex);
					
					++(curr_slot);
					--(accept_amount);
				}
				else // we are reconnecting
				{
					fprintf(stderr, "RECONNECTING\n");
					map<Uint16, Data>::iterator i;
					bool found = false;

					for (i = data.begin(); i != data.end(); ++i)
					{
						Data * d = &((*i).second);
						if (d->conn_status == SN_CONN_RECON)
						{
							fprintf(stderr,"reconn?//.\n");
							if (strcmp(d->playername.c_str(), msg) &&
							    strcmp(d->password.c_str(), msg+1+nullch))
							{
								fprintf(stderr,"Reconnection.\n");
								d->conn_status = SN_CONN_OK;
								d->tcp = csd;
								d->last_active = time(NULL);
								--(accept_amount);
								found = true;
								break;
							}
						}
					}

					if (!found)
					{
						fprintf(stderr, "Invalid playername and password.\n");
						SDLNet_TCP_Send(csd, (void*)"QUIT", OP_SIZE);
						SDLNet_TCP_DelSocket(cnx_set, csd);
						SDLNet_TCP_Close(csd);
					}
				}
			}

			SDL_mutexP(accept_mutex);
			amt = accept_amount;
			if (!amt) {
				accept_running = false;
			}
			SDL_mutexV(accept_mutex);
		}
		SDL_Delay(10);
	}

	return EXIT_SUCCESS;
}

// runs the client thread which recieves information from the server if they send something
// and adds the recieved values to a queue which can be taken from using recv().
int SimpleNetwork::recv_handler(void* s)
{
	return ((SimpleNetwork*)(s))->RunRecv();
}

int SimpleNetwork::RunRecv()
{
	Uint8 msg [MSG_SIZE];
	char* cstr; // for debugging purposes
	int result;

	while (recv_running)
	{
		if ((result = SDLNet_CheckSockets(cnx_set, TIMEOUT)) < 0)
		{
			fprintf(stderr, "CheckSockets() error %s\n", SDLNet_GetError());
			perror("empty set?\n");
		//	exit(EXIT_FAILURE);
		}

		//fprintf(stderr,"runrecv %d, res: %d\n", (int)(data.size()), result);
		map<Uint16, Data>::iterator i = data.begin();
		for (; i != data.end(); ++i)
		{
			// get a pointer to the Data in the vector data.
			Data* d = &((*i).second);

			if (d->conn_status != SN_CONN_OK)
				continue;

			// if its not ready we want to make sure its non-idle, assuming we are the server.
			if (!SDLNet_SocketReady(d->tcp))
			{
				if (time(NULL) - d->last_active > 10)
				{
					fprintf(stderr, "Deleting connection.\n");
					SDLNet_TCP_Send(d->tcp, (void*)"QUIT", OP_SIZE);
					SDLNet_TCP_DelSocket(cnx_set, d->tcp);
					SDLNet_TCP_Close(d->tcp);
					d->conn_status = (connections_locked) ? SN_CONN_RECON : SN_CONN_NONE;
					if (isserver && connections_locked)
					{
						fprintf(stderr,"will recon\n");
						StartAccepting(1);
					}
				}
				else if (time(NULL) - d->last_active >= 5)
				{
					fprintf(stderr, "Sending NOOP\n");
					SDLNet_TCP_Send(d->tcp, (void*)"NOOP", OP_SIZE);
				}
				continue;
			}

			result = SDLNet_TCP_Recv(d->tcp, msg, MSG_SIZE);
			if(result <= 0)
			{
				fprintf(stderr, "Terminating socket due to error.\n");
				d->conn_status = (connections_locked) ? SN_CONN_RECON : SN_CONN_NONE;
				SDLNet_TCP_DelSocket(cnx_set, d->tcp);
				SDLNet_TCP_Close(d->tcp);
				if (isserver && connections_locked)
				{
					fprintf(stderr,"will recon\n");
					StartAccepting(1);
				}
				continue;
			}
			d->last_active = time(NULL);

			// debugging code...
				fprintf(stderr, "ret: %d\n", result);

				// for debugging code
				fprintf(stderr, "Recieved Bytes (RECV): ");
				for (int c = 0; c < result; ++c)
					fprintf(stderr,"%d  ", msg[c]);
				fprintf(stderr, "\n");
				if(isserver)
					fprintf(stderr, "SERVER\n");
			// ... end debugging code

			cstr = (char*)msg;

			fprintf(stderr, "Received String: %s\n",msg);

			if(strcmp(cstr, "QUIT") == 0)
			{
				if (!isserver)
				{
					fprintf(stderr, "recieved QUIT request.\n");
					SDLNet_TCP_DelSocket(cnx_set, d->tcp);
					SDLNet_TCP_Close(d->tcp);
					d->conn_status = (connections_locked) ? SN_CONN_RECON : SN_CONN_NONE;
				}
				else
				{
					fprintf(stderr, "client quitting\n");
					SDLNet_TCP_Send(d->tcp, (void*)"QUIT", OP_SIZE);
					SDLNet_TCP_DelSocket(cnx_set, d->tcp);
					SDLNet_TCP_Close(d->tcp);
					d->conn_status = (connections_locked) ? SN_CONN_RECON : SN_CONN_NONE;
					if (connections_locked)
					{
						fprintf(stderr,"will recon\n");
						StartAccepting(1);
					}
				}
			}
			else if (strcmp(cstr,"NOOP") == 0)
			// if a NOOP is sent, an OKOP verification is sent back, this is to assert
			// that their is still a live connection between the server and the client.
			// A NOOP can be requested by either side of the connection.
			{
				fprintf(stderr, "Sending OKOP\n");
				if (SDLNet_TCP_Send(d->tcp, (void*)"OKOP", OP_SIZE) < OP_SIZE)
				{
					fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
					SDLNet_TCP_DelSocket(cnx_set, d->tcp);
					SDLNet_TCP_Close(d->tcp);
					d->conn_status = (connections_locked) ? SN_CONN_RECON : SN_CONN_NONE;
					if (isserver && connections_locked)
					{
						fprintf(stderr,"will recon\n");
						StartAccepting(1);
					}
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
		SDL_Delay(10);
	}

	fprintf(stderr, "Ending RunRecv.\n");
	return EXIT_SUCCESS;
}

int SimpleNetwork::FindNull(const char* packet, int offset, int maxoffset)
{
	int i = 0+offset;
	for (; i < maxoffset && packet[i] != 0x00; ++i)
	{}
	return i;
}


