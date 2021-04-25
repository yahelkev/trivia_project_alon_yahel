#include "SqliteDatabase.h"

SqliteDatabase::SqliteDatabase(std::string dbPath)
{
    // check if db already exists
    bool fileExists = _access(dbPath.c_str(), 0) != -1;
    // open db
    bool success = sqlite3_open(dbPath.c_str(), &this->_database) == SQLITE_OK;
    if (!fileExists && success)
    {   // new db, create tables
        this->executeQuery(
            "CREATE TABLE IF NOT EXISTS Users("
            "id INTEGER PRIMARY KEY,"
            "username TEXT NOT NULL,"
            "password TEXT NOT NULL,"
            "email TEXT"
            ");"
        );
    }
    if (!success)
        throw std::exception("Opening Database Failed...");
}

SqliteDatabase::~SqliteDatabase()
{
    sqlite3_close(this->_database);
}

bool SqliteDatabase::executeQuery(const std::string& sql, callbackFunction callback, void* callbackData)
{
    char* errorMsg = nullptr;
    bool empty = true;
    void* data[3] = { callback, callbackData, &empty };
    int res = sqlite3_exec(this->_database, sql.c_str(),
        // callback
        [](void* data, int argc, char** argv, char** cols)
        {
            // get data from param
            callbackFunction callback = (callbackFunction)((void**)data)[0];
            void* callbackData = ((void**)data)[1];
            bool& empty = *(bool*)((void**)data)[2];
            empty = false;
            // call original callback
            return callback(callbackData, argc, argv, cols);
        },
        data, &errorMsg);
    if (res != SQLITE_OK)
    {
        throw std::exception(("SQL ERROR: " + std::string(errorMsg)).c_str());
    }
    return !empty;
}
