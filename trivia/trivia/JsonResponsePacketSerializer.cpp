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
	std::string rooms_data = "";
	for (auto room : response.rooms)
	{
		rooms_data += "{" + room.name + ":" + std::to_string(room.id) + "},";
	}
	//removes last ','
	if (!rooms_data.empty())
	{
		rooms_data.pop_back();
	}

	json content = {
		{"Rooms", rooms_data}
	};
	return createResponseBuffer(GET_ROOM, content);
}

Buffer JsonResponsePacketSerializer::serializeResponse(GetPlayersInRoomResponse response)
{
	std::string players_names = "";
	for (auto player : response.players)
	{
		players_names += player + ",";
	}
	//removes last ','
	if (!players_names.empty())
	{
		players_names.pop_back();
	}

	json content = {
		{"PlayersInRoom", players_names}
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
