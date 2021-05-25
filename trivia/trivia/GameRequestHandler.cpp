#include "GameRequestHandler.h"

GameRequestHandler::GameRequestHandler(RequestHandlerFactory& factory, roomID roomId, LoggedUser user) :
	 m_handlerFacroty(factory), m_game(roomId), m_user(user), m_gameManager(m_handlerFacroty.getGameManager())
{
}

bool GameRequestHandler::isRequestRelevant(RequestInfo requestInfo)
{
	return GAME_RESULTS <= requestInfo.id && requestInfo.id <= LEAVE_GAME;
}

RequestResult GameRequestHandler::handleRequest(RequestInfo requestInfo)
{

	switch (requestInfo.id)
	{
	case GAME_RESULTS:
		return getGameResults();
	case SUBMIT_ANSWER:
		return submitAnswer(requestInfo);
	case GET_QUESTION:
		return getQuestion();
	case LEAVE_GAME:
		return leaveGame();
	default:
	{
		Buffer buffer = JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ "Invalid request code for your state!" });
		return RequestResult{ buffer, this->m_handlerFacroty.createMenuRequestHandler(this->m_user) };
	}
	}
}

void GameRequestHandler::abortSignout()
{
	m_gameManager.removePlayer(m_game, m_user);
	m_handlerFacroty.getLoginManager().logout(this->m_user.getUsername());
}

RequestResult GameRequestHandler::getQuestion()
{
	Question newQuestion = m_gameManager.getGame(m_game).getQuestionForUser(m_user);
	std::map<unsigned int, std::string> answers;
	std::vector<std::string> possibleAnswers = newQuestion.getPossibleAnswers();
	//gets the answers as a map (for the request result)
	for (int i = 0; i < NUM_OF_QUESTIONS; i++)
	{
		answers[i] = possibleAnswers[i];
	}
	Buffer responseBuffer = JsonResponsePacketSerializer::serializeResponse(GetQuestionResponse{ 1,
		newQuestion.getQuestion(), answers });
	return  RequestResult{ responseBuffer , m_handlerFacroty.createGameRequestHandler(m_game, m_user) };
}

RequestResult GameRequestHandler::submitAnswer(RequestInfo requestInfo)
{
	SubmitAnswerRequest request = JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(requestInfo.jsonBuffer);
	SubmitAnswerResponse response = { 1, m_gameManager.getGame(m_game).submitAnswer(m_user, request.answerId) };
	Buffer responseBuffer = JsonResponsePacketSerializer::serializeResponse(response);
	return  RequestResult{ responseBuffer , m_handlerFacroty.createGameRequestHandler(m_game, m_user) };
}

RequestResult GameRequestHandler::getGameResults()
{
	GetGameResultsResponse response = { 1};
	Buffer responseBuffer = JsonResponsePacketSerializer::serializeResponse(response);
	return  RequestResult{ responseBuffer , m_handlerFacroty.createGameRequestHandler(m_game, m_user) };
}

RequestResult GameRequestHandler::leaveGame()
{
	m_gameManager.removePlayer(m_game, m_user);
	LeaveGameResponse response = { 1 };
	Buffer responseBuffer = JsonResponsePacketSerializer::serializeResponse(response);
	return  RequestResult{ responseBuffer , m_handlerFacroty.createMenuRequestHandler(m_user) };
}
