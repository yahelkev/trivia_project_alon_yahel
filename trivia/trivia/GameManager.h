#pragma once
#include <map>
#include "IDatabase.h"
#include "MessageStructs.h"
#include "Game.h"
#include "Room.h"

class GameManager
{
public:
	GameManager();
	GameManager(IDatabase*);

	void createGame(Room&);
	void deleteGame(const roomID);
	Game& getGame(const roomID);
	void removePlayer(const roomID, const LoggedUser);
private:
	IDatabase* m_database;
	std::map<roomID, Game> m_games;
};

