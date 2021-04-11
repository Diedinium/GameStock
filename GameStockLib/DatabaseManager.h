#pragma once
#include <iostream>
#include "sqlite3.h"
#include <filesystem>

class DatabaseManager
{
	sqlite3* _db;
	int _i_return_code;
	std::filesystem::path _database_path = std::filesystem::path(L"database");
	void ensure_directory_exists();
public:
	DatabaseManager();
	void connect(std::string str_db_name);
	void create_tables_if_not_exist();
	void insert_initial();

	sqlite3* get_database() { return _db; }
	int get_return_code() { return _i_return_code; }
};

