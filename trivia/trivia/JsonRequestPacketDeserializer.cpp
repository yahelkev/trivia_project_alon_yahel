#include "JsonRequestPacketDeserializer.h"

int JsonRequestPacketDeserializer::fourByteVecToDecimal(Buffer buffer)
{
	
	int result = 0;
	for (int i = 0; i < CONTENT_LENGTH_BYTES; i++)
	{
		result |= buffer[CONTENT_LENGTH_BYTES - i] << i* BYTE_SIZE;
	}

	return result;
}

LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(Buffer msg)
{
	json jsonData = json::parse(msg);
	LoginRequest result;
	result.password = jsonData["password"];
	result.username = jsonData["username"];
	return result;
}

SignupRequest JsonRequestPacketDeserializer::deserializeSignupRequest(Buffer msg)
{
	json jsonData = json::parse(msg);
	SignupRequest result;
	result.password = jsonData["password"];
	result.username = jsonData["username"];
	result.email = jsonData["email"];
	return result;
}

GetPlayersInRoomRequest JsonRequestPacketDeserializer::deserializeGetPlayersRequest(Buffer msg)
{
	json jsonData = json::parse(msg);
	GetPlayersInRoomRequest result;
	result.roomId = jsonData["roomId"];
	return result;
}

JoinRoomRequest JsonRequestPacketDeserializer::deserializeJoinRoomRequest(Buffer msg)
{
	json jsonData = json::parse(msg);
	JoinRoomRequest result;
	result.roomId = jsonData["roomId"];
	return result;
}

CreateRoomRequest JsonRequestPacketDeserializer::deserializeCreateRoomRequest(Buffer msg)
{
	json jsonData = json::parse(msg);
	CreateRoomRequest result;
	result.answerTimeout = jsonData["answerTimeout"];
	result.maxUsers = jsonData["maxUsers"];
	result.questionCount = jsonData["questionCount"];
	result.roomName = jsonData["roomName"];
	return result;
}
