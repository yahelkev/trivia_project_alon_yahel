#pragma once

#include <algorithm>
#include <vector>
#include <mutex>
#include "LoggedUser.h"
#include "IDatabase.h"

class LoginManager
{
public:
	LoginManager(IDatabase* dataBase) : m_database(dataBase) {};
	bool signup(const std::string& username, const std::string& password, const std::string& email);
	bool login(const std::string& username, const std::string& password);
	bool logout(const std::string& username);

private:
	IDatabase* m_database;
	std::vector<LoggedUser> m_loggedUsers;
	std::mutex m_lggedUsersMutex;
	std::mutex m_database_mutex;

};

