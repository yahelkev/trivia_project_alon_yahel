#pragma once
#include "IDatabase.h"
#include "UserStatistics.h"
#include "Constants.h"

class IDatabase;
class StatisticsManager
{
public:
	static float scoreFormula(float averageAnswerTime, int correctAnswers, int totalAnswers, int gameCount);
	std::vector<std::string> getHighScores();
	std::vector<std::string> getUserStatistics(const std::string& username);
private:
	IDatabase* m_database;
};

