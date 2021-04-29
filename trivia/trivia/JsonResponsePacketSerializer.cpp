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

Buffer JsonResponsePacketSerializer::serializeResponse(GetRoomsResponse)
{
	return Buffer();
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
