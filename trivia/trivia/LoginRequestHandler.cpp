#include "LoginRequestHandler.h"

bool LoginRequestHandler::isRequestRelevant(RequestInfo rqstInfo)
{
	return rqstInfo.id == 1 || rqstInfo.id == 2;
}

RequestResult LoginRequestHandler::handleRequest(RequestInfo requesrInfo)
{
	RequestResult result = { Buffer(), nullptr };
	result.response;
	result.newHandler;
	switch (requesrInfo.id)
	{
	case LOGIN:
	{
		LoginRequest requestData = JsonRequestPacketDeserializer::deserializeLoginRequest(requesrInfo.jsonBuffer);
		std::cout << "Loged in user: " << requestData.username << " -- " << requestData.password << std::endl;
		result.response = JsonResponsePacketSerializer::serializeResponse(LoginResponse{ 1 });
		result.newHandler = new LoginRequestHandler;
		break;
	}
	case SIGNUP:
	{
		SignupRequest requestData = JsonRequestPacketDeserializer::deserializeSignupRequest(requesrInfo.jsonBuffer);
		std::cout << "New user: " << requestData.username << " -- " << requestData.password << " -- " << requestData.email << std::endl;
		result.response = JsonResponsePacketSerializer::serializeResponse(SignupResponse{ 1 });
		result.newHandler = new LoginRequestHandler;
		break;
	}
	default:
		result.response = JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ "Invalid request code for your state!" });
		result.newHandler = new LoginRequestHandler;
		break;
	}
	return result;
}
