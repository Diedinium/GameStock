#include "DatabaseManager.h"

DatabaseManager::DatabaseManager() {
	_i_return_code = 0;
	ensure_directory_exists();
}

void DatabaseManager::ensure_directory_exists() {
	if (!std::filesystem::exists(_database_path)) {
		std::filesystem::create_directory(_database_path);
	}
}

void DatabaseManager::create_database(std::string str_db_name) {
	std::filesystem::path db_path = _database_path /= str_db_name;
	_i_return_code = sqlite3_open(db_path.string().c_str(), &_db);
}

void DatabaseManager::create_tables_if_not_exist() {
	char* errorMessage;

	std::string str_create_sql =
		"CREATE TABLE IF NOT EXISTS games(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, name TEXT NOT NULL, genre_id INTEGER NOT NULL REFERENCES genres(id) ON DELETE CASCADE, age_rating INTEGER NOT NULL REFERENCES ratings(id) ON DELETE CASCADE, price REAL NOT NULL, copies INTEGER NOT NULL DEFAULT(0));" \
		"CREATE TABLE IF NOT EXISTS genres(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, genre TEXT NOT NULL UNIQUE);" \
		"CREATE TABLE IF NOT EXISTS purchase_items(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, purchase_id INTEGER REFERENCES purchases(id) ON DELETE CASCADE NOT NULL, game_id INTEGER REFERENCES games(id) ON DELETE CASCADE NOT NULL, count INTEGER NOT NULL, price REAL NOT NULL, total REAL NOT NULL AS(count * price) STORED);" \
		"CREATE TABLE IF NOT EXISTS purchases(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, user_id INTEGER REFERENCES users(id) ON DELETE CASCADE NOT NULL, total REAL NOT NULL, date TEXT NOT NULL);" \
		"CREATE TABLE IF NOT EXISTS ratings(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, rating TEXT UNIQUE NOT NULL);" \
		"CREATE TABLE IF NOT EXISTS status(is_init BOOLEAN NOT NULL DEFAULT(0));" \
		"CREATE TABLE IF NOT EXISTS users(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, name TEXT NOT NULL, age INTEGER NOT NULL, email TEXT UNIQUE NOT NULL, password TEXT NOT NULL, is_admin BOOLEAN NOT NULL DEFAULT(0));";

	_i_return_code = sqlite3_exec(_db, str_create_sql.c_str(), NULL, NULL, &errorMessage);
}

void DatabaseManager::insert_initial() {
	char* errorMessage;
	sqlite3_stmt* stmt_status;

	std::string str_status_sql = "SELECT * FROM status;";

	sqlite3_prepare_v2(_db, str_status_sql.c_str(), -1, &stmt_status, NULL);
	_i_return_code = sqlite3_step(stmt_status);
	sqlite3_finalize(stmt_status);

	if (_i_return_code == SQLITE_DONE) {
		std::string str_insert_sql =
			"INSERT INTO genres(id, genre) VALUES(1, 'Strategy');" \
			"INSERT INTO genres(id, genre) VALUES(2, 'Action');" \
			"INSERT INTO genres(id, genre) VALUES(3, 'FPS');" \
			"INSERT INTO genres(id, genre) VALUES(4, 'Romance');" \
			"INSERT INTO genres(id, genre) VALUES(5, 'Horror');" \
			"INSERT INTO genres(id, genre) VALUES(6, 'MMORPG');" \
			"INSERT INTO genres(id, genre) VALUES(7, 'Battle Royale');" \
			"INSERT INTO genres(id, genre) VALUES(8, 'RPG');" \
			"INSERT INTO genres(id, genre) VALUES(9, 'Tower Defence');" \
			"INSERT INTO ratings(id, rating) VALUES(1, '18');" \
			"INSERT INTO ratings(id, rating) VALUES(2, '16');" \
			"INSERT INTO ratings(id, rating) VALUES(3, '12');" \
			"INSERT INTO ratings(id, rating) VALUES(4, '7');" \
			"INSERT INTO ratings(id, rating) VALUES(5, '3');" \
			"INSERT INTO ratings(id, rating) VALUES(6, 'PG');" \
			"INSERT INTO users(id, name, age, email, password, is_admin) VALUES(1, 'Admin', 0, 'admin@gamestock.com', 'somesecureadminpassword', 1);" \
			"INSERT INTO status(is_init) VALUES(1);";

		_i_return_code = sqlite3_exec(_db, str_insert_sql.c_str(), NULL, NULL, &errorMessage);
	}
}