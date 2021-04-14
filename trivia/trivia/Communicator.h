#pragma once

#include <Map>
#include <mutex>
#include <iostream>
#include <WinSock2.h>
#include <thread>
#include "LoginRequestHandler.h"

#define PORT 6666
#define IFACE 0
#define MESSAGE_LENGTH 5

class Communicator
{
	SOCKET m_serverSocket;
	std::mutex m_clientMapMutex;
	std::map<SOCKET, IRequestHandler*> m_clients;
	// function prepares listening socket for accepting clients
	void bindAndListen();
	// function handles conversation with a client
	void handleNewClient(SOCKET);
public:
	~Communicator();
	// function starts communication with clients
	void startHandleRequests();
};

