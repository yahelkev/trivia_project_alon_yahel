#pragma comment (lib, "ws2_32.lib")

#include "WSAInitializer.h"
#include "Server.h"
#include <iostream>
#include <fstream>
#include "Constants.h"
#include "JsonRequestPacketDeserializer.h"

int main()
{
	const char* cstr = "10003{ \"password\": \"12331\", \"username\": \"lolyy\" }";
	std::cout << cstr << std::endl;
	std::vector<Byte> vec(cstr, cstr + strlen(cstr));
	LoginRequest p = JsonRequestPacketDeserializer::deserializeLoginRequest(vec);
	p.password;
	p.username;
	try
	{
		std::cout << "Starting..." << std::endl;
		WSAInitializer wsa_init;
		Server myServer;
		myServer.run();
	}
	catch (const std::exception& e)
	{
		std::cout << "Exception was thrown in function: " << e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Unknown exception in main !" << std::endl;
	}
}
