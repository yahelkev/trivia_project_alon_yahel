#pragma once
#include "RoomAdminRequestHandler.h"

class RequestHandlerFactory;
class RoomMemberRequestHandler : public RoomAdminRequestHandler
{
public:
	RoomMemberRequestHandler(RequestHandlerFactory& factory, LoggedUser user, unsigned int roomID);

	// restrict admin only requests
	virtual bool isRequestRelevant(RequestInfo);
protected:
	// change leave room functionality
	virtual RequestResult leaveRoom(RequestInfo);
	virtual IRequestHandler* copyHandler();
};

