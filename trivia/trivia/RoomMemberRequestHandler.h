#pragma once
#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"

class RequestHandlerFactory;
class RoomMemberRequestHandler : public IRequestHandler
{
public:
	RoomMemberRequestHandler(RequestHandlerFactory& factory, LoggedUser user, unsigned int roomID);

	virtual bool isRequestRelevant(RequestInfo);
	virtual RequestResult handleRequest(RequestInfo);

	virtual void abortSignout();
protected:
	virtual RequestResult leaveRoom();
	virtual RequestResult getRoomState();
	virtual IRequestHandler* copyHandler();

	RequestHandlerFactory& m_handlerFactory;
	RoomManager& m_roomManager;
	LoggedUser m_user;
	roomID m_roomID;
};

