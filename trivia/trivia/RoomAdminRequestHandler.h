#pragma once
#include "RoomMemberRequestHandler.h"
#include "RequestHandlerFactory.h"

class RequestHandlerFactory;
class RoomAdminRequestHandler : public RoomMemberRequestHandler
{
public:
	RoomAdminRequestHandler(RequestHandlerFactory& factory, LoggedUser user, unsigned int roomID);

	virtual bool isRequestRelevant(RequestInfo);
	virtual RequestResult handleRequest(RequestInfo);
protected:
	virtual RequestResult leaveRoom();
	virtual IRequestHandler* copyHandler();
private:
	RequestResult startGame();
};

