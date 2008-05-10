#ifndef SC_CONN_H
#define SC_CONN_H

#include <string>
#include <vector>
#include <list>
#include <map>
#include "../simpleutils/safecomm.h"

#include "SDL_thread.h"
#include "SDL_net.h"
using namespace std;

enum SN_Status {
  SN_CONN_NONE = 0,
  SN_CONN_OK = 1,
  SN_CONN_RECON = 2
};

class SimpleNetwork {
    public:
	static SimpleNetwork *Current() { return current; };
	
	struct Data {
		string playername;
		string password;
		TCPsocket tcp;
		SDL_mutex* recv_mutex;
		vector<Uint8> recv_buffer; // contains buffered data recieved over TCP
		vector<Uint8> send_buffer; // contains buffered sending data.
		Uint32 last_active;
		Uint8 conn_status;
	};

	SimpleNetwork(Uint16 port = 4052);
	~SimpleNetwork(); //cleanup

	// add to send_buffer
	void Add(int, const Uint8&);
        void Add(int, const Uint16&);
	void Add(int, const Uint32&);
	void Add(int, const string&);

	// sends send_buffer over tcp and clears it.
	void Send(int); // should this return int for some reason?

	// returns conection id, adds new connection
	// also for reconnection, with name and password.
	int Connect(IPaddress&, const string& name="", const string& password="");

	void StopAccepting();

	// internals: start this up when a connection drops, takes max amount to accept.
	// when we recieve this amount, stop accepting
	void StartAccepting(int);

	// returns number of connections to the server.
	int NumConnections();

	void LockConnections();
	void UnlockConnections();

	// disconnects an individual client from the server given its slot.
	void Disconnect(int);

	// these recieve data from the recv_buffer and take it off the queue.
	void Recv(int, Uint8&);
	void Recv(int, Uint16&);
	void Recv(int, Uint32&);
        void Recv(int, string&);

	// gets the size of the recv_buffer.
	int RecvBufferSize(int);

	// sets port to given value
	void SetPort(Uint16);

	// set the playername of the given id.
	void SetName(int, const string&);

	// set the password of the given id.
	void SetPassword(int, const string&);

	// gets the playername of the given id.
	string GetName(int);

	// returns the connection status of the player in given slot.
	SN_Status IsConnected(int);
    private:
	Uint8 curr_slot;
	Uint16 port;
	map<Uint16, Data> data;
	SDLNet_SocketSet cnx_set;
	int accept_amount; // number of accepted sockets left for StartAccepting()
	TCPsocket sd; // server tcp socket.
	bool isserver;
	bool connections_locked;
	static int RunRecv(void*); // client thread.
	bool recv_running;
	static int RunAccept(void*); // runs the accept routine.
	bool accept_running;
	static int FindNull(const char*, int, int);
    protected:
	static SimpleNetwork *current;

	SDL_Thread* accept_thread;
	SDL_Thread* recv_thread;
	SDL_mutex * accept_mutex;
	SDL_mutex * data_mutex;
};

#endif // SC_CONN_H

