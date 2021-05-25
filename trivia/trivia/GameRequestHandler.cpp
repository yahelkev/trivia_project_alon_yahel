#include "GameRequestHandler.h"

GameRequestHandler::GameRequestHandler(RequestHandlerFactory& factory, roomID roomId, LoggedUser user) :
	 m_handlerFacroty(factory), m_game(roomId), m_user(user), m_gameManager(m_handlerFacroty.getGameManager())
{
}

bool GameRequestHandler::isRequestRelevant(RequestInfo requestInfo)
{
	return GAME_RESULTS <= requestInfo.id && requestInfo.id <= LEAVE_GAME;
}

RequestResult GameRequestHandler::handleRequest(RequestInfo)
{
	return RequestResult();
}
