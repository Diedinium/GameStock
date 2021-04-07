#include "User.h"

User::User() {
	_i_id = 0;
	_str_full_name = "";
	_i_age = 0;
	_str_email = "";
	_str_password = "";
	_bool_is_admin = false;
};

User::User(std::string str_full_name, int i_age, std::string str_email, std::string str_password, bool bool_is_admin) {
	_i_id = 0;
	_str_full_name = str_full_name;
	_i_age = i_age;
	_str_email = str_email;
	_str_password = str_password;
	_bool_is_admin = bool_is_admin;
};