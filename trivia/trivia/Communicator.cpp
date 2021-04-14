#include "Communicator.h"

void Communicator::startHandleRequests()
{
	// create listening socket for accepting clients
	this->m_serverSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->m_serverSocket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__ " - socket creation");
	this->bindAndListen();

	// accept clients
	while (true)
	{
		SOCKET clientSocket = accept(this->m_serverSocket, NULL, NULL);
		if (clientSocket == INVALID_SOCKET)
			throw std::exception(__FUNCTION__ " - accept client");
		// handle client conversation
		std::thread(&Communicator::handleNewClient, this, clientSocket).detach();
	}
}

void Communicator::bindAndListen()
{
	struct sockaddr_in sa = { 0 };
	sa.sin_port = htons(PORT);
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = IFACE;

	if (::bind(this->m_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - bind");
	if (::listen(this->m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - listen");
}

void Communicator::handleNewClient(SOCKET clientSocket)
{
	int socketResult = 0;
	// add client with login state to map
	std::unique_lock<std::mutex> clientMapLock(this->m_clientMapMutex);
	this->m_clients[clientSocket] = new LoginRequestHandler;
	clientMapLock.unlock();

	// send hello message
	const char* msg = "hello";
	socketResult = send(clientSocket, msg, strlen(msg), 0);
	if (socketResult == INVALID_SOCKET)
		throw std::exception(__FUNCTION__ " - send msg");

	// receive hello message
	char receivedMessage[MESSAGE_LENGTH + 1] = { 0 };
	socketResult = recv(clientSocket, receivedMessage, MESSAGE_LENGTH, 0);
	if (socketResult == INVALID_SOCKET)
		throw std::exception(__FUNCTION__ "- receive msg");

	// print result and close connection
	std::cout << "received message: " << receivedMessage << std::endl;
	closesocket(clientSocket);
	// delete from map
	clientMapLock.lock();
	delete this->m_clients[clientSocket];
	this->m_clients.erase(clientSocket);

}

Communicator::~Communicator()
{
	try
	{
		// delete clients left in map
		for (auto const& client : this->m_clients)
		{
			closesocket(client.first);
			delete client.second;
		}
		// close listening socket
		closesocket(this->m_serverSocket);
	}
	catch (...) {}
}
