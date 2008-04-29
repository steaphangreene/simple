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

#include "sc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <list>
#include <time.h>
#include <SDL/SDL_thread.h>
#include "SDL_net.h"

#define OP_SIZE 5
#define TIME_OUT 5000
#define MAXSOCKETS 17
#define MAXLEN 1024
#define MSG_SIZE 1024

// constructer for creating a client connection.
SimpleConnect::Connection::Connection(TCPsocket sock)
{
 	socket = sock;
	Conn_Setup setup;
	setup.tcp = sock;
	setup.recv_buffer = recv_buffer;
	setup.send_buffer = send_buffer;
	networking_thread = SDL_CreateThread(RunClient, (void*)&setup);
}

// constructer for creating a server connection.
SimpleConnect::Connection::Connection(TCPsocket sock, SimpleConnections sconn)
{
	socket = sock;
	sc = sconn;
	Conn_Setup setup;
	setup.tcp = sock;
	setup.recv_buffer = recv_buffer;
	setup.send_buffer = send_buffer;
	setup.sc = sc;
	networking_thread = SDL_CreateThread(RunServer, (void*)&setup);
}

SimpleConnect::Connection::~Connection()
{

}

void SimpleConnect::Connection::Add(const Uint8& ref)
{
	send_buffer.push((void*)ref);
}

void SimpleConnect::Connection::Add(const Uint16& ref)
{
	send_buffer.push((void*)ref);
}

void SimpleConnect::Connection::Add(const Uint32& ref)
{
	send_buffer.push((void*)ref);
}

void SimpleConnect::Connection::Add(const string& ref)
{
	send_buffer.push((void*)&ref);
}


int SimpleConnect::Connection::Send()
{
	// orders the sending of Add()ed values.
	return 0;
}



void SimpleConnect::Connection::Recv(Uint8& ref)
{
	//recv_buffer.front((Tp)ref);
	//recv_buffer.pop();	
}

void SimpleConnect::Connection::Recv(Uint16& ref)
{
}

void SimpleConnect::Connection::Recv(Uint32& ref)
{
}
void SimpleConnect::Connection::Recv(string& ref)
{
}


// returns 1 if it is connected, 0 otherwise.
int SimpleConnect::Connection::is_connected()
{
	if(socket)
		return 1;
	else
		return 0;
}

int SimpleConnect::Connection::RunClient(void* s)
{
	// runs the client thread which recieves information from the server if they send something
        // and adds the recieved values to a queue which can be taken from using recv().
	Conn_Setup* setup = (Conn_Setup*)s;
	
	queue<string> recv_buffer = setup->recv_buffer;
	queue<void*> send_buffer = setup->send_buffer;

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
	char msg[MSG_SIZE];
	connected = 1;


	while (!quit)
	{
		failed = 0;

		// Open a connection with the IP provided (listen on the host's port)
		if(!connected) {
			printf("Trying to connect \n");
			socket = SDLNet_TCP_Open(ip);
			if(socket == NULL)
				break;
			connected = 1;
		}

		while(connected)
		{

			if (!socket) { //Make sure there is a connection
				fprintf(stderr, "SDLNet_TCP: %s\n", SDLNet_GetError());
				connected = 0;
				break;
			}

			result = SDLNet_TCP_Recv(socket,msg,MSG_SIZE);

			if(result<=0) {
				fprintf(stderr, "SDLNet_TCP_RECV: %s\n", SDLNet_GetError());
				connected = 0;
				break;
			}

			if(strcmp(msg, "QUIT") == 0)
			{
				quit = 1;
				break;
			}
			else if (strcmp(msg,"NOOP") == 0)
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
				list<string> tmp = Connection::tokenize(msg);
				list<string>::iterator i;
				for(i=tmp.begin(); i != tmp.end(); ++i)
				{
					recv_buffer.push(*i);
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

	Conn_Setup* setup = (Conn_Setup*)s;
	queue<string> recv_buffer = setup->recv_buffer;
	queue<void*> send_buffer = setup->send_buffer;
	TCPsocket sd = setup->tcp;
	SimpleConnections sc = setup->sc;

	SDLNet_SocketSet cnx_set = sc.tcpset;
	TCPsocket sock; // sock descr & client sock descr

	int ready = 0;
	list<Conn> socket_list;

	if( (SDLNet_TCP_AddSocket(cnx_set, sd)) == -1)
	{
		printf("SDLNet_AddSocket: %s\n", SDLNet_GetError());
		// perhaps you need to restart the set and make it bigger...
	}

	while (true)
	{
		printf("Top of loop\n");
		if ( (ready = SDLNet_CheckSockets(cnx_set, TIME_OUT)) == -1)
			break;

 		//printf("%d sockets ready!\n", ready);

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
					// perhaps you need to restart the set and make it bigger...

					SDLNet_TCP_Send(csd, (void*)"QUIT", OP_SIZE);
					printf("%d sockets in set (fail)\n", ready);
					SDLNet_TCP_Close(csd);
				} else {
/*
					SDLNet_TCP_Send(csd, (void*)"password", 10);
					printf("%d sockets in set\n", ready);
					Conn x = {csd, "", "", time(NULL)};
					socket_list.push_back(x);
*/
				}
			}
		}

		// check if any connections have action.
		list<Conn>::iterator i;
		int bufflen = 0;
		char buffer[MAXLEN];
		for(i=socket_list.begin(); i != socket_list.end(); ++i)
		{
			sock = i->data.sock;

			// if not ready, check idleness.
			if(!SDLNet_SocketReady(sock))
			{
				if (time(NULL) - i->last_active > 20)
				{
					SDLNet_TCP_Send(sock, (void*)"QUIT", OP_SIZE);
					SDLNet_TCP_DelSocket(cnx_set,sock);
					SDLNet_TCP_Close(sock);
					socket_list.erase(i);
					printf("kill socket thanks to time\n");
					break;
				}
				else if (time(NULL) - i->last_active > 10)
				{
					SDLNet_TCP_Send(sock, (void*)"NOOP", OP_SIZE);
				}
				continue;
			}
			printf("** a connected socket is ready!\n");
			i->last_active = time(NULL);
			if ( (bufflen = SDLNet_TCP_Recv(sock, buffer, MAXLEN-1)) > 0)
			{
				buffer[bufflen] = 0x00;
				printf("Client said: %s\n", buffer);

				if(strcmp(buffer, "QUIT") == 0)
				// Quit the program
				{
					SDLNet_TCP_Send(sock, (void*)"QUIT", OP_SIZE);
					SDLNet_TCP_DelSocket(cnx_set,sock);
					socket_list.erase(i);
					SDLNet_TCP_Close(sock);
					printf("kill socket\n");
					break;
				} else if (strcmp(i->password, "") == 0 ) {
/*
					char aft=buffer[strlen(PASSWORD)];
					if(aft == '\n' || aft == '\r')
						buffer[strlen(PASSWORD)] = 0;
					if(strcmp(buffer, PASSWORD) == 0) {
						i->password[0] = 'x';
						SDLNet_TCP_Send(sock, (void*)"OKOP", OP_SIZE);
					} else {
						printf("* Incorrect password!\n");
						SDLNet_TCP_Send(sock, (void*)"QUIT", OP_SIZE);
						SDLNet_TCP_DelSocket(cnx_set,sock);
						socket_list.erase(i);
						SDLNet_TCP_Close(sock);	
					} 
*/
				} else
					SDLNet_TCP_Send(sock, (void*)"OKOP", OP_SIZE);
			}
			else
			{
				SDLNet_TCP_DelSocket(cnx_set, sock);
				socket_list.erase(i);
				SDLNet_TCP_Close(sock);
				printf("Terminate connection due to error.\n");
				break;
			}
		}
	}
	printf("SDLNet_CheckSockets: %s\n", SDLNet_GetError());
	//most of the time this is a system error, where perror might help you.
	perror("SDLNet_CheckSockets");


	SDLNet_TCP_Close(sd);
	SDLNet_Quit();
 
	return EXIT_SUCCESS;

	return 0;
}


list<string> SimpleConnect::Connection::tokenize(char* packet)
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
