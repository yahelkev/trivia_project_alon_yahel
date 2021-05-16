#include "RoomMemberRequestHandler.h"
#include "RequestHandlerFactory.h"

RoomMemberRequestHandler::RoomMemberRequestHandler(RequestHandlerFactory& factory, LoggedUser user, unsigned int roomID) : RoomAdminRequestHandler(factory, user, roomID)
{
}

bool RoomMemberRequestHandler::isRequestRelevant(RequestInfo request)
{
	return request.id == LEAVE_ROOM || request.id == GET_ROOM_STATE;
}

RequestResult RoomMemberRequestHandler::leaveRoom(RequestInfo)
{
	// exit room if it still exists
	if(this->m_roomManager.doesRoomExist(this->m_roomID))
		this->m_roomManager.getRoom(this->m_roomID).removeUser(this->m_user);
	// send response
	Buffer responseBuffer = JsonResponsePacketSerializer::serializeResponse(LeaveRoomResponse{ 1 });
	return RequestResult{ responseBuffer, this->m_handlerFactory.createMenuRequestHandler(this->m_user) };	// back to menu
}

IRequestHandler* RoomMemberRequestHandler::copyHandler()
{
	return this->m_handlerFactory.createRoomMemberRequestHandler(this->m_user, this->m_roomID);
}
