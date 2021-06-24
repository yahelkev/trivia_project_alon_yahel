#pragma comment (lib, "ws2_32.lib")

#include "WSAInitializer.h"
#include "Server.h"
#include <iostream>
#include <fstream>
#include <string>


int main()
{
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
