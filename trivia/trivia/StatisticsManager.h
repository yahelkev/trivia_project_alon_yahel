#pragma once
#include <vector>
#include <string>
#include "IDatabase.h"

class StatisticsManager
{
public:
	StatisticsManager(IDatabase* database);

	std::vector<std::string> getUserStatistics(std::string username);
	std::vector<std::string> getHighScores();
private:
	IDatabase* m_database;
};

