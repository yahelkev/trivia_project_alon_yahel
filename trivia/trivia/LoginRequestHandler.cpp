#include "LoginRequestHandler.h"



LoginRequestHandler::LoginRequestHandler(RequestHandlerFactory& factory):
	m_handlerFactory(factory), m_loginManager(factory.getLoginManager()){
}

bool LoginRequestHandler::isRequestRelevant(RequestInfo rqstInfo)
{
	return rqstInfo.id == LOGIN || rqstInfo.id == SIGNUP || rqstInfo.id == RESET_PASSWORD;
}

RequestResult LoginRequestHandler::handleRequest(RequestInfo requesrInfo)
{
	RequestResult result = { Buffer(), nullptr };
	switch (requesrInfo.id)
	{
	case LOGIN:
	{
		LoginRequest requestData = JsonRequestPacketDeserializer::deserializeLoginRequest(requesrInfo.jsonBuffer);
		if (this->m_loginManager.login(requestData.username, requestData.password))
		{
			std::cout << "Loged in user: " << requestData.username << " -- " << requestData.password << std::endl;
			result.response = JsonResponsePacketSerializer::serializeResponse(LoginResponse{ 1 });
			result.newHandler = this->m_handlerFactory.createMenuRequestHandler(LoggedUser(requestData.username));
		}
		else
		{
			result.response = JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ "Error while logging in!" });
			result.newHandler = this->m_handlerFactory.createLoginRequestHandler();

		}
		break;
	}
	case SIGNUP:
	{
		SignupRequest requestData = JsonRequestPacketDeserializer::deserializeSignupRequest(requesrInfo.jsonBuffer);
		if (this->m_loginManager.signup(requestData.username, requestData.password, requestData.email))
		{
			std::cout << "New user: " << requestData.username << " -- " << requestData.password << " -- " << requestData.email << std::endl;
			result.response = JsonResponsePacketSerializer::serializeResponse(SignupResponse{ 1 });
			result.newHandler = this->m_handlerFactory.createLoginRequestHandler();
		}
		else
		{
			result.response = JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ "Error while signning up in!" });
			result.newHandler = this->m_handlerFactory.createLoginRequestHandler();
		}
		break;
	}
	case RESET_PASSWORD:
		result = resetPassword(requesrInfo);
		break;
	default:
		result.response = JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ "Invalid request code for your state!" });
		result.newHandler = this->m_handlerFactory.createLoginRequestHandler();
		break;
	}
	return result;
}

RequestResult LoginRequestHandler::resetPassword(RequestInfo requestInfo)
{
	ResetPasswordRequest request = JsonRequestPacketDeserializer::deserializeResetPasswordRequest(requestInfo.jsonBuffer);
	m_loginManager.resetPassword(request.userName);
	Buffer responseBuffer = JsonResponsePacketSerializer::serializeResponse(ResetPasswordResponse{ 1 });
	return RequestResult{ responseBuffer,  this->m_handlerFactory.createLoginRequestHandler() };
}
