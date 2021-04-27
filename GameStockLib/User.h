#pragma once
#include <string>

/// <summary>
/// Class used to store and represent a user.
/// </summary>
class User
{
	int _i_id;
	std::string _str_full_name;
	int _i_age;
	std::string _str_email;
	std::string _str_password;
	bool _bool_is_admin;
public:
	User();
	User(std::string str_full_name, int i_age, std::string str_email, std::string str_password, bool bool_is_admin);
	User(int i_id, std::string str_full_name, int i_age, std::string str_email, std::string str_password, bool bool_is_admin);

	int get_id() { return _i_id; }
	void set_id(int i_id) { _i_id = i_id; }
	
	std::string get_full_name() { return _str_full_name; }
	void set_full_name(std::string str_full_name) { _str_full_name = str_full_name; }

	std::string get_email() { return _str_email; }
	void set_email(std::string str_email) { _str_email = str_email; }

	int get_age() { return _i_age; }
	void set_age(int i_age) { _i_age = i_age; }

	std::string get_password() { return _str_password; }
	void set_password(std::string str_password) { _str_password = str_password; }

	bool get_is_admin() { return _bool_is_admin; }
	void set_is_admin(bool bool_is_admin) { _bool_is_admin = bool_is_admin; }
};

