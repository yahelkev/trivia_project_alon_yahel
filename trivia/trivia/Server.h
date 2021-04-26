#pragma once
#include "IDatabase.h"
#include "RequestHandlerFactory.h"
#include "Communicator.h"
#include "Constants.h"

class Server
{
	Communicator m_communicator;
	//IDatabase* m_database;
	//RequestHandlerFactory m_handlerFactory;
public:
	void run();
};

