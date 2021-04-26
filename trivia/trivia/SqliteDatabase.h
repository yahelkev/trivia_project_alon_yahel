#pragma once
#include "IDatabase.h"
#include "sqlite3.h"

class SqliteDatabase : public IDatabase
{
public:
	SqliteDatabase(std::string dbPath) {};
	SqliteDatabase() {};
	~SqliteDatabase() {};
	virtual bool doesUserExist(std::string username) { return true; }
	virtual bool doesPasswordMatch(std::string username, std::string password) { return true; }
	virtual bool addNewUser(std::string username, std::string password, std::string email) { return true; }
private:
	sqlite3* _database;
};

