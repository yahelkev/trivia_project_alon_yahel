#pragma once
#include "MessageStructs.h"
#include "Constants.h"
#include "json.hpp"

using json = nlohmann::json;

class JsonResponsePacketSerializer
{
public:
	static Buffer serializeResponse(LoginResponse);
	static Buffer serializeResponse(SignupResponse);
	static Buffer serializeResponse(ErrorResponse);
	//2.0.0
	static Buffer serializeResponse(LogoutResponse);
	static Buffer serializeResponse(GetRoomsResponse);
	static Buffer serializeResponse(GetPlayersInRoomResponse);
	static Buffer serializeResponse(JoinRoomResponse);
	static Buffer serializeResponse(CreateRoomResponse);
	static Buffer serializeResponse(GetHighScoreResponse);
	static Buffer serializeResponse(GetPersonalStatsResponse);
	//3.0.0 waiting room
	static Buffer serializeResponse(CloseRoomResponse);
	static Buffer serializeResponse(StartGameResponse);
	static Buffer serializeResponse(GetRoomStateResponse);
	static Buffer serializeResponse(LeaveRoomResponse);

	//4.0.0
	static Buffer serializeResponse(GetGameResultsResponse);
	static Buffer serializeResponse(SubmitAnswerResponse);
	static Buffer serializeResponse(GetQuestionResponse);
	static Buffer serializeResponse(LeaveGameResponse);
	//bonus
	static Buffer serializeResponse(ChangePasswordResponse);
	static Buffer serializeResponse(ResetPasswordResponse);

private:
	// function gets response code and json and returns a buffer that can be sent back to the client
	static Buffer createResponseBuffer(Byte code, json& content);
};

