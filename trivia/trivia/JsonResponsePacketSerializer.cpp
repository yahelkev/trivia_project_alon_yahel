#include "JsonResponsePacketSerializer.h"

Buffer JsonResponsePacketSerializer::serializeResponse(LoginResponse response)
{
	json content = {
		{"status", response.status}
	};
	return createResponseBuffer(1, content);
}

Buffer JsonResponsePacketSerializer::serializeResponse(SignupResponse response)
{
	json content = {
		{"status", response.status}
	};
	return createResponseBuffer(2, content);
}

Buffer JsonResponsePacketSerializer::serializeResponse(ErrorResponse response)
{
	json content = {
		{"message", response.message}
	};
	return createResponseBuffer(0, content);
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
	buffer.insert(buffer.end(), contentLengthBytes, contentLengthBytes + 4);
	buffer.insert(buffer.end(), jsonDump.begin(), jsonDump.end());

	for (const Byte& b : buffer)
		std::cout << (int)b << " ";
	std::cout << std::endl;
	for (const Byte& b : buffer)
		std::cout << b;
	std::cout << std::endl << std::endl;

	return buffer;
}
