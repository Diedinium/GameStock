#pragma once
#include "sqlite3.h"
#include "User.h"
#include <string>
#include <stdexcept>

class UserManager
{
	sqlite3* _db;
	bool _bool_login_valid;
	User _obj_current_user;
public:
	UserManager(sqlite3* db);

	void register_user(User* ptr_user);
	void attempt_login(User* ptr_user);
	void logout();

	bool is_login_valid() { return _bool_login_valid; }
	void set_login_valid(bool bool_login_valid) { _bool_login_valid = bool_login_valid; }
};

