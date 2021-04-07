#include "UserManager.h"

UserManager::UserManager(sqlite3* db) { 
	_db = db; 
	_bool_login_valid = false;
}

void UserManager::register_user(User* ptr_user) {
	char* errorMessage;
	int i_return_code;

	std::string str_user_insert = "INSERT INTO users (name, age, email, password) VALUES ('" + 
		ptr_user->get_full_name() + "', " + 
		std::to_string(ptr_user->get_age()) + ", '" + 
		ptr_user->get_email() + "', '" + 
		ptr_user->get_password() +"');";

	i_return_code = sqlite3_exec(_db, str_user_insert.c_str(), NULL, NULL, &errorMessage);

	if (i_return_code != SQLITE_OK) {
		throw std::invalid_argument(errorMessage);
	}
}

void UserManager::attempt_login(User* ptr_user) {
	int i_return_code;
	sqlite3_stmt* stmt_user_query;

	std::string str_find_user_sql = "SELECT * FROM users WHERE email = '" + ptr_user->get_email() + "' AND password = '" + ptr_user->get_password() + "'";

	sqlite3_prepare_v2(_db, str_find_user_sql.c_str(), -1, &stmt_user_query, NULL);
	i_return_code = sqlite3_step(stmt_user_query);

	if (i_return_code != SQLITE_ROW) {
		sqlite3_finalize(stmt_user_query);
		throw std::invalid_argument("No matching user could be found with the provided details.");
	}

	ptr_user->set_full_name((char*)sqlite3_column_text(stmt_user_query, 1));
	ptr_user->set_age(sqlite3_column_int(stmt_user_query, 2));
	ptr_user->set_is_admin(sqlite3_column_int(stmt_user_query, 5));
	ptr_user->set_id(sqlite3_column_int(stmt_user_query, 0));

	_bool_login_valid = true;
	_obj_current_user = *ptr_user;

	sqlite3_finalize(stmt_user_query);
}

void UserManager::logout() {
	_bool_login_valid = false;
	_obj_current_user = User();
}