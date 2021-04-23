#pragma once
#include "LoginManager.h"
#include "IDatabase.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"

class LoginRequestHandler;
class RequestHandlerFactory
{
public:
	RequestHandlerFactory() = default;
	RequestHandlerFactory(IDatabase*);
	LoginRequestHandler* createLoginRequestHandler();
	MenuRequestHandler* createMenuRequestHandler();
	LoginManager& getLoginManager();
private:
	IDatabase* m_database;
	LoginManager m_loginManager;
};

