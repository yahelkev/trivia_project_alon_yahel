#pragma once
#include <io.h>
#include <mutex>
#include "IDatabase.h"
#include "sqlite3.h"

typedef int(*callbackFunction)(void*, int, char**, char**);

class SqliteDatabase : public IDatabase
{
public:
	SqliteDatabase(std::string dbPath);
	~SqliteDatabase();
	virtual bool doesUserExist(std::string username);
	virtual bool doesPasswordMatch(std::string username, std::string password);
	virtual bool addNewUser(std::string username, std::string password, std::string email);
private:
	// function executes a sql query in the opened database. throws exception on error.
	void executeQuery(const std::string& sql, callbackFunction callback = nullptr, void* callbackData = nullptr);
	// function gets one value from the database as a string.
	std::string valueQuery(const std::string& sql);

	std::mutex _databaseMutex;
	sqlite3* _database;
};

