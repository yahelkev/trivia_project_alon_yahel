#include "MenuRequestHandler.h"

MenuRequestHandler::MenuRequestHandler(RequestHandlerFactory& factory, LoggedUser user) :
	m_handlerFactory(factory), m_roomManager(factory.getRoomManager()), m_statisticsManager(factory.getStatisticsManager()), m_user(user)
{
}

bool MenuRequestHandler::isRequestRelevant(RequestInfo requestInfo)
{
	return LOGOUT <= requestInfo.id && requestInfo.id <= USER_STATISTICS ||
		requestInfo.id == RESET_PASSWORD || requestInfo.id == CHANGE_PASSWORD;
}

RequestResult MenuRequestHandler::handleRequest(RequestInfo requestInfo)
{
	switch (requestInfo.id)
	{
	case LOGOUT:
		return signout();
	case GET_ROOMS:
		return getRooms();
	case GET_PLAYERS_IN_ROOM:
		return getPlayersInRoom(requestInfo);
	case JOIN_ROOM:
		return joinRoom(requestInfo);
	case CREATE_ROOM:
		return createRoom(requestInfo);
	case HIGH_SCORE:
		return getHighScores();
	case USER_STATISTICS:
		return getPersonalStatistics();
	case CHANGE_PASSWORD:
		return changePassword(requestInfo);
	case RESET_PASSWORD:
		return resetPassword(requestInfo);
	default:
	{
		Buffer buffer = JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ "Invalid request code for your state!" });
		return RequestResult{ buffer, this->m_handlerFactory.createMenuRequestHandler(this->m_user) };
	}
	}
}

void MenuRequestHandler::abortSignout()
{
	this->m_handlerFactory.getLoginManager().logout(this->m_user.getUsername());
}

RequestResult MenuRequestHandler::signout()
{
	// logout from manager
	bool result = this->m_handlerFactory.getLoginManager().logout(this->m_user.getUsername());
	// send response
	Buffer responseBuffer = JsonResponsePacketSerializer::serializeResponse(LogoutResponse{ result ? (unsigned int)1 : 0 });
	return RequestResult{ responseBuffer , this->m_handlerFactory.createLoginRequestHandler() };
}

RequestResult MenuRequestHandler::getRooms()
{
	// get list of rooms
	std::vector<RoomData> rooms = this->m_roomManager.getRooms();
	// send response
	Buffer responseBuffer = JsonResponsePacketSerializer::serializeResponse(GetRoomsResponse{ 1, rooms });
	return RequestResult{ responseBuffer, this->m_handlerFactory.createMenuRequestHandler(this->m_user) };
}

RequestResult MenuRequestHandler::getPlayersInRoom(RequestInfo requestInfo)
{
	Buffer responseBuffer;
	// deserialize response
	GetPlayersInRoomRequest request = JsonRequestPacketDeserializer::deserializeGetPlayersRequest(requestInfo.jsonBuffer);
	// get response and return
	if (this->m_roomManager.doesRoomExist(request.roomId))
	{	// get list of players
		std::vector<std::string> players = this->m_roomManager.getRoom(request.roomId).getAllUsers();
		responseBuffer = JsonResponsePacketSerializer::serializeResponse(GetPlayersInRoomResponse{ players });
	}
	else
	{	// room doesn't exists, return error
		responseBuffer = responseBuffer = JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ "Room doesn't exist!" });
	}
	return RequestResult{ responseBuffer, this->m_handlerFactory.createMenuRequestHandler(this->m_user) };
}

RequestResult MenuRequestHandler::getPersonalStatistics()
{
	// get statistics
	std::vector<std::string> statistics = this->m_statisticsManager.getUserStatistics(this->m_user.getUsername());
	// get response and return
	Buffer responseBuffer = JsonResponsePacketSerializer::serializeResponse(GetPersonalStatsResponse{ 1, statistics });
	return RequestResult{ responseBuffer, this->m_handlerFactory.createMenuRequestHandler(this->m_user) };
}

RequestResult MenuRequestHandler::getHighScores()
{
	// get statistics
	std::vector<std::string> highScores = this->m_statisticsManager.getHighScores();
	// get response and return
	Buffer responseBuffer = JsonResponsePacketSerializer::serializeResponse(GetHighScoreResponse{ 1, highScores });
	return RequestResult{ responseBuffer, this->m_handlerFactory.createMenuRequestHandler(this->m_user) };
}

RequestResult MenuRequestHandler::joinRoom(RequestInfo requestInfo)
{
	JoinRoomRequest request = JsonRequestPacketDeserializer::deserializeJoinRoomRequest(requestInfo.jsonBuffer);
	Buffer responseBuffer;
	IRequestHandler* newHandler = nullptr;
	// join room and return response
	if (this->m_roomManager.doesRoomExist(request.roomId))
	{	// add user
		bool res = this->m_roomManager.getRoom(request.roomId).addUser(this->m_user);
		responseBuffer = JsonResponsePacketSerializer::serializeResponse(JoinRoomResponse{ res ? (unsigned int)1 : 0 });
		if (!res)	// failed to join
			newHandler = this->m_handlerFactory.createMenuRequestHandler(this->m_user);
		else	// joined successfully
			newHandler = (IRequestHandler*)this->m_handlerFactory.createRoomMemberRequestHandler(this->m_user, request.roomId);
	}
	else
	{	// room doesn't exists, return error
		responseBuffer = JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ "Room doesn't exist!" });
		newHandler = this->m_handlerFactory.createMenuRequestHandler(this->m_user);
	}
	return RequestResult{ responseBuffer, newHandler };
}

RequestResult MenuRequestHandler::createRoom(RequestInfo requestInfo)
{
	CreateRoomRequest request = JsonRequestPacketDeserializer::deserializeCreateRoomRequest(requestInfo.jsonBuffer);
	// create room and return response
	roomID room = this->m_roomManager.createRoom(this->m_user, RoomData{0, request.roomName, request.maxUsers, request.questionCount, request.answerTimeout, false});
	Buffer responseBuffer = JsonResponsePacketSerializer::serializeResponse(CreateRoomResponse{ room == INVALID_ROOM ? 0u : 1u, room });
	return RequestResult{ responseBuffer, (IRequestHandler*)this->m_handlerFactory.createRoomAdminRequestHandler(this->m_user, room) };
}

RequestResult MenuRequestHandler::changePassword(RequestInfo requestInfo)
{
	ChangePasswordRequest request = JsonRequestPacketDeserializer::deserializeChangePasswordRequest(requestInfo.jsonBuffer);
	Buffer responseBuffer = JsonResponsePacketSerializer::serializeResponse(ChangePasswordResponse{
		m_handlerFactory.getLoginManager().changePassword(request.password, m_user.getUsername(), request.oldPassword)});
	return RequestResult{ responseBuffer,  this->m_handlerFactory.createMenuRequestHandler(this->m_user) };
}

RequestResult MenuRequestHandler::resetPassword(RequestInfo requestInfo)
{
	return RequestResult();
}
