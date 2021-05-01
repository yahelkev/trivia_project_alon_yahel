#pragma once
#include <io.h>
#include <mutex>
#include <fstream>
#include "IDatabase.h"
#include "sqlite3.h"
#include "Constants.h"
#include "json.hpp"

using json = nlohmann::json;

typedef int(*callbackFunction)(void*, int, char**, char**);

class SqliteDatabase : public IDatabase
{
public:
	SqliteDatabase(std::string dbPath);
	~SqliteDatabase();
	virtual bool doesUserExist(std::string username);
	virtual bool doesPasswordMatch(std::string username, std::string password);
	virtual bool addNewUser(std::string username, std::string password, std::string email);
	virtual std::list<Question> getQuestions(int questionCount);

	virtual float getAverageAnswerTime(const std::string& username) { return 0; }
	virtual int getNumOfCorrectAnswers(const std::string& username) { return 0; }
	virtual int getNumOfTotalAnswers(const std::string& username) { return 0; }
	virtual int getNumOfPlayerGames(const std::string& username) { return 0; }
	virtual UserStatistics getUserStatistics(const std::string& username) { return { 0 }; }
	virtual std::list<UserStatistics> getHighScores() { return std::list<UserStatistics>(); }
private:
	// function executes a sql query in the opened database. throws exception on error.
	bool executeQuery(const std::string& sql, callbackFunction callback = nullptr, void* callbackData = nullptr);
	// function gets one value from the database as a string.
	std::string valueQuery(const std::string& sql);
	// callback adds question to a list passed in data
	static int pushQuestion(void* data, int argc, char** argv, char** cols);
	// function inserts questions to database from a json file specified in Constants.h
	void insertQuestions();

	std::mutex _databaseMutex;
	sqlite3* _database;
};

