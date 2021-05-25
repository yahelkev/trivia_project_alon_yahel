#pragma once
#include "IRequestHandler.h"
#include "GameManager.h"
#include "RequestHandlerFactory.h"

class RequestHandlerFactory;
class GameRequestHandler :
    public IRequestHandler
{
public:
    GameRequestHandler(RequestHandlerFactory& factory, roomID roomId, LoggedUser user);

    bool isRequestRelevant(RequestInfo);
    RequestResult handleRequest(RequestInfo);

private:
    RequestResult getQuestion(RequestInfo);
    RequestResult submitAnswer(RequestInfo);
    RequestResult getGameResults(RequestInfo);
    RequestResult leaveGame(RequestInfo);

    roomID m_game;
    LoggedUser m_user;
    GameManager& m_gameManager;
    RequestHandlerFactory& m_handlerFacroty;

};

