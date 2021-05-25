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

	switch (requestInfo.id)
	{
	case GAME_RESULTS:
		return this.();
	case SUBMIT_ANSWER:
		return getRooms();
	case GET_QUESTION:
		return getPlayersInRoom(requestInfo);
	case LEAVE_GAME:
		return joinRoom(requestInfo);
	default:
	{
		Buffer buffer = JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ "Invalid request code for your state!" });
		return RequestResult{ buffer, this->m_handlerFactory.createMenuRequestHandler(this->m_user) };
	}
	}
}

void GameRequestHandler::abortSignout()
{
	m_gameManager.removePlayer(m_game, m_user);
	m_handlerFacroty.getLoginManager().logout(this->m_user.getUsername());
}

