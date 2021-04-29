#pragma comment (lib, "ws2_32.lib")

#include "WSAInitializer.h"
#include "Server.h"
#include <iostream>
#include <fstream>

#include "RoomManager.h"

int checkLeak()
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
	return 0;
}
int main()
{
	RoomManager l;
	l.creatRoom(LoggedUser("lool"), RoomData{});
	l.creatRoom(LoggedUser("lool"), RoomData{});
	l.deleteRoom(0);
	l.deleteRoom(0);
	//checkLeak();
	if (_CrtDumpMemoryLeaks())
	{
		std::cout << "Memory leaks!\n";
	}
	else
	{
		std::cout << "No leaks\n";
	}
}
