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
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "username TEXT NOT NULL,"
            "password TEXT NOT NULL,"
            "email TEXT"
            ");"
            "CREATE TABLE IF NOT EXISTS Questions("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "question TEXT NOT NULL,"
            "answer0 TEXT NOT NULL,"
            "answer1 TEXT NOT NULL,"
            "answer2 TEXT NOT NULL,"
            "answer3 TEXT NOT NULL,"
            "correct_answer INTEGER NOT NULL"
            ");"
            "CREATE TABLE IF NOT EXISTS Statistics("
            "user_id INTEGER PRIMARY KEY,"
            "correct_answers INTEGER DEFAULT 0,"
            "total_answers INTEGER DEFAULT 0,"
            "game_count INTEGER DEFAULT 0,"
            "average_answer_time FLOAT DEFAULT 0,"
            "FOREIGN KEY(user_id) REFERENCES Users(id) ON DELETE CASCADE"
            ");"
        );
        // enforce foreign key constraint
        this->executeQuery("PRAGMA foreign_keys = ON;");
    }
    if (!success)
        throw std::exception("Opening Database Failed...");

    // add user defined functions to database
    // function score for finding highscores
    int a = sqlite3_create_function(this->_database, "SCORE", 4, SQLITE_UTF8, NULL,
        &SqliteDatabase::scoreFunction, NULL, NULL);
}
void SqliteDatabase::scoreFunction(sqlite3_context* context, int argc, sqlite3_value** argv)
{
    if (argc != 4)
    {
        sqlite3_result_null(context);
        return;
    }
    // return from score formula
    sqlite3_result_double(context, StatisticsManager::scoreFormula(sqlite3_value_double(argv[0]), 
        sqlite3_value_int(argv[1]), sqlite3_value_int(argv[2]), sqlite3_value_int(argv[3])));
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
    return this->executeQuery(
        // add user
        "INSERT INTO Users (username, password, email) VALUES ("
        "\"" + username + "\","
        "\"" + password + "\","
        "\"" + email + "\");"
        // add statistics
        "INSERT INTO Statistics (user_id) VALUES ((SELECT MAX(id) FROM Users));"
    );
}

std::list<Question> SqliteDatabase::getQuestions(int questionCount)
{
    std::list<Question> questionList;
    // get random questions
    this->executeQuery("SELECT * FROM Questions ORDER BY RANDOM() LIMIT " + std::to_string(questionCount) + ";",
        this->pushCallback<Question>, &questionList);
    return questionList;
}
float SqliteDatabase::getAverageAnswerTime(const std::string& username)
{
    return std::stof(this->valueQuery(
        "SELECT average_answer_time FROM Statistics WHERE "
        "user_id = (SELECT id FROM Users WHERE username = " + username + ");"
    ));
}
int SqliteDatabase::getNumOfCorrectAnswers(const std::string& username)
{
    return std::stoi(this->valueQuery(
        "SELECT correct_answers FROM Statistics WHERE "
        "user_id = (SELECT id FROM Users WHERE username = " + username + ");"
    ));
}
int SqliteDatabase::getNumOfTotalAnswers(const std::string& username)
{
    return std::stoi(this->valueQuery(
        "SELECT total_answers FROM Statistics WHERE "
        "user_id = (SELECT id FROM Users WHERE username = " + username + ");"
    ));
}
int SqliteDatabase::getNumOfPlayerGames(const std::string& username)
{
    return std::stoi(this->valueQuery(
        "SELECT game_count FROM Statistics WHERE "
        "user_id = (SELECT id FROM Users WHERE username = " + username + ");"
    ));
}

UserStatistics SqliteDatabase::getUserStatistics(const std::string& username)
{
    UserStatistics statistics;
    // get random questions
    this->executeQuery(
        "SELECT Statistics.*, Users.username FROM Statistics INNER JOIN Users ON Statistics.user_id = Users.id WHERE "
        "Users.username = \"" + username + "\";",
        this->createObjectCallback<UserStatistics>, &statistics);
    return statistics;
}

std::list<UserStatistics> SqliteDatabase::getHighScores()
{
    std::list<UserStatistics> statisticsList;
    // get random questions
    this->executeQuery("SELECT statistics.*, Users.username FROM Statistics INNER JOIN Users ON Statistics.user_id = Users.id "
        "ORDER BY SCORE(average_answer_time, correct_answers, total_answers, game_count) "
        "LIMIT " HIGHSCORES_USER_COUNT ";",
        this->pushCallback<UserStatistics>, &statisticsList);
    return statisticsList;
}

bool SqliteDatabase::executeQuery(const std::string& sql, callbackFunction callback, void* callbackData)
{
    std::lock_guard<std::mutex> databaseLock(this->_databaseMutex);
    int res = sqlite3_exec(this->_database, sql.c_str(), callback, callbackData, nullptr);
    return res == SQLITE_OK;
}

std::string SqliteDatabase::valueQuery(const std::string& sql)
{
    std::string value = "";
    // execute statement
    bool completeSuccesfully = this->executeQuery(sql, [](void* data, int argc, char** argv, char** cols)
        {
            // get one value
            if (argc == 1)
                *(std::string*)data = argv[0];
            return 0;
        },
        &value
    );
    if (!completeSuccesfully)
        return "";
    return value;
}
