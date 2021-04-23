#include "UserManager.h"

UserManager::UserManager(sqlite3* db) { 
	_db = db; 
	_bool_login_valid = false;
}

void UserManager::register_user(User& obj_user) {
	char* errorMessage;
	int i_return_code;

	std::string str_user_insert = "INSERT INTO users (name, age, email, password) VALUES ('" + 
		obj_user.get_full_name() + "', " + 
		std::to_string(obj_user.get_age()) + ", '" + 
		obj_user.get_email() + "', '" + 
		obj_user.get_password() +"');";

	i_return_code = sqlite3_exec(_db, str_user_insert.c_str(), NULL, NULL, &errorMessage);

	if (i_return_code != SQLITE_OK) {
		throw std::invalid_argument(errorMessage);
	}
}

void UserManager::attempt_login(User& obj_user) {
	int i_return_code;
	sqlite3_stmt* stmt_user_query;

	std::string str_find_user_sql = "SELECT * FROM users WHERE email = '" + obj_user.get_email() + "' AND password = '" + obj_user.get_password() + "'";

	sqlite3_prepare_v2(_db, str_find_user_sql.c_str(), -1, &stmt_user_query, NULL);
	i_return_code = sqlite3_step(stmt_user_query);

	if (i_return_code != SQLITE_ROW) {
		sqlite3_finalize(stmt_user_query);
		throw std::invalid_argument("No matching user could be found with the provided details.");
	}

	obj_user.set_full_name((char*)sqlite3_column_text(stmt_user_query, 1));
	obj_user.set_age(sqlite3_column_int(stmt_user_query, 2));
	obj_user.set_is_admin(sqlite3_column_int(stmt_user_query, 5));
	obj_user.set_id(sqlite3_column_int(stmt_user_query, 0));

	_bool_login_valid = true;
	_obj_current_user = obj_user;

	sqlite3_finalize(stmt_user_query);
}

void UserManager::logout() {
	_bool_login_valid = false;
	_obj_current_user = User();
}

void UserManager::fetch_users(bool no_admins) {
	_vec_users.clear();
	sqlite3_stmt* stmt_fetch_users;

	std::string str_sql = "SELECT * FROM users ORDER BY email";

	if (no_admins) str_sql = "SELECT * FROM users WHERE is_admin = 0 ORDER BY email";

	sqlite3_prepare_v2(_db, str_sql.c_str(), -1, &stmt_fetch_users, NULL);
	while (sqlite3_step(stmt_fetch_users) == SQLITE_ROW) {
		_vec_users.push_back(User(
			sqlite3_column_int(stmt_fetch_users, 0), 
			(char*)sqlite3_column_text(stmt_fetch_users, 1), 
			sqlite3_column_int(stmt_fetch_users, 2),
			(char*)sqlite3_column_text(stmt_fetch_users, 3), 
			(char*)sqlite3_column_text(stmt_fetch_users, 4),
			(sqlite3_column_int(stmt_fetch_users, 5) == 1) ? true : false));
	}

	sqlite3_finalize(stmt_fetch_users);
}

void UserManager::update_user_password(User& obj_user) {
	sqlite3_stmt* stmt_update_user_password;
	std::string str_update_user_password = "UPDATE users SET password = ? WHERE id = ?";

	if (sqlite3_prepare_v2(_db, str_update_user_password.c_str(), -1, &stmt_update_user_password, NULL) != SQLITE_OK) {
		std::string str_error_msg = "Failed to prepare insert statement: ";
		str_error_msg = str_error_msg + (char*)sqlite3_errmsg(_db);
		throw std::runtime_error(str_error_msg);
	}

	sqlite3_bind_text(stmt_update_user_password, 1, obj_user.get_password().c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_int(stmt_update_user_password, 2, obj_user.get_id());

	if (sqlite3_step(stmt_update_user_password) != SQLITE_DONE) {
		throw std::runtime_error("Something went wrong while updating password, please try again.");
	}

	sqlite3_finalize(stmt_update_user_password);
}

void UserManager::update_user_age(User& obj_user) {
	sqlite3_stmt* stmt_update_user_age;
	std::string str_update_user_age = "UPDATE users SET age = ? WHERE id = ?";

	if (sqlite3_prepare_v2(_db, str_update_user_age.c_str(), -1, &stmt_update_user_age, NULL) != SQLITE_OK) {
		std::string str_error_msg = "Failed to prepare insert statement: ";
		str_error_msg = str_error_msg + (char*)sqlite3_errmsg(_db);
		throw std::runtime_error(str_error_msg);
	}

	sqlite3_bind_int(stmt_update_user_age, 1, obj_user.get_age());
	sqlite3_bind_int(stmt_update_user_age, 2, obj_user.get_id());

	if (sqlite3_step(stmt_update_user_age) != SQLITE_DONE) {
		throw std::runtime_error("Something went wrong while updating age, please try again.");
	}

	sqlite3_finalize(stmt_update_user_age);
}

void UserManager::update_user_fullname(User& obj_user) {
	sqlite3_stmt* stmt_update_user_name;
	std::string str_update_user_name = "UPDATE users SET name = ? WHERE id = ?";

	if (sqlite3_prepare_v2(_db, str_update_user_name.c_str(), -1, &stmt_update_user_name, NULL) != SQLITE_OK) {
		std::string str_error_msg = "Failed to prepare insert statement: ";
		str_error_msg = str_error_msg + (char*)sqlite3_errmsg(_db);
		throw std::runtime_error(str_error_msg);
	}

	sqlite3_bind_text(stmt_update_user_name, 1, obj_user.get_full_name().c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_int(stmt_update_user_name, 2, obj_user.get_id());

	if (sqlite3_step(stmt_update_user_name) != SQLITE_DONE) {
		throw std::runtime_error("Something went wrong while updating full name, please try again.");
	}

	sqlite3_finalize(stmt_update_user_name);
}

void UserManager::update_user_email(User& obj_user) {
	sqlite3_stmt* stmt_update_user_email;
	std::string str_update_user_email = "UPDATE users SET email = ? WHERE id = ?";

	if (sqlite3_prepare_v2(_db, str_update_user_email.c_str(), -1, &stmt_update_user_email, NULL) != SQLITE_OK) {
		std::string str_error_msg = "Failed to prepare insert statement: ";
		str_error_msg = str_error_msg + (char*)sqlite3_errmsg(_db);
		throw std::runtime_error(str_error_msg);
	}

	sqlite3_bind_text(stmt_update_user_email, 1, obj_user.get_email().c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_int(stmt_update_user_email, 2, obj_user.get_id());

	if (sqlite3_step(stmt_update_user_email) != SQLITE_DONE) {
		throw std::runtime_error("Something went wrong while updating email (Most likely, a matching email conflict), please try again.");
	}

	sqlite3_finalize(stmt_update_user_email);
}

void UserManager::change_user_admin_status(User& obj_user) {
	sqlite3_stmt* stmt_update_user_admin_status;
	std::string str_update_user_admin_status = "UPDATE users SET is_admin = ? WHERE id = ?";

	if (sqlite3_prepare_v2(_db, str_update_user_admin_status.c_str(), -1, &stmt_update_user_admin_status, NULL) != SQLITE_OK) {
		std::string str_error_msg = "Failed to prepare insert statement: ";
		str_error_msg = str_error_msg + (char*)sqlite3_errmsg(_db);
		throw std::runtime_error(str_error_msg);
	}

	sqlite3_bind_int(stmt_update_user_admin_status, 1, obj_user.get_is_admin() ? 1 : 0);
	sqlite3_bind_int(stmt_update_user_admin_status, 2, obj_user.get_id());

	if (sqlite3_step(stmt_update_user_admin_status) != SQLITE_DONE) {
		throw std::runtime_error("Something went wrong while updating admin status, please try again.");
	}

	sqlite3_finalize(stmt_update_user_admin_status);
}