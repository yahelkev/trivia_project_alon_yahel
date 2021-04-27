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

bool SqliteDatabase::doesUserExist(std::string username)
{
    return this->valueQuery("SELECT COUNT(*) FROM Users WHERE username = \"" + username + "\";") != "0";
}

bool SqliteDatabase::doesPasswordMatch(std::string username, std::string password)
{
    return this->valueQuery("SELECT password FROM Users WHERE username = \"" + username + "\";") == password;
}

bool SqliteDatabase::addNewUser(std::string username, std::string password, std::string email)
{
    this->executeQuery("INSERT INTO Users (username, password, email) VALUES ("
        "\"" + username + "\","
        "\"" + password + "\","
        "\"" + email + "\");"
    );
    return true;
}

void SqliteDatabase::executeQuery(const std::string& sql, callbackFunction callback, void* callbackData)
{
    char* errorMsg = nullptr;
    int res = sqlite3_exec(this->_database, sql.c_str(), callback, callbackData, &errorMsg);
    if (res != SQLITE_OK)
    {
        throw std::exception(("SQL ERROR: " + std::string(errorMsg)).c_str());
    }
}

std::string SqliteDatabase::valueQuery(const std::string& sql)
{
    std::string value = "";
    // execute statement
    this->executeQuery(sql, [](void* data, int argc, char** argv, char** cols)
    {
        // get one value
        if (argc == 1)
            *(std::string*)data = argv[0];
        return 0;
    },
        &value
        );
    return value;
}
