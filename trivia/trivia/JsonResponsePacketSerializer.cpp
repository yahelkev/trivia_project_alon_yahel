#include "JsonResponsePacketSerializer.h"

Buffer JsonResponsePacketSerializer::serializeResponse(LoginResponse response)
{
	json content = {
		{"status", response.status}
	};
	return createResponseBuffer(LOGIN, content);
}

Buffer JsonResponsePacketSerializer::serializeResponse(SignupResponse response)
{
	json content = {
		{"status", response.status}
	};
	return createResponseBuffer(SIGNUP, content);
}

Buffer JsonResponsePacketSerializer::serializeResponse(ErrorResponse response)
{
	json content = {
		{"message", response.message}
	};
	return createResponseBuffer(ERROR_CODE, content);
}

Buffer JsonResponsePacketSerializer::serializeResponse(LogoutResponse response)
{
	json content = {
		{"status", response.status}
	};
	return createResponseBuffer(LOGOUT, content);
}

Buffer JsonResponsePacketSerializer::serializeResponse(GetRoomsResponse response)
{
	json rooms_data = json::object();
	for (auto room : response.rooms)
	{
		rooms_data.push_back({ room.name , room.id });
	}

	json content = {
		{ "Rooms", rooms_data }
	};
	return createResponseBuffer(GET_ROOMS, content);
}

Buffer JsonResponsePacketSerializer::serializeResponse(GetPlayersInRoomResponse response)
{
	json players_names = json::array();
	for (auto player : response.players)
	{
		players_names.push_back(player);
	}

	json content = {
		{ "PlayersInRoom", players_names }
	};
	return createResponseBuffer(GET_PLAYERS_IN_ROOM, content);
}

Buffer JsonResponsePacketSerializer::serializeResponse(JoinRoomResponse response)
{
	json content = {
		{"status", response.status}
	};
	return createResponseBuffer(JOIN_ROOM, content);
}

Buffer JsonResponsePacketSerializer::serializeResponse(CreateRoomResponse response)
{
	json content = {
		{"status", response.status}
	};
	return createResponseBuffer(CREATE_ROOM, content);
}

Buffer JsonResponsePacketSerializer::serializeResponse(GetHighScoreResponse response)
{
	json content = {
		{"HighScores", response.statistics}
	};
	return createResponseBuffer(HIGH_SCORE, content);
}

Buffer JsonResponsePacketSerializer::serializeResponse(GetPersonalStatsResponse response)
{
	json content = {
		{"UserStatistics", response.statistics}
	};
	return createResponseBuffer(USER_STATISTICS, content);
}


Buffer JsonResponsePacketSerializer::createResponseBuffer(Byte code, json& content)
{
	std::string jsonDump = content.dump();
	// get content length as bytes
	int contentLengthInt = jsonDump.size();
	Byte* contentLengthBytes = (Byte*)&contentLengthInt;
	// create buffer
	Buffer buffer;
	buffer.push_back(code);
	buffer.insert(buffer.end(), contentLengthBytes, contentLengthBytes + CONTENT_LENGTH_BYTES);
	buffer.insert(buffer.end(), jsonDump.begin(), jsonDump.end());

	return buffer;
}
