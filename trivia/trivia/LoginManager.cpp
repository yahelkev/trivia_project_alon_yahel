#include "LoginManager.h"

bool LoginManager::signup(const std::string& username, const std::string& password, const std::string& email)
{
	if (m_database->doesUserExist(username))
	{
		return false;
	}
	return m_database->addNewUser(username, password, email);
}

bool LoginManager::login(const std::string& username, const std::string& password)
{
	if (this->m_database->doesUserExist(username) && (std::find(m_loggedUsers.begin(), m_loggedUsers.end(), LoggedUser(username)) == m_loggedUsers.end()))
	{
		if (this->m_database->doesPasswordMatch(username, password))
		{
			this->m_loggedUsers.push_back(LoggedUser(username));
			return true;
		}
	}
	return false;
}

bool LoginManager::logout(const std::string& username)
{
	if ((std::find(m_loggedUsers.begin(), m_loggedUsers.end(), LoggedUser(username)) != m_loggedUsers.end()))
	{
		m_loggedUsers.erase(std::remove(m_loggedUsers.begin(), m_loggedUsers.end(), LoggedUser(username)), m_loggedUsers.end());
		return true;
	}
	return false;
}
