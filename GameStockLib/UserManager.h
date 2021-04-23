#pragma once
#include <string>
#include <stdexcept>
#include <vector>
#include "sqlite3.h"
#include "User.h"

class UserManager
{
	sqlite3* _db;
	bool _bool_login_valid;
	User _obj_current_user;
	std::vector<User> _vec_users;
public:
	UserManager(sqlite3* db);

	void register_user(User& ptr_user);
	void attempt_login(User& ptr_user);
	void logout();

	bool is_login_valid() { return _bool_login_valid; }
	void set_login_valid(bool bool_login_valid) { _bool_login_valid = bool_login_valid; }

	int get_user_id() { return _obj_current_user.get_id(); }
	User& get_current_user() { return _obj_current_user; }

	std::vector<User>& get_vec_users() { return _vec_users; }

	/// <summary>
	/// Gets all users from database in order of email, populates _vec_users
	/// </summary>
	void fetch_users(bool no_admins = false);

	/// <summary>
	/// Pass in user object to update, password stored in obj_user will be used to then persist the password to the database.
	/// </summary>
	/// <param name="obj_user"></param>
	void update_user_password(User& obj_user);

	/// <summary>
	/// Pass in user object to update, age stored in obj_user will be used to then persist the age to the database.
	/// </summary>
	/// <param name="obj_user"></param>
	void update_user_age(User& obj_user);

	/// <summary>
	/// Pass in user object to update, full name stored in obj_user will be used to then persist the full name to the database.
	/// </summary>
	/// <param name="obj_user"></param>
	void update_user_fullname(User& obj_user);

	/// <summary>
	/// Pass in user object to update, email stored in obj_user will be used to then persist the email to the database.
	/// </summary>
	/// <param name="obj_user"></param>
	void update_user_email(User& obj_user);

	/// <summary>
	/// Pass in user object to update, admin status stored in obj_user will be used to then persist the admin status to the database.
	/// </summary>
	/// <param name="obj_user"></param>
	void change_user_admin_status(User& obj_user);
};

