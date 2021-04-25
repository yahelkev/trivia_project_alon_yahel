#pragma once
#include <io.h>
#include "IDatabase.h"
#include "sqlite3.h"

typedef int(*callbackFunction)(void*, int, char**, char**);

class SqliteDatabase : public IDatabase
{
public:
	SqliteDatabase(std::string dbPath);
	~SqliteDatabase();
	virtual bool doesUserExist(std::string username) { return true; }
	virtual bool doesPasswordMatch(std::string username, std::string password) { return true; }
	virtual bool addNewUser(std::string username, std::string password, std::string email) { return true; }
private:
	// function executes a sql query in the opened database. returns whether the callback was used or not.
	bool executeQuery(const std::string& sql, callbackFunction callback=nullptr, void* callbackData=nullptr);

	sqlite3* _database;
};

