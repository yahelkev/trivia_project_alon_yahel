#pragma once
#include "IDatabase.h"
#include "UserStatistics.h"
#include "Constants.h"

class IDatabase;
class StatisticsManager
{
public:
	StatisticsManager(IDatabase* database);
	// function scores statistics of a user, used to find the highest scores.
	static float scoreFormula(float averageAnswerTime, int correctAnswers, int totalAnswers, int gameCount);
	static float scoreFormula(const UserStatistics& statistics);

	std::vector<std::string> getHighScores();
	std::vector<std::string> getUserStatistics(const std::string& username);
private:
	IDatabase* m_database;
};

