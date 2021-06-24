#include "LoginManager.h"

std::mutex m_loggedUsersMutex;

bool LoginManager::signup(const std::string& username, const std::string& password, const std::string& email)
{
	if (m_database->doesUserExist(username))
	{
		return false;
	}
	// check info
	const std::regex username_check(".+");	// at least one char
	const std::regex password_check(PASSWORD_REGEX);// at least 8 letters, numbers or special symbols
	const std::regex email_check("([a-zA-Z0-9]+[!#$%&'*+-/=?^_`{|]?)*[a-zA-Z0-9]@([a-zA-Z0-9-]+\.)+[a-zA-Z0-9-]+");	// based on "https://help.returnpath.com/hc/en-us/articles/220560587-What-are-the-rules-for-email-address-syntax-"
	if (!std::regex_match(username, username_check) ||
		!std::regex_match(password, password_check) ||
		!std::regex_match(email, email_check))
	{
		// at least one parameter isn't valid
		return false;
	}
	// try to add to database
	return m_database->addNewUser(username, password, email);
}

bool LoginManager::login(const std::string& username, const std::string& password)
{ 
	//logs in only if the user exists and and it is not loged in yet
	std::lock_guard<std::mutex> loggedUsersLock(m_loggedUsersMutex);
	if (this->m_database->doesUserExist(username) && (std::find(m_loggedUsers.begin(), m_loggedUsers.end(), LoggedUser(username)) == m_loggedUsers.end()))
	{
		if (this->m_database->doesPasswordMatch(username, password) ||
			_oneTimePassword.find(username) != _oneTimePassword.end() && _oneTimePassword[username] == password)
		{
			this->m_loggedUsers.push_back(LoggedUser(username));
			_oneTimePassword.erase(username);
			return true;
		}
	}
	return false;
}

bool LoginManager::logout(const std::string& username)
{
	//logs out only if the user is loged in
	std::lock_guard<std::mutex> loggedUsersLock(m_loggedUsersMutex);
	if ((std::find(m_loggedUsers.begin(), m_loggedUsers.end(), LoggedUser(username)) != m_loggedUsers.end()))
	{
		m_loggedUsers.erase(std::remove(m_loggedUsers.begin(), m_loggedUsers.end(), LoggedUser(username)), m_loggedUsers.end());
		return true;
	}
	return false;
}

bool LoginManager::changePassword(const std::string& newPassword, const std::string& userName)
{
	const std::regex password_check(PASSWORD_REGEX);
	if (std::regex_match(newPassword, password_check))// if password is valid
	{
		return m_database->changePassword(userName, newPassword);
	}
	return false;
}

void LoginManager::resetPassword(const std::string& userName)
{
	std::string newPassword = randomPassword();
	_oneTimePassword[userName] = newPassword;
	std::string msg = "Python "   PY_MAIL_PATH  " " + m_database->getMail(userName) + 
		" \"Subject: Reset Password~From: Best Trivia~~Use this temporary password.~It can only be used once.~After you log in, change your password.~Password: " +
		newPassword + "\"";
	std::cout << msg << std::endl;
	system(msg.c_str());
}

std::string LoginManager::randomPassword()
{
	const char charset[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";
	const size_t max_index = sizeof(charset) - 2;

	std::random_device randomDevice;
	std::default_random_engine engine(randomDevice());
	std::uniform_int_distribution<int> distrinution(0, max_index);

	std::string password;
	for (int i = 0; i < RAND_PASSWORD_LENGHT; i++)
	{
		password += charset[distrinution(engine)];
	}
	return password;
}
