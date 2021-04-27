#pragma once
#include <iostream>
#include "sqlite3.h"
#include <filesystem>

/// <summary>
/// Class that contains any database management related functions; such as initialising the database structure, inserting initial data etc
/// </summary>
class DatabaseManager
{
	sqlite3* _db;
	int _i_return_code;
	std::filesystem::path _database_path = std::filesystem::path(L"database");
	/// <summary>
	/// Creates the .\database directory if it does not yet exist
	/// </summary>
	void ensure_directory_exists();
public:
	DatabaseManager();

	/// <summary>
	/// Opens a connection to the specified database name using the name provided, cannot create paths, so runs ensure_directory_exists first to ensure database directory exists as well
	/// </summary>
	/// <param name="str_db_name"></param>
	void connect(std::string str_db_name);

	/// <summary>
	/// Runs operation against the database that creates the database structure (tables, relationships etc) if they do not yet exist.
	/// </summary>
	void create_tables_if_not_exist();

	/// <summary>
	/// Inserts the initial data, such as test users and games to test with.
	/// </summary>
	void insert_initial();

	/// <summary>
	/// Returns pointer to the database that was connected to
	/// </summary>
	/// <returns></returns>
	sqlite3* get_database() { return _db; }

	/// <summary>
	/// Database operations in this class store the result within the class, this retrieves the result
	/// </summary>
	/// <returns></returns>
	int get_return_code() { return _i_return_code; }
};

