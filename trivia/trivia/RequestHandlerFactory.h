#pragma once
#include "LoginManager.h"
#include "StatisticsManager.h"
#include "RoomManager.h"
#include "IDatabase.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"
#include "GameRequestHandler.h"
#include "QuestionManager.h"

class GameRequestHandler;
class LoginRequestHandler;
class MenuRequestHandler;
class RoomAdminRequestHandler;
class RoomMemberRequestHandler;
class RequestHandlerFactory
{
public:
	RequestHandlerFactory(IDatabase* database) :m_database(database), m_loginManager(m_database), m_statisticsManager(m_database), m_gameManager(m_database), m_questionManager(m_database) {}
	LoginRequestHandler* createLoginRequestHandler();
	MenuRequestHandler* createMenuRequestHandler(LoggedUser user);
	RoomAdminRequestHandler* createRoomAdminRequestHandler(LoggedUser user, roomID room);
	RoomMemberRequestHandler* createRoomMemberRequestHandler(LoggedUser user, roomID room);
	LoginManager& getLoginManager();
	StatisticsManager& getStatisticsManager();
	RoomManager& getRoomManager();
	QuestionManager& getQuestionManager();

	GameRequestHandler* createGameRequestHandler(roomID roomID, LoggedUser user);
	GameManager& getGameManager();
private:
	IDatabase* m_database;
	LoginManager m_loginManager;
	StatisticsManager m_statisticsManager;
	RoomManager m_roomManager;
	GameManager m_gameManager;
	QuestionManager m_questionManager;
};
