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
