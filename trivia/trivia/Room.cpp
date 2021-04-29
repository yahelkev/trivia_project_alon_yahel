#include "Room.h"

bool Room::addUser(LoggedUser newUser)
{
    std::lock_guard<std::mutex> usersLock(this->m_users_lock);
    //add if not exists
    if (std::find(m_users.begin(), m_users.end(), newUser) == m_users.end())
    {
        m_users.push_back(newUser);
        return true;
    }
    return false;
}

bool Room::removeUser(LoggedUser user)
{
    //removes only if the user is in the room
    std::lock_guard<std::mutex> usersLock(m_users_lock);
    if ((std::find(m_users.begin(), m_users.end(), LoggedUser(user)) != m_users.end()))
    {
        m_users.erase(std::remove(m_users.begin(), m_users.end(), LoggedUser(user)), m_users.end());
        return true;
    }
    return false;
}

std::vector<std::string> Room::getAllUsers()
{
    std::vector<std::string> result;
    std::lock_guard<std::mutex> usersLock(m_users_lock);
    for (auto i = m_users.begin(); i != m_users.end(); i++)
    {
        result.push_back(i->getUsername());
    }
    return result;
}
