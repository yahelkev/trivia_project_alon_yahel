#pragma once
#include "IDatabase.h"
#include "SqliteDatabase.h"
#include "RequestHandlerFactory.h"
#include "Communicator.h"
#include "Constants.h"

class Server
{
	Communicator m_communicator;
	IDatabase* m_database;
	RequestHandlerFactory m_handlerFactory;
public:
	void run();
	Server() : m_database(new SqliteDatabase(DB_PATH)), m_handlerFactory(RequestHandlerFactory(m_database)), m_communicator(m_handlerFactory) {};
	~Server() { delete m_database; };
};

