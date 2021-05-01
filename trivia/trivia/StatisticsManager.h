#pragma once
#include "IDatabase.h"
#include "UserStatistics.h"
#include "Constants.h"

class IDatabase;
class StatisticsManager
{
public:
	std::list<UserStatistics> getStatistics();

	static float scoreFormula(float averageAnswerTime, int correctAnswers, int totalAnswers, int gameCount);
private:
	IDatabase* m_database;

	std::list<UserStatistics> getHighScores();
	UserStatistics getUserStatistics(const std::string& username);
};

