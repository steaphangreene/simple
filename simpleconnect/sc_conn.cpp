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

#define MAXSOCKETS 17
#define MAXLEN 1024
#define MSG_SIZE 1024


// constructer for creating a client connection.
SimpleConnect::Connection::Connection(TCPsocket sock)
{
 	socket = sock;
	networking_thread = SDL_CreateThread(RunClient, (void*)sock);
}

// constructer for creating a server connection.
SimpleConnect::Connection::Connection(TCPsocket sock, SimpleConnections sconn)
{
	socket = sock;
	sc = sconn;
	networking_thread = SDL_CreateThread(RunServer, (void*)sock);
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

int SimpleConnect::Connection::RunClient(void*conn)
{
	// runs the client thread which recieves information from the server if they send something
        // and adds the recieved values to a queue which can be taken from using recv().

	int quit, connected, failed;
	TCPsocket socket = (TCPsocket)conn;
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

			if (strcmp(msg,"noop") != 0) {
				printf("Received: %s\n",msg);
			}
			else {
				if ( SDLNet_TCP_Send(socket, (void*)"Ok", 3) < 3)
				{
					fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
					connected = 0;
					break;
				}
			}

			if(strcmp(msg, "quit") == 0)
			{
				quit = 1;
				break;
			}

			if(strcmp(msg, "Ok") == 0)
			{
				continue;
			}
		}
	}
	
	SDLNet_TCP_Close(socket);
	return EXIT_SUCCESS;
}


int SimpleConnect::Connection::RunServer(void*conn)
{
	// runs the server thread which recieves information from a SocketSet of clients. This will
	// add their messages to a queue of messages which are delimited by socket number.
	return 0;
}

