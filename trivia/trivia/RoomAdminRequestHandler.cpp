#include "RoomAdminRequestHandler.h"

RoomAdminRequestHandler::RoomAdminRequestHandler(RequestHandlerFactory& factory, LoggedUser user, unsigned int roomID) : RoomMemberRequestHandler(factory, user, roomID)
{
}

bool RoomAdminRequestHandler::isRequestRelevant(RequestInfo request)
{
	return request.id == LEAVE_ROOM || request.id == START_GAME || request.id == GET_ROOM_STATE;
}

RequestResult RoomAdminRequestHandler::handleRequest(RequestInfo requestInfo)
{
	// admin specific actions
	if (requestInfo.id == START_GAME)
		return this->startGame();
	// member actions
	return RoomMemberRequestHandler::handleRequest(requestInfo);
}

RequestResult RoomAdminRequestHandler::leaveRoom()
{
	// close room
	this->m_roomManager.deleteRoom(this->m_roomID);
	// send response
	Buffer responseBuffer = JsonResponsePacketSerializer::serializeResponse(LeaveRoomResponse{ 1 });
	return RequestResult{ responseBuffer, this->m_handlerFactory.createMenuRequestHandler(this->m_user) };	// back to menu
}

IRequestHandler* RoomAdminRequestHandler::copyHandler()
{
	return this->m_handlerFactory.createRoomAdminRequestHandler(this->m_user, this->m_roomID);
}

RequestResult RoomAdminRequestHandler::startGame()
{
	// change state to active
	this->m_roomManager.getRoom(this->m_roomID).setActive();
	// return response
	Buffer responseBuffer = JsonResponsePacketSerializer::serializeResponse(StartGameResponse{ 1 });
	return RequestResult{ responseBuffer, this->m_handlerFactory.createRoomAdminRequestHandler(this->m_user, this->m_roomID) };	// will be game handler in the next version
}
