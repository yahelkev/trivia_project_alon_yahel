#include "LoginManager.h"

bool LoginManager::signup(const std::string& username, const std::string& password, const std::string& email)
{
	std::lock_guard<std::mutex> dataLock(this->m_database_mutex);
	if (m_database->doesUserExist(username))
	{
		return false;
	}
	return m_database->addNewUser(username, password, email);
}

bool LoginManager::login(const std::string& username, const std::string& password)
{
	//logs in only if the user exists and and it is not loged in yet
	std::lock_guard<std::mutex> datalock(this->m_database_mutex);
	std::lock_guard<std::mutex> loggedUsersLock(this->m_lggedUsersMutex);
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
	//logs out only if the user is loged in
	std::lock_guard<std::mutex> loggedUsersLock(this->m_lggedUsersMutex);
	if ((std::find(m_loggedUsers.begin(), m_loggedUsers.end(), LoggedUser(username)) != m_loggedUsers.end()))
	{
		m_loggedUsers.erase(std::remove(m_loggedUsers.begin(), m_loggedUsers.end(), LoggedUser(username)), m_loggedUsers.end());
		return true;
	}
	return false;
}
