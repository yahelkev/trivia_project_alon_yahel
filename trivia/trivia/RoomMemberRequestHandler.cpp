#include "RoomMemberRequestHandler.h"

RoomMemberRequestHandler::RoomMemberRequestHandler(RequestHandlerFactory& factory, LoggedUser user, unsigned int roomID) : m_handlerFactory(factory), m_roomManager(factory.getRoomManager()), m_user(user), m_roomID(roomID)
{
}

bool RoomMemberRequestHandler::isRequestRelevant(RequestInfo request)
{
	return request.id == LEAVE_ROOM || request.id == GET_ROOM_STATE;
}

RequestResult RoomMemberRequestHandler::handleRequest(RequestInfo requestInfo)
{
	switch (requestInfo.id)
	{
	case LEAVE_ROOM:
		return this->leaveRoom();
	case GET_ROOM_STATE:
		return this->getRoomState();
	default:
	{
		Buffer buffer = JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ "Invalid request code for your state!" });
		return RequestResult{ buffer, this->copyHandler() };
	}
	}
}

void RoomMemberRequestHandler::abortSignout()
{
	this->leaveRoom();
	this->m_handlerFactory.getLoginManager().logout(this->m_user.getUsername());
}

RequestResult RoomMemberRequestHandler::leaveRoom()
{
	// exit room if it still exists
	if(this->m_roomManager.doesRoomExist(this->m_roomID))
		this->m_roomManager.getRoom(this->m_roomID).removeUser(this->m_user);
	// send response
	Buffer responseBuffer = JsonResponsePacketSerializer::serializeResponse(LeaveRoomResponse{ 1 });
	return RequestResult{ responseBuffer, this->m_handlerFactory.createMenuRequestHandler(this->m_user) };	// back to menu
}

RequestResult RoomMemberRequestHandler::getRoomState()
{
	if (!this->m_roomManager.doesRoomExist(this->m_roomID))
	{	// room is closed
		return this->leaveRoom();
	}
	if (this->m_roomManager.getRoomState(this->m_roomID) == 1)
	{	// game started
		Buffer responseBuffer = JsonResponsePacketSerializer::serializeResponse(StartGameResponse{ 1 });
		m_roomManager.getRoom(this->m_roomID).removeUser(this->m_user);
		if (m_roomManager.getRoom(this->m_roomID).getAllUsers().size() == 0)
			m_roomManager.deleteRoom(this->m_roomID);
		return RequestResult{ responseBuffer, this->copyHandler() };	// will be game handler in the next version
	}
	// return room data and player list
	RoomData roomData = this->m_roomManager.getRoom(this->m_roomID).getMetaData();
	std::vector<std::string> players = this->m_roomManager.getRoom(this->m_roomID).getAllUsers();
	Buffer responseBuffer = JsonResponsePacketSerializer::serializeResponse(GetRoomStateResponse{ 1, roomData.isActive == 1, players, roomData.numOfQuestionsInGame, roomData.timePerQuestion });
	return RequestResult{ responseBuffer, this->copyHandler() };
}

IRequestHandler* RoomMemberRequestHandler::copyHandler()
{
	return this->m_handlerFactory.createRoomMemberRequestHandler(this->m_user, this->m_roomID);
}
