#pragma once

#include <Map>
#include <mutex>
#include <iostream>
#include <WinSock2.h>
#include <thread>
#include "IRequestHandler.h"

class Communicator
{
	SOCKET m_serverSocket;
	std::map<SOCKET, IRequestHandler*> m_clients;
	void bindAndListen();
	void handleNewClient(SOCKET);

public:
	void startHandleRequests();
};

