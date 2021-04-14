#include "GameManager.h"

void GameManager::initialise_games() {
	if (!_bool_initialised) {
		if (get_games() != SQLITE_DONE) {
			throw new std::runtime_error("Something went wrong while initialising games.");
		}
		_bool_initialised = true;
	}
}

void GameManager::refresh_games() {
	if (get_games() != SQLITE_DONE) {
		throw new std::runtime_error("Something went wrong while updating games.");
	}
}

int GameManager::get_games() {
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

	sqlite3_finalize(stmt_games);
	return i_return_code;
}

void GameManager::add_basket_item(PurchaseItem& obj_purchase_item) { 
	auto position = std::find_if(_obj_basket.get_vec_purchase_items().begin(), _obj_basket.get_vec_purchase_items().end(), [&obj_purchase_item](PurchaseItem& obj) { return obj.get_game_id() == obj_purchase_item.get_game_id(); });

	if (position != _obj_basket.get_vec_purchase_items().end()) {
		auto& obj_current_game = _obj_basket.get_vec_purchase_items().at(std::distance(_obj_basket.get_vec_purchase_items().begin(), position));
		obj_current_game.set_count(obj_current_game.get_count() + obj_purchase_item.get_count());
	}
	else {
		_obj_basket.get_vec_purchase_items().push_back(obj_purchase_item);
	}

	auto game_position = std::find_if(_vec_games.begin(), _vec_games.end(), [&](Game& obj) { return obj.get_id() == obj_purchase_item.get_game_id(); });

	if (game_position != _vec_games.end()) {
		Game& obj_game = _vec_games.at(std::distance(_vec_games.begin(), game_position));
		obj_game.set_copies(obj_game.get_copies() - obj_purchase_item.get_count());
	}
}

void GameManager::remove_basket_item(int i_game_id) {
	auto position = std::find_if(_obj_basket.get_vec_purchase_items().begin(), _obj_basket.get_vec_purchase_items().end(), [&i_game_id](PurchaseItem& obj) { return obj.get_game_id() == i_game_id; });

	if (position != _obj_basket.get_vec_purchase_items().end()) {
		PurchaseItem obj_purchase_item = _obj_basket.get_vec_purchase_items().at(std::distance(_obj_basket.get_vec_purchase_items().begin(), position));

		auto game_position = std::find_if(_vec_games.begin(), _vec_games.end(), [&i_game_id](Game& obj) { return obj.get_id() == i_game_id; });

		if (game_position != _vec_games.end()) {
			Game& obj_game = _vec_games.at(std::distance(_vec_games.begin(), game_position));
			obj_game.set_copies(obj_game.get_copies() + obj_purchase_item.get_count());
		}

		_obj_basket.get_vec_purchase_items().erase(position);
	}
	else {
		throw std::invalid_argument("Cannot remove game with id of " + std::to_string(i_game_id) + " from basket, item not found in basket.");
	}
}