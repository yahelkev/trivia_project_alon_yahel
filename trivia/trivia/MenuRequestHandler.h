#pragma once
#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "RoomManager.h"

class RequestHandlerFactory;
class MenuRequestHandler : public IRequestHandler
{
public:
	MenuRequestHandler(RequestHandlerFactory& factory, LoggedUser user);

	virtual bool isRequestRelevant(RequestInfo);
	virtual RequestResult handleRequest(RequestInfo);

	virtual void abortSignout();
private:
	LoggedUser m_user;
	RoomManager& m_roomManager;
	StatisticsManager& m_statisticsManager;
	RequestHandlerFactory& m_handlerFactory;

	RequestResult signout();
	RequestResult getRooms();
	RequestResult getPlayersInRoom(RequestInfo);
	RequestResult getPersonalStatistics();
	RequestResult getHighScores();
	RequestResult joinRoom(RequestInfo);
	RequestResult createRoom(RequestInfo);
	RequestResult changePassword(RequestInfo);
	RequestResult resetPassword(RequestInfo);
};

