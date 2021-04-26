#include "RequestHandlerFactory.h"

LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
    return new LoginRequestHandler();
}

MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler()
{
    return new MenuRequestHandler();
}

LoginManager& RequestHandlerFactory::getLoginManager()
{
    return this->m_loginManager;
}
