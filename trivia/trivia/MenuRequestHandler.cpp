#include "MenuRequestHandler.h"

MenuRequestHandler::MenuRequestHandler(RequestHandlerFactory& factory, LoggedUser user) :
	m_handlerFactory(factory), m_roomManager(factory.getRoomManager()), m_statisticsManager(factory.getStatisticsManager()), m_user(user)
{
}

bool MenuRequestHandler::isRequestRelevant(RequestInfo requestInfo)
{
	return LOGOUT <= requestInfo.id && requestInfo.id <= USER_STATISTICS;
}

RequestResult MenuRequestHandler::handleRequest(RequestInfo requestInfo)
{
	switch (requestInfo.id)
	{
	case LOGOUT:
		return signout(requestInfo);
	case GET_ROOMS:
		return getRooms(requestInfo);
	case GET_PLAYERS_IN_ROOM:
		return getPlayersInRoom(requestInfo);
	case JOIN_ROOM:
		return joinRoom(requestInfo);
	case CREATE_ROOM:
		return createRoom(requestInfo);
	case HIGH_SCORE:
		return getHighScores(requestInfo);
	case USER_STATISTICS:
		return getPersonalStatistics(requestInfo);
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

RequestResult MenuRequestHandler::signout(RequestInfo requestInfo)
{
	// logout from manager
	bool result = this->m_handlerFactory.getLoginManager().logout(this->m_user.getUsername());
	// send response
	Buffer responseBuffer = JsonResponsePacketSerializer::serializeResponse(LoginResponse{ result ? (unsigned int)1 : 0 });
	return RequestResult{ responseBuffer , this->m_handlerFactory.createLoginRequestHandler() };
}

RequestResult MenuRequestHandler::getRooms(RequestInfo)
{
	// get list of rooms
	std::vector<RoomData> rooms = this->m_roomManager.getRooms();
	// send response
	Buffer responseBuffer = JsonResponsePacketSerializer::serializeResponse(GetRoomsResponse{ 1, rooms });
	return RequestResult{ responseBuffer, this->m_handlerFactory.createMenuRequestHandler(this->m_user) };
}

RequestResult MenuRequestHandler::getPlayersInRoom(RequestInfo requestInfo)
{
	// deserialize response
	GetPlayersInRoomRequest request = JsonRequestPacketDeserializer::deserializeGetPlayersRequest(requestInfo.jsonBuffer);
	// get response and return
	std::vector<std::string> players = this->m_roomManager.getRoom(request.roomId).getAllUsers();
	Buffer responseBuffer = JsonResponsePacketSerializer::serializeResponse(GetPlayersInRoomResponse{ players });
	return RequestResult{ responseBuffer, this->m_handlerFactory.createMenuRequestHandler(this->m_user) };
}

RequestResult MenuRequestHandler::getPersonalStatistics(RequestInfo)
{
	// get statistics
	std::vector<std::string> statistics = this->m_statisticsManager.getUserStatistics(this->m_user.getUsername());
	// get response and return
	Buffer responseBuffer = JsonResponsePacketSerializer::serializeResponse(GetPersonalStatsResponse{ 1, statistics });
	return RequestResult{ responseBuffer, this->m_handlerFactory.createMenuRequestHandler(this->m_user) };
}

RequestResult MenuRequestHandler::getHighScores(RequestInfo)
{
	// get statistics
	std::vector<std::string> highScores = this->m_statisticsManager.getHighScores();
	// get response and return
	Buffer responseBuffer = JsonResponsePacketSerializer::serializeResponse(GetHighScoreResponse{ 1, highScores });
	return RequestResult{ responseBuffer, this->m_handlerFactory.createMenuRequestHandler(this->m_user) };
}

RequestResult MenuRequestHandler::joinRoom(RequestInfo requestInfo)
{
	JoinRoomRequest request =  JsonRequestPacketDeserializer::deserializeJoinRoomRequest(requestInfo.jsonBuffer);
	// join room and return response
	bool res = this->m_roomManager.getRoom(request.roomId).addUser(this->m_user);
	Buffer responseBuffer = JsonResponsePacketSerializer::serializeResponse(JoinRoomResponse{ res ? (unsigned int)1 : 0 });
	return RequestResult{ responseBuffer, this->m_handlerFactory.createMenuRequestHandler(this->m_user) };
}

RequestResult MenuRequestHandler::createRoom(RequestInfo requestInfo)
{
	CreateRoomRequest request = JsonRequestPacketDeserializer::deserializeCreateRoomRequest(requestInfo.jsonBuffer);
	// create room and return response
	bool res = this->m_roomManager.createRoom(this->m_user, RoomData{0, request.roomName, request.maxUsers, request.questionCount, request.answerTimeout, false});
	Buffer responseBuffer = JsonResponsePacketSerializer::serializeResponse(CreateRoomResponse{ res ? (unsigned int)1 : 0 });
	return RequestResult{ responseBuffer, this->m_handlerFactory.createMenuRequestHandler(this->m_user) };
	return RequestResult();
}
