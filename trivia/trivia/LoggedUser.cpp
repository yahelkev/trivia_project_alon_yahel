#include "LoggedUser.h"


const std::string& LoggedUser::getUsername() const
{
	return this->m_username;
}

bool LoggedUser::operator==(const LoggedUser& other)
{
	return this->m_username == other.m_username;
}

bool LoggedUser::operator<(const LoggedUser& other) const
{
	return this->m_username < other.m_username;
}
