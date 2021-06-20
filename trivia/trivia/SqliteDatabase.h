#pragma once
#include <io.h>
#include <mutex>
#include <fstream>
#include "IDatabase.h"
#include "sqlite3.h"
#include "Constants.h"
#include "json.hpp"
#include "StatisticsManager.h"

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

	virtual float getAverageAnswerTime(const std::string& username);
	virtual int getNumOfCorrectAnswers(const std::string& username);
	virtual int getNumOfTotalAnswers(const std::string& username);
	virtual int getNumOfPlayerGames(const std::string& username);
	virtual UserStatistics getUserStatistics(const std::string& username);
	virtual void updateUserStatistics(const std::string& username, GameData results);
	virtual std::list<UserStatistics> getHighScores();
	virtual bool changePassword(const std::string& username, const std::string& newPassword);

private:
	// function executes a sql query in the opened database. throws exception on error.
	bool executeQuery(const std::string& sql, callbackFunction callback = nullptr, void* callbackData = nullptr);
	// function gets one value from the database as a string.
	std::string valueQuery(const std::string& sql);
	// callback adds Object to a list passed in data. Object should have a constructor Object(int argc, char** argv, char** cols)
	template <class Object>
	static int pushCallback(void* data, int argc, char** argv, char** cols);
	// callback creates one object. Object should have a constructor Object(int argc, char** argv, char** cols) and an assignment operator.
	template <class Object>
	static int createObjectCallback(void* data, int argc, char** argv, char** cols);
	// sqlite user defined function to compare high scores
	static void scoreFunction(sqlite3_context*, int, sqlite3_value**);

	std::mutex _databaseMutex;
	sqlite3* _database;
};

template <class Object>
int SqliteDatabase::pushCallback(void* data, int argc, char** argv, char** cols)
{
	std::list<Object>& questionList = *(std::list<Object>*)data;
	questionList.push_back(Object(argc, argv, cols));
	return 0;
}

template<class Object>
inline int SqliteDatabase::createObjectCallback(void* data, int argc, char** argv, char** cols)
{
	Object& object = *(Object*)data;
	object = Object(argc, argv, cols);
	return 0;
}
