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
	json rooms_data = json::array();
	for (auto room : response.rooms)
	{
		rooms_data.push_back({{ "name", room.name },
			{"id", room.id},
			{"playerCount", room.playerCount},
			{"maxPlayers", room.maxPlayers},
			{"numOfQuestions", room.numOfQuestionsInGame},
			{"timePerQuestion", room.timePerQuestion},
			});
	}

	json content = {
		{"status", response.status},
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
		{ "players", players_names }
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
		{"status", response.status},
		{"roomId", response.roomId}
	};
	return createResponseBuffer(CREATE_ROOM, content);
}

Buffer JsonResponsePacketSerializer::serializeResponse(GetHighScoreResponse response)
{
	json content = {
		{"status", response.status},
		{"HighScores", response.statistics}
	};
	return createResponseBuffer(HIGH_SCORE, content);
}

Buffer JsonResponsePacketSerializer::serializeResponse(GetPersonalStatsResponse response)
{
	json content = {
		{"status", response.status},
		{"UserStatistics", response.statistics}
	};
	return createResponseBuffer(USER_STATISTICS, content);
}

Buffer JsonResponsePacketSerializer::serializeResponse(GetGameResultsResponse response)
{
	//gets the relevant info in a way that will fit json
	json players_results = json::array();
	for (auto player : response.results)
	{
		players_results.push_back({
			{ "username", player.getUsername()},
			{ "correctAnswerCount" , player.getCorrectAnswerCount() },
			{ "wrongAnswerCount", player.getWrongAnswerCount() },
			{ "averageAnswerTime", player.getAverageAnswerTime() }
			});
	}
	json content = {
		{"status", response.status},
		{"results", players_results}
	};
	return createResponseBuffer(GAME_RESULTS, content);

}

Buffer JsonResponsePacketSerializer::serializeResponse(SubmitAnswerResponse response)
{
	json content = {
		{"status", response.status},
		{"correctAnswerId", response.correctAnswerId}
	};
	return createResponseBuffer(SUBMIT_ANSWER, content);
}

Buffer JsonResponsePacketSerializer::serializeResponse(GetQuestionResponse response)
{
	//turns the answers map to array
	json answers = json::array();
	for (const auto& answer : response.answers) {
		answers.push_back(answer.second);
	}
	json content = {
		{"status", response.status},
		{"question", response.question},
		{"answers", answers}
	};
	return createResponseBuffer(GET_QUESTION, content);
}

Buffer JsonResponsePacketSerializer::serializeResponse(LeaveGameResponse response)
{
	json content = {
		{"status", response.status}
	};
	return createResponseBuffer(LEAVE_GAME, content);
}

Buffer JsonResponsePacketSerializer::serializeResponse(CloseRoomResponse response)
{
	json content = {
		{"status", response.status}
	};
	return createResponseBuffer(LEAVE_ROOM, content);
}

Buffer JsonResponsePacketSerializer::serializeResponse(StartGameResponse response)
{
	json content = {
		{"status", response.status}
	};
	return createResponseBuffer(START_GAME, content);
}

Buffer JsonResponsePacketSerializer::serializeResponse(GetRoomStateResponse response)
{
	json content = {
		{"status", response.status},
		{"hasGameBegun", response.hasGameBegun},
		{"questionCount", response.questionCount},
		{"answerTimeout", response.answerTimeout},
		{"players", response.players}
	};
	return createResponseBuffer(GET_ROOM_STATE, content);
}

Buffer JsonResponsePacketSerializer::serializeResponse(LeaveRoomResponse response)
{
	json content = {
		{"status", response.status}
	};
	return createResponseBuffer(LEAVE_ROOM, content);
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
