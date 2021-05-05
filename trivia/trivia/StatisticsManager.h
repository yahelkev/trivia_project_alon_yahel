#pragma once
#include <vector>
#include <string>
#include "IDatabase.h"

class StatisticsManager
{
public:
	StatisticsManager(IDatabase* database) : m_database(database) {}

	std::vector<std::string> getUserStatistics(std::string username) { return std::vector<std::string>(); }
	std::vector<std::string> getHighScores() { return std::vector<std::string>(); }
private:
	IDatabase* m_database;
};

