#include "JsonRequestPacketDeserializer.h"

LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(Buffer msg)
{
	int code = msg[0] - CONVERT_TO_DECIMAL;
	Buffer sizeOfJsonData{ msg.begin() + 1, msg.begin() + 1 + CONTENT_LENGTH_BYTES };
	json jsonData = json::parse(msg.begin() + CONTENT_LENGTH_BYTES + 1, msg.end());
	std::cout << "code is: " << code << std::endl;
	//std::cout << "size is: " << sizeOfJsonData << std::endl;
	std::cout << "json data: " << jsonData << std::endl;
	LoginRequest result;
	result.password = jsonData["password"];
	result.username = jsonData["username"];
	return result;
}
