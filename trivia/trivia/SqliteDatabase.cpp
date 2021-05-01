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
        // insert questions from file
        this->insertQuestions();
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
        "SELECT * FROM Statistics WHERE "
        "user_id = (SELECT id FROM Users WHERE username = " + username + ");",
        this->createObjectCallback<UserStatistics>, &statistics);
    return statistics;
}

std::list<UserStatistics> SqliteDatabase::getHighScores()
{
    std::list<UserStatistics> statisticsList;
    // get random questions
    this->executeQuery("SELECT * FROM Statistics ORDER BY user_id LIMIT " HIGHSCORES_USER_COUNT ";",
        this->pushCallback<UserStatistics>, &statisticsList);
    return statisticsList;
}

void SqliteDatabase::insertQuestions()
{
    // open file
    std::ifstream questionFile(QUESTION_FILE_PATH);
    if (!questionFile.is_open())
        return;
    // read json
    json questionJson;
    questionFile >> questionJson;
    questionFile.close();
    // insert questions into database
    std::string sql = "INSERT INTO Questions (question, answer0, answer1, answer2, answer3, correct_answer) VALUES ";
    for (const json& question : questionJson)
    {
        sql += '(' + question["question"].dump() + ',';
        // add answers
        for (const json& answer : question["answers"])
            sql += answer.dump() + ',';
        sql += question["correctAnswer"].dump() + "),";
    }
    sql.pop_back();
    this->executeQuery(sql + ';');
}

bool SqliteDatabase::executeQuery(const std::string& sql, callbackFunction callback, void* callbackData)
{
    std::lock_guard<std::mutex> databaseLock(this->_databaseMutex);
    char* errorMsg = nullptr;
    int res = sqlite3_exec(this->_database, sql.c_str(), callback, callbackData, &errorMsg);
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
