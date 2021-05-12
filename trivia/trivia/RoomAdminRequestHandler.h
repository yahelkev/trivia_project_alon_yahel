#pragma once
#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"

class RequestHandlerFactory;
class RoomAdminRequestHandler : public IRequestHandler
{
public:
	RoomAdminRequestHandler(RequestHandlerFactory& factory, LoggedUser user, unsigned int roomID);

	virtual bool isRequestRelevant(RequestInfo);
	virtual RequestResult handleRequest(RequestInfo);
protected:
	// shared functionality between admin and member
	virtual RequestResult leaveRoom(RequestInfo);
	virtual void getRoomState(RequestInfo);

	roomID m_roomID;
	LoggedUser m_user;
	RoomManager& m_roomManager;
	RequestHandlerFactory& m_handlerFactory;
private:
	// can only be accessed by admin
	RequestResult startGame(RequestInfo);
};

