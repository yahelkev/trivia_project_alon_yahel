#include "Room.h"

bool Room::addUser(LoggedUser newUser)
{
    std::lock_guard<std::mutex> usersLock(this->m_users_lock);
    //add if not exists
    if (std::find(m_users.begin(), m_users.end(), newUser) == m_users.end() && // player not in room
        this->m_users.size() != this->m_metadata.maxPlayers)    // room not full
    {
        m_users.push_back(newUser);
        this->m_metadata.playerCount++;
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
        this->m_metadata.playerCount--;
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

void Room::setActive()
{
    this->m_metadata.isActive = 1;
}

RoomData Room::getMetaData()
{
    return this->m_metadata;
}

Room& Room::operator=(const Room& other)
{
    this->m_metadata.id = other.m_metadata.id;
    this->m_metadata.isActive = other.m_metadata.isActive;
    this->m_metadata.playerCount = other.m_metadata.playerCount;
    this->m_metadata.maxPlayers = other.m_metadata.maxPlayers;
    this->m_metadata.name = other.m_metadata.name;
    this->m_metadata.numOfQuestionsInGame = other.m_metadata.numOfQuestionsInGame;
    this->m_metadata.timePerQuestion = other.m_metadata.timePerQuestion;
    return *this;
}
