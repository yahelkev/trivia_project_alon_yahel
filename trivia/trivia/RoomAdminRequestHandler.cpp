#include "RoomAdminRequestHandler.h"

RoomAdminRequestHandler::RoomAdminRequestHandler(RequestHandlerFactory& factory, LoggedUser user, unsigned int roomID) : m_handlerFactory(factory), m_roomManager(factory.getRoomManager()), m_user(user), m_roomID(roomID)
{
}

bool RoomAdminRequestHandler::isRequestRelevant(RequestInfo request)
{
	return request.id == LEAVE_ROOM || request.id == START_GAME || request.id == GET_ROOM_STATE;
}

RequestResult RoomAdminRequestHandler::handleRequest(RequestInfo requestInfo)
{
	switch (requestInfo.id)
	{
	case LEAVE_ROOM:
		return this->leaveRoom(requestInfo);
	case START_GAME:
		return this->startGame(requestInfo);
	case GET_ROOM_STATE:
		return this->getRoomState(requestInfo);
	default:
	{
		Buffer buffer = JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ "Invalid request code for your state!" });
		return RequestResult{ buffer, this->copyHandler() };
	}
	}
}

RequestResult RoomAdminRequestHandler::leaveRoom(RequestInfo)
{
	// close room
	this->m_roomManager.deleteRoom(this->m_roomID);
	// send response
	Buffer responseBuffer = JsonResponsePacketSerializer::serializeResponse(LeaveRoomResponse{ 1 });
	return RequestResult{ responseBuffer, this->m_handlerFactory.createMenuRequestHandler(this->m_user) };	// back to menu
}

RequestResult RoomAdminRequestHandler::getRoomState(RequestInfo)
{
	if (!this->m_roomManager.doesRoomExist(this->m_roomID))
	{	// room is closed
		return this->leaveRoom({});
	}
	if (this->m_roomManager.getRoomState(this->m_roomID) == 1)
	{	// game started
		Buffer responseBuffer = JsonResponsePacketSerializer::serializeResponse(StartGameResponse{ 1 });
		return RequestResult{ responseBuffer, this->m_handlerFactory.createRoomAdminRequestHandler(this->m_user, this->m_roomID) };	// will be game handler in the next version
	}
	// return room data
	RoomData roomData = this->m_roomManager.getRoom(this->m_roomID).getMetaData();
	std::vector<std::string> players = this->m_roomManager.getRoom(this->m_roomID).getAllUsers();
	Buffer responseBuffer = JsonResponsePacketSerializer::serializeResponse(GetRoomStateResponse{ 1, roomData.isActive == 1, players, roomData.numOfQuestionsInGame, roomData.timePerQuestion });
	return RequestResult{ responseBuffer, this->copyHandler() };
}

IRequestHandler* RoomAdminRequestHandler::copyHandler()
{
	return this->m_handlerFactory.createRoomAdminRequestHandler(this->m_user, this->m_roomID);
}

RequestResult RoomAdminRequestHandler::startGame(RequestInfo)
{
	// change state to active
	this->m_roomManager.getRoom(this->m_roomID).setActive();
	// return response
	Buffer responseBuffer = JsonResponsePacketSerializer::serializeResponse(StartGameResponse{ 1 });
	return RequestResult{ responseBuffer, this->m_handlerFactory.createRoomAdminRequestHandler(this->m_user, this->m_roomID) };	// will be game handler in the next version
}
