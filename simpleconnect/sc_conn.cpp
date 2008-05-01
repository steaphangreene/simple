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

#include <set>
using namespace std;

#include <stdio.h>
#include <cassert>
#include <stdlib.h>
#include "sc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <list>
#include <time.h>
#include <SDL/SDL_thread.h>
#include "SDL_net.h"

#define OP_SIZE 5
#define TIME_OUT 50000
#define MAXSOCKETS 17
#define MAXLEN 1024
#define MSG_SIZE 1024

// constructer for creating a client connection.
SimpleConnect::Connection::Connection(TCPsocket sock)
{
 	socket = sock;
	Conn_Setup* setup = new Conn_Setup;
	setup->tcp = sock;
	setup->recv_buffer = recv_buffer;
	setup->send_buffer = send_buffer;
	networking_thread = SDL_CreateThread(RunClient, (void*)setup);
}

// constructer for creating a server connection.
SimpleConnect::Connection::Connection(TCPsocket sock, SimpleConnections sconn)
{
	socket = sock;
	sc = sconn;
	Conn_Setup* setup = new Conn_Setup;
	setup->tcp = sock;
	setup->recv_buffer = recv_buffer;
	setup->send_buffer = send_buffer;
	setup->sc = sconn;
	networking_thread = SDL_CreateThread(RunServer, (void*)setup);
}

SimpleConnect::Connection::~Connection()
{

}

void SimpleConnect::Connection::Add(const Uint8& ref)
{
	Uint8 buf[sizeof(ref)];
	WriteNBO(ref, buf);

	for (unsigned int i = 0 ; i < sizeof(ref); ++i)
	{
		send_buffer.push_back(buf[i]);
	}
}

void SimpleConnect::Connection::Add(const Uint16& ref)
{
	Uint8 buf[sizeof(ref)];
	WriteNBO(ref,buf);

	for (unsigned int i = 0 ; i < sizeof(ref); ++i)
	{
		send_buffer.push_back(buf[i]);
	}
}

void SimpleConnect::Connection::Add(const Uint32& ref)
{
	Uint8 buf[sizeof(ref)];
	WriteNBO(ref,buf);

	for (unsigned int i = 0 ; i < sizeof(ref); ++i)
	{
		send_buffer.push_back(buf[i]);
	}
}

void SimpleConnect::Connection::Add(const string& ref)
{
	for (unsigned int i = 0 ; i <= ref.length(); ++i)
	{
		send_buffer.push_back(ref[i]);
	}
}

// orders the sending of Add()ed values.
void SimpleConnect::Connection::Send()
{
	Uint8 tmp_buffer[send_buffer.size()];

	// copies send_buffer to a Uint8 array of equal size for accurate sending.
	vector<Uint8>::iterator i;
	int count;
	for (count = 0, i = send_buffer.begin(); i != send_buffer.end(); ++i, ++count)
		tmp_buffer[count] = *i;

	SDLNet_TCP_Send(socket, (void*) &tmp_buffer, send_buffer.size());
	send_buffer.clear();
}

void SimpleConnect::Connection::Recv(Uint8& ref)
{
	unsigned int size = 1;
	Uint8 tmp[size];
	unsigned int current;

	vector<Uint8>::iterator i;
	vector<Uint8>::iterator start = recv_buffer.begin();
	for (i = start, current = 0; current < size; ++i, ++current)
		tmp[current] = *i;
	ReadNBO(ref,tmp);
	recv_buffer.erase(start,i);
}

void SimpleConnect::Connection::Recv(Uint16& ref)
{
	unsigned int size = 2;
	Uint8 tmp[size];
	unsigned int current;

	vector<Uint8>::iterator i;
	vector<Uint8>::iterator start = recv_buffer.begin();
	for (i = start, current = 0; current < size; ++i, ++current)
		tmp[current] = *i;
	ReadNBO(ref,tmp);
	recv_buffer.erase(start,i);
}

void SimpleConnect::Connection::Recv(Uint32& ref)
{
	unsigned int size = 4;
	Uint8 tmp[size];
	unsigned int current;

	vector<Uint8>::iterator i;
	vector<Uint8>::iterator start = recv_buffer.begin();
	for (i = start, current = 0; current < size; ++i, ++current)
		tmp[current] = *i;
	ReadNBO(ref,tmp);
	recv_buffer.erase(start,i);
}

void SimpleConnect::Connection::Recv(string& ref)
{
	string s = "";
	vector<Uint8>::iterator i;
	vector<Uint8>::iterator start = recv_buffer.begin();
	for (i = start; *i != 0x00; ++i)
		s.append((const char *) *i);
	s.append((const char *) *i); 
	ref = s;
	++i;
	recv_buffer.erase(start,i);
}


// returns 1 if it is connected, 0 otherwise.
int SimpleConnect::Connection::is_connected()
{
	if(socket)
		return 1;
	else
		return 0;
}

void SimpleConnect::Connection::ClearBuffer()
{
	send_buffer.clear();
}

int SimpleConnect::Connection::RunClient(void* s)
{
	// runs the client thread which recieves information from the server if they send something
        // and adds the recieved values to a queue which can be taken from using recv().
	Conn_Setup* setup = (Conn_Setup*)s;
	
	vector<Uint8>* recv_buffer = &setup->recv_buffer;

	int quit, connected, failed;
	TCPsocket socket = setup->tcp;


	IPaddress* ip = SDLNet_TCP_GetPeerAddress(socket);    // Server address 
	if(!ip)
	{
		printf("SDLNet_TCP_GetPeerAddress: %s\n", SDLNet_GetError());
		return 0;
	}


	// Send and Recieve messages
	quit = 0;
	int result;
	Uint8* msg;
	connected = 1;


	while (!quit)
	{
		failed = 0;

/*
		// Open a connection with the IP provided (listen on the host's port)
		if(!connected) {
			printf("Trying to connect: %d\n", connected);
			socket = SDLNet_TCP_Open(ip);
			if(socket == NULL)
				break;
			connected = 1;
		}
*/
		//fprintf(stderr, "HEREB!!\n");

		while(connected)
		{
			//vector<Uint8> recvd;

			if (!socket) { //Make sure there is a connection
				fprintf(stderr, "SDLNet_TCP: %s\n", SDLNet_GetError());
				connected = 0;
				break;
			}
			result = SDLNet_TCP_Recv(socket, msg, MSG_SIZE);

			if(result < 0)
			{
				fprintf(stderr, "SDLNet_TCP_RECV: %s\n", SDLNet_GetError());
				connected = 0;
				break;
			}
			printf("HERE!!\n");


			char* cstr = (char*)msg;
			if(strcmp(cstr, "QUIT") == 0)
			{
				quit = 1;
				break;
			}
			else if (strcmp(cstr,"NOOP") == 0)
			{
				if (SDLNet_TCP_Send(socket, (void*)"OKOP", OP_SIZE) < OP_SIZE)
				{
					fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
					connected = 0;
					break;
				}
			}
			else //if (strcmp(msg,"NOOP") != 0) {
			{
				printf("Received: %s\n",msg);
				for (int i = 0; i < result; ++i)
				{
					recv_buffer->push_back(msg[i]);
				}
			}
		}
	}
	
	SDLNet_TCP_Close(socket);
	return EXIT_SUCCESS;
}


int SimpleConnect::Connection::RunServer(void* s)
{
	// runs the server thread which recieves information from a SocketSet of clients. This will
	// add their messages to a queue of messages which are delimited by socket number.
	fprintf(stderr, "Server Step 1\n");

	Conn_Setup* setup = (Conn_Setup*)s;
	vector<Uint8>* recv_buffer = &setup->recv_buffer;
	vector<Uint8>* send_buffer = &setup->send_buffer;
	TCPsocket sd = setup->tcp;
	SimpleConnections sc = setup->sc;

	vector<SlotData> all_connections = sc.slots;
	SDLNet_SocketSet cnx_set = sc.tcpset;
	TCPsocket sock; // sock descr & client sock descr
	IPaddress* remoteIP;
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

	list<SlotData> unconnected_list; // see below.
		// list of all possible game players which are not connected (all start connected).
		// when a socket looses its connection it is moved to here.

	if( (SDLNet_TCP_AddSocket(cnx_set, sd)) == -1)
	{
		printf("SDLNet_AddSocket: %s\n", SDLNet_GetError());
		// perhaps you need to restart the set and make it bigger...
	}

	fprintf(stderr, "Server Step 3\n");

	while (true)
	{
		printf("Top of loop\n");
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
					SDLNet_TCP_Send(csd, (void*)"PWRD", OP_SIZE);
					printf("%d sockets in set (Server)\n", ready);
					Conn x;
					x.data = NULL;
					x.last_active = time(NULL);
					socket_list.push_back(x);
					fprintf(stderr, "Yay, socket added to socket_list!\n");
					continue;
				}
			}
		}

		// check if any connections have action.
		list<Conn>::iterator i;
		int bufflen = 0;
		char buffer[MAXLEN];
		for(i=socket_list.begin(); i != socket_list.end(); ++i)
		{
			sock = i->data->sock;

			fprintf(stderr, "inside loop\n");
/*
			if ((remoteIP = SDLNet_TCP_GetPeerAddress(sock)))
			{
				// Print the address, converting in the host format
				printf("Server Host connected: %x %d\n",
						SDLNet_Read32(&remoteIP->host), SDLNet_Read16(&remoteIP->port));
			}
			else {
				fprintf(stderr, "SDLNet_TCP_GetPeerAddress: %s\n", SDLNet_GetError());
			}
*/
			// if not ready, check idleness.
			if(!SDLNet_SocketReady(sock))
			{
				fprintf(stderr, "unrelated error\n");
				if (time(NULL) - i->last_active > 60)
				{
					fprintf(stderr, "unrelated error 2\n");
					SDLNet_TCP_Send(sock, (void*)"QUIT", OP_SIZE);
					SDLNet_TCP_DelSocket(cnx_set,sock);
					SDLNet_TCP_Close(sock);
					unconnected_list.push_front(*(i->data));
					socket_list.erase(i);
					printf("kill socket due to timeout\n");
					break;
				}
				else if (time(NULL) - i->last_active > 30)
				{
					fprintf(stderr, "unrelated error 3\n");
					SDLNet_TCP_Send(sock, (void*)"NOOP", OP_SIZE);
				}
				fprintf(stderr, "GR 2\n");
				continue;
			}

			// else if they are ready...

			fprintf(stderr, "** a connected socket is ready!\n");
			i->last_active = time(NULL);
			if ( (bufflen = SDLNet_TCP_Recv(sock, buffer, MAXLEN-1)) > 0)
			{
				buffer[bufflen] = 0x00;

				// for debugging code
				Uint8* mybuf = (Uint8*) buffer; 
				printf("Recieved Bytes: ");
				for (int c = 0; c < bufflen; ++c)
					printf("%d  ", mybuf[c]);//itoa(msg[i], debug_buffer, 10));
				printf("\n");
//				fprintf(stderr, "Client said: %s\n", buffer);

				if(strcmp(buffer, "QUIT") == 0)
				// Quit the program
				{
					SDLNet_TCP_Send(sock, (void*)"QUIT", OP_SIZE);
					SDLNet_TCP_DelSocket(cnx_set,sock);
					socket_list.erase(i);
					SDLNet_TCP_Close(sock);
					printf("kill socket\n");
					break;
				}
				else if (i->data == NULL)
				// i.e. if it's not signed in.
				{
					fprintf(stderr, "** Not Signed In!\n");
					list<string> tmp = tokenize(buffer);
					if (tmp.size() == 2)
					{
						const char* user_name_tmp = (char*) (tmp.front()).c_str();
						tmp.pop_front();
						const char* pwrd_tmp = (char*) (tmp.front()).c_str();
	
						list<SlotData>::iterator e;
						for(e=unconnected_list.begin(); e != unconnected_list.end(); ++e) 
						{
							char* pname = (char*) e->playername;
							if (strcmp(user_name_tmp, pname) &&
							    strcmp(pwrd_tmp, (char*) *(e->password)) == 0)
							{
								i->data = (SlotData*) &e;
								i->data->sock = sock;
								unconnected_list.erase(e);
								break;
							}
						}
					}

					// if you couldn't find a username and password which fits that entry...
					if (i->data == NULL)
					{
						SDLNet_TCP_Send(sock, (void*)"QUIT", OP_SIZE);
						SDLNet_TCP_DelSocket(cnx_set,sock);
						socket_list.erase(i);
						SDLNet_TCP_Close(sock);
						printf("kill socket (Password Failure)\n");
					}
				} else
					SDLNet_TCP_Send(sock, (void*)"OKOP", OP_SIZE);
			}
			else
			{
				SDLNet_TCP_DelSocket(cnx_set, sock);
				socket_list.erase(i);
				SDLNet_TCP_Close(sock);
				i->data->sock = NULL;
				unconnected_list.push_back(*i->data);
				printf("Terminate connection due to error.\n");
				break;
			}
		}
	}
	//printf("SDLNet_CheckSockets: %s\n", SDLNet_GetError());
	//most of the time this is a system error, where perror might help you.
	perror("SDLNet_CheckSockets");


	SDLNet_TCP_Close(sd);
	SDLNet_Quit();
 
	return EXIT_SUCCESS;

	return 0;
}


list<string> SimpleConnect::Connection::tokenize(const char* packet)
{
	list<string> tokns;
	string pk(packet);
	size_t start = 0;
	for (size_t i = pk.find_first_of(' '); i != string::npos; i = pk.find_first_of(' ', i))
	{
		tokns.push_back(pk.substr(start, i-1));
		start = i;
	}
	tokns.push_back(pk.substr(start, string::npos));
	return tokns;
}
