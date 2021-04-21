#include "Communicator.h"
#include "Constants.h"

void Communicator::startHandleRequests()
{
	// create listening socket for accepting clients
	this->m_serverSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->m_serverSocket == INVALID_SOCKET)
	{
		std::cout << "Server stopped working. ERROR_CODE on socket creation." << std::endl;
		exit(1);
	}
	this->bindAndListen();

	// accept clients
	while (true)
	{
		SOCKET clientSocket = accept(this->m_serverSocket, NULL, NULL);
		if (clientSocket == INVALID_SOCKET)
		{
			std::cout << "Server stopped working. ERROR_CODE on client accept." << std::endl;
			exit(1);
		}
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
	this->addClient(clientSocket, new LoginRequestHandler);

	while (true)
	{
		RequestInfo requestInfo;
		// read request from socket
		try
		{
			requestInfo = this->getRequest(clientSocket);
		}
		catch (...)
		{
			std::cout << "Client " << clientSocket << " was discconected!" << std::endl;
			this->deleteClient(clientSocket);
			return;
		}

		// process request
		IRequestHandler* requestHandler = this->getRequestHandler(clientSocket);
		RequestResult requestResult = requestHandler->handleRequest(requestInfo);
		// set new handler
		this->setRequestHandler(clientSocket, requestResult.newHandler);

		// send response
		int socketResult = send(clientSocket, (char*)requestResult.response.data(), requestResult.response.size(), 0);
		if (socketResult == INVALID_SOCKET)
		{
			this->deleteClient(clientSocket);
			return;
		}
	}
}

void Communicator::addClient(SOCKET clientSocket, IRequestHandler* requestHandler)
{
	std::lock_guard<std::mutex> clientMapLock(this->m_clientMapMutex);
	this->m_clients[clientSocket] = requestHandler;
}

void Communicator::deleteClient(SOCKET clientSocket)
{
	std::lock_guard<std::mutex> clientMapLock(this->m_clientMapMutex);
	delete this->m_clients[clientSocket];
	this->m_clients.erase(clientSocket);
	closesocket(clientSocket);
}

RequestInfo Communicator::getRequest(SOCKET clientSocket)
{
	RequestInfo requestInfo = { 0 };
	// read request code
	int socketResult = recv(clientSocket, (char*)&requestInfo.id, REQUEST_CODE_BYTES, 0);
	if (socketResult == INVALID_SOCKET)
	{
		throw std::exception();
	}
	// read content length
	int contentLength = 0;
	socketResult = recv(clientSocket, (char*)&contentLength, CONTENT_LENGTH_BYTES, 0);
	if (socketResult == INVALID_SOCKET)
	{
		throw std::exception();
	}
	// read content
	char* content = new char[contentLength];
	socketResult = recv(clientSocket, (char*)&contentLength, contentLength, 0);
	if (socketResult == INVALID_SOCKET)
	{
		throw std::exception();
	}
	// set content in buffer
	requestInfo.jsonBuffer.insert(requestInfo.jsonBuffer.end(), content, content + contentLength);
	// set time
	requestInfo.receivalTime = time(nullptr);
	delete [] content;
	return requestInfo;
}

IRequestHandler* Communicator::getRequestHandler(SOCKET clientSocket)
{
	std::lock_guard<std::mutex> lock(this->m_clientMapMutex);
	return this->m_clients[clientSocket];
}

void Communicator::setRequestHandler(SOCKET clientSocket, IRequestHandler* newHandler)
{
	std::lock_guard<std::mutex> lock(this->m_clientMapMutex);
	// delete previous handler and set new one
	delete this->m_clients[clientSocket];
	this->m_clients[clientSocket] = newHandler;
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
