#include "RequestHandlerFactory.h"

LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
    return new LoginRequestHandler(*this);
}

MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler(LoggedUser user)
{
    return new MenuRequestHandler(*this, user);
}

LoginManager& RequestHandlerFactory::getLoginManager()
{
    return this->m_loginManager;
}

StatisticsManager& RequestHandlerFactory::getStatisticsManager()
{
    return this->m_statisticsManager;
}

RoomManager& RequestHandlerFactory::getRoomManager()
{
    return this->m_roomManager;
}
