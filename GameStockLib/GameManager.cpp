#include "GameManager.h"

void GameManager::get_games() {
	_vec_games.clear();
	int i_return_code;
	sqlite3_stmt* stmt_games;

	std::string str_status_sql = "SELECT g.id, g.name, r.id as rating_id, r.rating, x.id as genre_id, x.genre, g.price, g.copies FROM games AS g LEFT JOIN ratings AS r on g.genre_id = r.id LEFT JOIN genres as x ON g.genre_id = x.id WHERE g.copies > 0";

	sqlite3_prepare_v2(_db, str_status_sql.c_str(), -1, &stmt_games, NULL);
	while ((i_return_code = sqlite3_step(stmt_games)) == SQLITE_ROW) {
		Game obj_game = Game(
			sqlite3_column_int(stmt_games, 0),
			(char*)sqlite3_column_text(stmt_games, 1),
			Genre(sqlite3_column_int(stmt_games, 4), (char*)sqlite3_column_text(stmt_games, 5)),
			Rating(sqlite3_column_int(stmt_games, 2), (char*)sqlite3_column_text(stmt_games, 3)),
			sqlite3_column_double(stmt_games, 6),
			sqlite3_column_int(stmt_games, 7));

		_vec_games.push_back(obj_game);
	}
	if (i_return_code != SQLITE_DONE) {
		throw std::runtime_error("Something went wrong while fetching games.");
	}

	sqlite3_finalize(stmt_games);
}