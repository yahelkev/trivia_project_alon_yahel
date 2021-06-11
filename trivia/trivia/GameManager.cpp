#include "GameManager.h"

GameManager::GameManager(IDatabase* database) : m_database(database)
{
}

void GameManager::createGame(Room& room)
{
	// generate questions and create game
	std::list<Question> questions = this->m_database->getQuestions(room.getMetaData().numOfQuestionsInGame);
	this->m_games[room.getMetaData().id] = Game(room, questions);
}

void GameManager::deleteGame(const roomID room)
{
	this->m_games.erase(room);
}

Game& GameManager::getGame(const roomID room)
{
	return this->m_games[room];
}

void GameManager::removePlayer(const roomID room, const LoggedUser user)
{
	Game& game = this->getGame(room);
	// update database
	this->m_database->updateUserStatistics(user.getUsername(), game.getGameData(user));
	// remove player
	game.removePlayer(user);
	// check if all players were removed
	if (!game.playersLeft())
	{
		this->deleteGame(room);
	}
}
