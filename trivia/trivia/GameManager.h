#pragma once
#include <map>
#include "IDatabase.h"
#include "MessageStructs.h"
#include "Game.h"
#include "Room.h"

class GameManager
{
public:
	void createGame(const Room&);
	void deleteGame(const roomID);
	void getGame(const roomID);
	void removePlayer(const roomID, const LoggedUser);
private:
	IDatabase* m_database;
	std::map<roomID, Game> m_games;
};

