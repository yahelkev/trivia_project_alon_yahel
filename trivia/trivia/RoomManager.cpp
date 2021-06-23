#include "RoomManager.h"

RoomManager::RoomManager(IDatabase* dataBase) : _freeID(1), m_dataBase (dataBase)
{
}

roomID RoomManager::createRoom(LoggedUser user, RoomData data)
{
	if (data.numOfQuestionsInGame > m_dataBase->getNumOfQuestios())
	{
		return INVALID_ROOM;
	}
	std::lock_guard<std::mutex> usersLock(this->m_roomsMap_lock);
	data.id = this->_freeID++;
	if (this->m_rooms.find(data.id) != this->m_rooms.end())
	{
		return INVALID_ROOM;
	}
	this->m_rooms[data.id] = Room(data);
	this->m_rooms[data.id].addUser(user);
	return data.id;
}

bool RoomManager::deleteRoom(int ID)
{
	std::lock_guard<std::mutex> usersLock(this->m_roomsMap_lock);
	//if exists
	if (this->m_rooms.find(ID) != this->m_rooms.end())
	{
		m_rooms.erase(m_rooms.find(ID));
		return true;
	}
	return false;
}

unsigned int RoomManager::getRoomState(int ID)
{
	//if exists
	if (this->m_rooms.find(ID) != this->m_rooms.end())
	{
		return	m_rooms[ID].getMetaData().isActive;
	}
	throw std::exception("Room wan't found");
}

std::vector<RoomData> RoomManager::getRooms()
{
	std::vector<RoomData> result;
	for (auto it = m_rooms.begin(); it != m_rooms.end(); it++)
	{  
		result.push_back(it->second.getMetaData());
	}
	return result;
}

Room& RoomManager::getRoom(roomID id)
{
	return this->m_rooms[id];
}

bool RoomManager::doesRoomExist(roomID id)
{
	return this->m_rooms.find(id) != this->m_rooms.end();
}

