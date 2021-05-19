#include "StatisticsManager.h"

StatisticsManager::StatisticsManager(IDatabase* database) : m_database(database) 
{
}

std::vector<std::string> StatisticsManager::getUserStatistics(std::string username)
{
	return std::vector<std::string>();
}

std::vector<std::string> StatisticsManager::getHighScores()
{
	return std::vector<std::string>();
}
