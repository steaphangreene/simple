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
#define TIME_OUT 50000
#define MAXSOCKETS 17
#define MAXLEN 1024
#define MSG_SIZE 1024
using namespace std;

// constructer for creating a client connection.
/*Connection::Connection()
{
	TCPsocket sd;

	recv_mutex = NULL;
	Conn_Setup* setup = new Conn_Setup;
	setup->recv_buffer = &recv_buffer;

	// open stuff....
	if (!(sd = SDLNet_TCP_Open(&ip)))
	{
		fprintf(stderr, "TCP_OPEN");
		return;
	}
	
 	setup->tcp = socket = sd;
	setup->send_buffer = &send_buffer;


	setup->mutex = recv_mutex;
	networking_thread = SDL_CreateThread(RunClient, (void*)setup);
}*/

// constructer for creating a server connection.
Connection::Connection(const Uint16& port)
{
	this->port = port;
}

/* // old connection stuff...
	SDLNet_SocketSet cnx_set;
	TCPsocket sd; // sock descr & client sock descr
	IPaddress ip;

	// allocate our socket set.
	if( (cnx_set = SDLNet_AllocSocketSet(MAXSOCKETS)) == NULL) {
		printf("SDLNet_AllocSocketSet: %s\n", SDLNet_GetError());
		exit(1); //most of the time this is a major error, but do what you want.
	}

	// Resolving the host using NULL make network interface to listen
	if (SDLNet_ResolveHost(&ip, NULL, 2000) < 0)
	{
		fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	// Open a connection with the IP provided (listen on the host's port)
	if (!(sd = SDLNet_TCP_Open(&ip)))
	{
		fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	sc = new SimpleConnections;
	Conn_Setup* setup = new Conn_Setup;
	setup->tcp = socket = sd;
	setup->recv_buffer = &recv_buffer;
	setup->send_buffer = &send_buffer;
	setup->sc = *sc;

	recv_mutex = NULL;
	setup->mutex = recv_mutex;
	networking_thread = SDL_CreateThread(RunServer, (void*)setup);
}*/

Connection::~Connection()
{
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
	
	SDL_mutexP(recv_mutex);
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
	SDL_mutexV(recv_mutex);
}

void Connection::Recv(int slot, Uint16& ref)
{
	vector<Uint8>* recv_buffer = &data[slot].recv_buffer;
	unsigned int size = 2;
	Uint8 tmp[size];
	unsigned int current;

	SDL_mutexP(recv_mutex);
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
	SDL_mutexV(recv_mutex);
}

void Connection::Recv(int slot, Uint32& ref)
{
	vector<Uint8>* recv_buffer = &data[slot].recv_buffer;
	unsigned int size = 4;
	Uint8 tmp[size];
	unsigned int current;
	
	SDL_mutexP(recv_mutex);
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
	SDL_mutexV(recv_mutex);
}

void Connection::Recv(int slot, string& ref)
{
	vector<Uint8>* recv_buffer = &data[slot].recv_buffer;
	string s = "";

	vector<Uint8>::iterator i;

	SDL_mutexP(recv_mutex);
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
	SDL_mutexV(recv_mutex);
}


// returns 1 if it is connected, 0 otherwise.
int Connection::IsConnected(int slot)
{
	return data[slot].tcp != NULL;
}

int Connection::RecvBufferSize(int slot)
{
	return data[slot].recv_buffer.size();
}

int Connection::NumConnections()
{
	return data.size();
}

// Disconnectes from the server by sending it the QUIT operator which will force the removal of the socket.
// TODO: see if this works.
void Connection::Disconnect(int slot)
{
	if (data[slot].connected)
	{
		data[slot].connected = false;
	}
	//SDLNet_TCP_Close(data[slot].tcp);
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
	return data[slot].playername;
}

// TODO
int Connection::Connect(const IPaddress& ip, const string& name="", const string& password="")
{
	return 0;
}

void Connection::StopAccepting()
{

}

void Connection::StartAccepting(int slot)
{

}


// runs the client thread which recieves information from the server if they send something
// and adds the recieved values to a queue which can be taken from using recv().
int Connection::RunClient(void* s)
{
/*
	Conn_Setup* setup = (Conn_Setup*)s;
	SDL_mutex* recv_mutex = setup->mutex;
	vector<Uint8>* recv_buffer = setup->recv_buffer;

	int quit = 0, connected = 1;
	TCPsocket csd = setup->tcp;
	int result;
	Uint8 msg [MSG_SIZE];

	IPaddress* ip = SDLNet_TCP_GetPeerAddress(csd);    // Server address 
	if(!ip)
	{
		printf("SDLNet_TCP_GetPeerAddress: %s\n", SDLNet_GetError());
		return 0;
	}

//	char* cmsg [1024];
//	int ret = SDLNet_TCP_Recv(csd, cmsg, 1024);
//	fprintf(stderr, "ret: %d, cmsg: %s\n", ret, cmsg);


	while (!quit)
	{
		// Open a connection with the IP provided (listen on the host's port)
		if(!connected) {
			if (!csd)
				printf("not csd\n");
			printf("Trying to connect: %d\n", connected);
			csd = SDLNet_TCP_Open(ip);
			if(!csd)
			{
				fprintf(stderr, "reconnection failed\n");
				break;
			}
			connected = 1;
		}
		fprintf(stderr, "HEREB!!\n");

		while(connected)
		{
			if (!csd) { //Make sure there is a connection
				fprintf(stderr, "SDLNet_TCP: %s\n", SDLNet_GetError());
				connected = 0;
				break;
			}

			fprintf(stderr, "recieving...\n");
			result = SDLNet_TCP_Recv(csd, msg, MSG_SIZE);
			fprintf(stderr, "ret: %d\n", result); //, cmsg: %s\n", ret, cmsg);
			if(result <= 0)
			{
				fprintf(stderr, "SDLNet_TCP_RECV: %s\n", SDLNet_GetError());
				connected = 0;
				break;
			}

			// for debugging code
			fprintf(stderr, "Recieved Bytes: ");
			for (int c = 0; c < result; ++c)
				fprintf(stderr,"%d  ", msg[c]);//itoa(msg[i], debug_buffer, 10));
			fprintf(stderr, "\n");

			char* cstr = (char*)msg;
			printf("Received String: %s\n",msg);
			if(strcmp(cstr, "QUIT") == 0)
			{
				fprintf(stderr, "recieved QUIT request.\n");
				quit = 1;
				break;
			}
			else if (strcmp(cstr,"NOOP") == 0)
			{
				if (SDLNet_TCP_Send(csd, (void*)"OKOP", OP_SIZE) < OP_SIZE)
				{
					fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
					connected = 0;
					break;
				}
			}
			else if (strcmp(cstr,"OKOP") != 0) // do nothing if OKOP otherwise this.
			{
				printf("Received: %s\n",msg);
				SDL_mutexP(recv_mutex);
				for (int i = 0; i < result; ++i)
				{
					fprintf(stderr, "  Added %c to recv_buffer\n", msg[i]);
					recv_buffer->push_back(msg[i]);
				}
				fprintf(stderr,"\nend recvd\n");
				SDL_mutexV(recv_mutex);
			}
		}
	}

	fprintf(stderr, "Closing connection.\n");
	SDLNet_TCP_Close(csd);
	delete setup;
*/
	return EXIT_SUCCESS;
}


int Connection::RunServer(void* s)
{
/*
	// runs the server thread which recieves information from a SocketSet of clients. This will
	// add their messages to a queue of messages which are delimited by socket number.
	Conn_Setup* setup = (Conn_Setup*)s;
	vector<Uint8>* recv_buffer = setup->recv_buffer;
	//vector<Uint8>* send_buffer = setup->send_buffer;
	TCPsocket sd = setup->tcp;
	SimpleConnections sc = setup->sc;
	SDL_mutex* recv_mutex = setup->mutex;


	vector<SlotData> all_connections = sc.slots;
	SDLNet_SocketSet cnx_set = sc.tcpset;
	TCPsocket sock; // sock descr & client sock descr
	//IPaddress* remoteIP;
	int ready = 0;
	list<Conn> socket_list;

	vector<SlotData>::iterator i = all_connections.begin();
	for (; i != all_connections.end(); ++i)
	{
		Conn x;
		x.last_active = time(NULL);
		x.data = (SlotData*) &(*i); // makes it compile!
		socket_list.push_back(x);
	}

	list<SlotData*> unconnected_list; // see below.
		// list of all possible game players which are not connected (all start connected).
		// when a socket looses its connection it is moved to here.

	if( (SDLNet_TCP_AddSocket(cnx_set, sd)) == -1)
	{
		//fprintf(stderr, "SDLNet_AddSocket: %s\n", SDLNet_GetError());
		// perhaps you need to restart the set and make it bigger...
	}

	while (true)
	{
		//fprintf(stderr,"Top of loop\n");
		//fprintf(stderr, "uncon: %d\n", int(unconnected_list.size()));

		if ( (ready = SDLNet_CheckSockets(cnx_set, TIME_OUT)) == -1)
			break;

		fprintf(stderr, "%d sockets ready!\n", ready);

		// server gets a new connection?
		if ( (SDLNet_SocketReady(sd)) )
		{
			TCPsocket csd;
			printf("** server-socket is ready!\n");
			if ( (csd = SDLNet_TCP_Accept(sd)) )
			{
				if( (ready = SDLNet_TCP_AddSocket(cnx_set, csd)) == -1)
				{
					printf("SDLNet_AddSocket: %s\n", SDLNet_GetError());
					// perhaps restart the set and make it bigger...

					SDLNet_TCP_Send(csd, (void*)"QUIT", OP_SIZE);
					printf("%d sockets in set (fail)\n", ready);
					SDLNet_TCP_Close(csd);
				} else {
					// query for password when first connected to server
					// SDLNet_TCP_Send(csd, (void*)"PWRD", OP_SIZE);
					printf("%d sockets in set (Server)\n", ready);
					Conn x;
					x.data = new SlotData;
					x.data->sock = csd;
					x.last_active = time(NULL);
					x.data->password[0] = 0x00;
					socket_list.push_back(x);
					fprintf(stderr, "Yay, socket added to socket_list!\n");
					continue;
				}
			}
		}

		// check if any connections have action.
		list<Conn>::iterator i;
		int bufflen;
		//Uint8 int8buff[MAXLEN];
		char buffer[MAXLEN];
		for(i=socket_list.begin(); i != socket_list.end(); ++i)
		{
			sock = i->data->sock;

//			if ((remoteIP = SDLNet_TCP_GetPeerAddress(sock)))
//			{
//				// Print the address, converting in the host format
//				printf("Server Host connected: %x %d\n",
//						SDLNet_Read32(&remoteIP->host), SDLNet_Read16(&remoteIP->port));
//			}
//			else {
//				fprintf(stderr, "SDLNet_TCP_GetPeerAddress: %s\n", SDLNet_GetError());
//			}

			// if not ready, check idleness.
			if(!SDLNet_SocketReady(sock))
			{
				if (time(NULL) - i->last_active > 60)
				{
					SDLNet_TCP_Send(sock, (void*)"QUIT", OP_SIZE);
					SDLNet_TCP_DelSocket(cnx_set,sock);
					SDLNet_TCP_Close(sock);
					unconnected_list.push_front(i->data);
					socket_list.erase(i);
					fprintf(stderr, "kill socket due to timeout\n");
					break;
				}
				else if (time(NULL) - i->last_active > 30)
				{
					SDLNet_TCP_Send(sock, (void*)"NOOP", OP_SIZE);
				}
				continue;
			}

			// else if they are ready...

			fprintf(stderr, "** a connected socket is ready!\n");
			i->last_active = time(NULL);
			if ( (bufflen = SDLNet_TCP_Recv(sock, buffer, MAXLEN-1)) > 0)
			{
				printf("bufflen %d\n", bufflen);
				buffer[bufflen+1] = 0x00;

				// for debugging code
				//Uint8* mybuf = (Uint8*) buffer; 
				//printf("Recieved Bytes: ");
				//for (int c = 0; c < bufflen; ++c)
				//	printf("%d  ", mybuf[c]);//itoa(msg[i], debug_buffer, 10));
				//printf("\n");

				fprintf(stderr, "Client said: %s\n", buffer);

				if(strcmp(buffer, "QUIT") == 0)
				// Quit the program
				{
					SDLNet_TCP_Send(sock, (void*)"QUIT", OP_SIZE);
					SDLNet_TCP_DelSocket(cnx_set,sock);
					socket_list.erase(i);
					SDLNet_TCP_Close(sock);
					printf("kill socket (requested)\n");
					break;
				}
				else if (i->data->password[0] == 0x00)
				// i.e. if it's not signed in.
				{
					fprintf(stderr, "** Not Signed In!\n");
					list<string> tmp = tokenize(buffer);

					if (tmp.size() == 2)
					{
						// collect the char* from the tokenized string.
						const char* user_name_tmp = (char*) (tmp.front()).c_str();
						tmp.pop_front();
						const char* pwrd_tmp = (char*) (tmp.front()).c_str();

						list<SlotData*>::iterator e;
						for(e=unconnected_list.begin(); e != unconnected_list.end(); ++e) 
						{
							fprintf(stderr, "gag");
							char* pwrd = (char*) (*e)->password;
							char* pname = (char*) (*e)->playername;
							if (strcmp(user_name_tmp, pname) == 0 &&
							    strcmp(pwrd_tmp, pwrd) == 0)
							{
								SDLNet_TCP_Send(sock, (void*)"OKOP", OP_SIZE);
// 	 							fprintf(stderr, "drg_if\n");
								unconnected_list.erase(e);
 								i->data = (SlotData*) *e;
								i->data->sock = sock;
								break;
							}
 						//	fprintf(stderr, "drg\n");
						}
					}

					// if you couldn't find a username and password which fits that entry...
					fprintf(stderr, "pwd[0]: %d", i->data->password[0]);
					if (i->data->password[0] == 0x00)
					{
						SDLNet_TCP_Send(sock, (void*)"QUIT", OP_SIZE);
						SDLNet_TCP_DelSocket(cnx_set,sock);
						socket_list.erase(i);
						SDLNet_TCP_Close(sock);
						--i;
						fprintf(stderr, "kill socket (Password Failure)\n");
						break;
					}
				}
				else
				{
					SDL_mutexP(recv_mutex);
					for (int i = 0; i < bufflen; ++i)
					{
						fprintf(stderr, "  Added %c to recv_buffer\n", buffer[i]);
						recv_buffer->push_back((Uint8)buffer[i]);
					}
					SDL_mutexV(recv_mutex);
					SDLNet_TCP_Send(sock, (void*)"OKOP", OP_SIZE);
					
					for (int i = 0; i < result; ++i)
					{
						fprintf(stderr, "  Added %c to recv_buffer\n", msg[i]);
						recv_buffer->push_back(msg[i]);
					}
					fprintf(stderr,"\nend recvd\n");
				}
			}
			else
			{
				SDLNet_TCP_DelSocket(cnx_set, sock);
				socket_list.erase(i);
				SDLNet_TCP_Close(sock);
				i->data->sock = NULL;
				unconnected_list.push_back(i->data);
				printf("Terminate connection due to error.\n");
				break;
			}
		}
		fprintf(stderr, "BOTTOM OF LOOP\n");
	}
	//printf("SDLNet_CheckSockets: %s\n", SDLNet_GetError());
	//most of the time this is a system error, where perror might help you.
	perror("SDLNet_CheckSockets");
	
	SDLNet_TCP_Close(sd);
	SDLNet_Quit();

	delete setup;
*/
	return EXIT_SUCCESS;
}

/*
list<string> Connection::tokenize(const char* packet)
{
	list<string> tokns;
	string pk(packet);
	size_t start = 0;
	for (size_t i = pk.find_first_of(' '); i != string::npos; i = pk.find_first_of(' ', start))
	{
		tokns.push_back(pk.substr(start, i));
		start = i+1;
	}
	tokns.push_back(pk.substr(start, string::npos));
	return tokns;
}
*/


