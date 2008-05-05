#include <set>

#include "../sc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include "SDL.h"
#include "SDL_net.h"
#include "SDL_thread.h"

#define OP_SIZE 5
#define MSG_SIZE 1024
#define RETRY_COUNT 30000
#define MAXSOCKETS 128
#define MAXLEN 1024
using namespace std;

char* PASSWORD = "HELLO!";

struct Conn {
	TCPsocket sock;
	char password[256];
	char uname[256];
	int last_active;
};

int main(int argc, char **argv)
{
	SDLNet_SocketSet cnx_set;
	//TCPsocket sock; // sock descr & client sock descr
	TCPsocket sd; // sock descr & client sock descr
	IPaddress ip;
	IPaddress* remoteIP;

	int ready = 0;
	list<Conn> socket_list;

	// allocate our socket set.
	if( (cnx_set = SDLNet_AllocSocketSet(MAXSOCKETS)) == NULL) {
		printf("SDLNet_AllocSocketSet: %s\n", SDLNet_GetError());
		exit(1); //most of the time this is a major error, but do what you want.
	}

	if (SDLNet_Init() < 0)
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
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

	if( (SDLNet_TCP_AddSocket(cnx_set, sd)) == -1)
	{
		printf("SDLNet_AddSocket: %s\n", SDLNet_GetError());
		// perhaps you need to restart the set and make it bigger...
	}

	TCPsocket csd;

	while (ready < 3)
	{
		printf("Top of loop: tjhing\n");
		if ( (ready = SDLNet_CheckSockets(cnx_set, 5000)) == -1)
			break;

 		printf("%d sockets ready!\n", ready);

		// server gets a new connection?
		if ( (SDLNet_SocketReady(sd)) )
		{
			
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
				} 
				else {
					SDLNet_TCP_Send(csd, (void*)"password", 9);
					//SDLNet_TCP_Send(csd, (void*)"OKOP", OP_SIZE);
					printf("%d sockets in set\n", ready);
					Conn x = {csd, "", "", time(NULL)};
					socket_list.push_back(x);
					fprintf(stderr, "Yay, socket added to socket_list!\n");

					if ((remoteIP = SDLNet_TCP_GetPeerAddress(csd)))
					{
						// Print the address, converting in the host format
						printf("Host connected: %x %d\n",
						          SDLNet_Read32(&remoteIP->host), SDLNet_Read16(&remoteIP->port));
					}
					else
						fprintf(stderr, "SDLNet_TCP_GetPeerAddress: %s\n", SDLNet_GetError());
				}
			}
		}

		char msg[256];
		list<Conn>::iterator i = socket_list.begin();
		for (; i != socket_list.end(); ++i)
		{
			if (SDLNet_SocketReady(i->sock))
			{
				fprintf(stderr, "send OKOP\n");
				SDLNet_TCP_Recv(i->sock, msg, 256);
				SDLNet_TCP_Send(i->sock, (void*)"OKOP", 5);
			}
		}	
	}

	fprintf(stderr, "Step 1\n");
	SDLNet_TCP_DelSocket(cnx_set, sd);

	vector<SlotData> slotsa;

	Sint8 usr1 [16] = {66,85,76,76,79,88 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	Sint8 pas1 [16] = {72, 69, 76, 76, 79, 33, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	Sint8 usr2 [16] = {72, 69, 76, 76, 79, 33, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	Sint8 pas2 [16] = {66,85,76,76,79,88 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	list<Conn>::iterator i = socket_list.begin();
	for (; i != socket_list.end(); ++i)
	{
		SlotData sl;
		sl.sock = i->sock;
		if (i == socket_list.begin())
		{
			memcpy(&sl.password, &usr1, 16);
			memcpy(&sl.playername, &pas1, 16);
		}
		else
		{
			memcpy(&sl.password, &usr2, 16);
			memcpy(&sl.playername, &pas2, 16);
		}

 		if ((remoteIP = SDLNet_TCP_GetPeerAddress(sl.sock)))
 		{
 			// Print the address, converting in the host format
 			printf("Again Host connected: %x %d\n",
 					SDLNet_Read32(&remoteIP->host), SDLNet_Read16(&remoteIP->port));
 		}
 		else {
 			fprintf(stderr, "Again SDLNet_TCP_GetPeerAddress: %s\n", SDLNet_GetError());
 		}
		slotsa.push_back( sl );
	}

	SimpleConnections connections;
	connections.slots = slotsa;
	connections.tcpset = cnx_set;
	connections.sock = sd;
	//SimpleConnect::Connection * conn =
	new SimpleConnect::Connection(sd, connections);

	fprintf(stderr, "Step 2\n");

	char written_msg[25];
	scanf("%s", written_msg);

	fprintf(stderr, "Step 3dd\n");

	SDLNet_TCP_Close(sd);
	SDLNet_Quit();
 
	return EXIT_SUCCESS;
}
