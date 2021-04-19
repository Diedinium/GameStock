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

	std::string str_status_sql = "SELECT g.id, g.name, r.id as rating_id, r.rating, x.id as genre_id, x.genre, g.price, g.copies FROM games AS g LEFT JOIN ratings AS r on g.age_rating = r.id LEFT JOIN genres as x ON g.genre_id = x.id WHERE g.copies > 0";

	if (_bool_admin_flag) {
		str_status_sql = "SELECT g.id, g.name, r.id as rating_id, r.rating, x.id as genre_id, x.genre, g.price, g.copies FROM games AS g LEFT JOIN ratings AS r on g.age_rating = r.id LEFT JOIN genres as x ON g.genre_id = x.id";
	}

	if (_obj_filter_genre.get_id() > 0 && _bool_admin_flag) {
		str_status_sql = str_status_sql += " WHERE g.genre_id = " + std::to_string(_obj_filter_genre.get_id());
	}
	else if (_obj_filter_genre.get_id() > 0 && !_bool_admin_flag){
		str_status_sql = str_status_sql += " AND g.genre_id = " + std::to_string(_obj_filter_genre.get_id());
	}

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

double GameManager::get_basket_total() {
	return std::accumulate(_obj_basket.get_vec_purchase_items().begin(), _obj_basket.get_vec_purchase_items().end(), 0.0, [&](double total, PurchaseItem& item) {
		return total + item.get_total();
	});
}

void GameManager::reset_basket() {
	_obj_basket.get_vec_purchase_items().clear();
}

void GameManager::add_game(Game& obj_game) {
	sqlite3_stmt* stmt_insert_game;
	std::string str_insert_game = "INSERT INTO games(name, genre_id, age_rating, price, copies) VALUES (?, ?, ?, ?, ?)";

	if (sqlite3_prepare_v2(_db, str_insert_game.c_str(), -1, &stmt_insert_game, NULL) != SQLITE_OK) {
		std::string str_error_msg = "Failed to prepare insert statement: ";
		str_error_msg = str_error_msg + (char*)sqlite3_errmsg(_db);
		throw new std::runtime_error(str_error_msg);
	}

	sqlite3_bind_text(stmt_insert_game, 1, obj_game.get_name().c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_int(stmt_insert_game, 2, obj_game.get_genre().get_id());
	sqlite3_bind_int(stmt_insert_game, 3, obj_game.get_rating().get_id());
	sqlite3_bind_double(stmt_insert_game, 4, obj_game.get_price());
	sqlite3_bind_int(stmt_insert_game, 5, obj_game.get_copies());

	if (sqlite3_step(stmt_insert_game) != SQLITE_DONE) {
		throw new std::runtime_error("Something went wrong while inserting this game, please try again.");
	}

	sqlite3_finalize(stmt_insert_game);
}

void GameManager::update_game_name(int i_game_id, std::string str_game_name) {
	sqlite3_stmt* stmt_update_name;

	std::string str_update_name_sql = "UPDATE games SET name = ? WHERE id = ?";

	if (sqlite3_prepare_v2(_db, str_update_name_sql.c_str(), -1, &stmt_update_name, NULL) != SQLITE_OK) {
		std::string str_error_msg = "Failed to prepare update statement: ";
		str_error_msg = str_error_msg + (char*)sqlite3_errmsg(_db);
		throw new std::runtime_error(str_error_msg);
	}

	if (sqlite3_bind_text(stmt_update_name, 1, str_game_name.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK) {
		std::string str_error_msg = "Error while binding name: ";
		str_error_msg = str_error_msg + (char*)sqlite3_errmsg(_db);
		throw new std::runtime_error(str_error_msg);
	}

	if (sqlite3_bind_int(stmt_update_name, 2, i_game_id) != SQLITE_OK) {
		std::string str_error_msg = "Error while binding game id: ";
		str_error_msg = str_error_msg + (char*)sqlite3_errmsg(_db);
		throw new std::runtime_error(str_error_msg);
	}

	if (sqlite3_step(stmt_update_name) != SQLITE_DONE) {
		throw new std::runtime_error("Something went wrong while performing this update, please try again.");
	}

	sqlite3_finalize(stmt_update_name);
}

void GameManager::update_game_genre(int i_game_id, int i_genre_id) {
	sqlite3_stmt* stmt_update_genre;

	std::string str_update_name_sql = "UPDATE games SET genre_id = ? WHERE id = ?";

	if (sqlite3_prepare_v2(_db, str_update_name_sql.c_str(), -1, &stmt_update_genre, NULL) != SQLITE_OK) {
		std::string str_error_msg = "Failed to prepare update statement: ";
		str_error_msg = str_error_msg + (char*)sqlite3_errmsg(_db);
		throw new std::runtime_error(str_error_msg);
	}

	if (sqlite3_bind_int(stmt_update_genre, 1, i_genre_id) != SQLITE_OK) {
		std::string str_error_msg = "Error while binding genre: ";
		str_error_msg = str_error_msg + (char*)sqlite3_errmsg(_db);
		throw new std::runtime_error(str_error_msg);
	}

	if (sqlite3_bind_int(stmt_update_genre, 2, i_game_id) != SQLITE_OK) {
		std::string str_error_msg = "Error while binding game id: ";
		str_error_msg = str_error_msg + (char*)sqlite3_errmsg(_db);
		throw new std::runtime_error(str_error_msg);
	}

	if (sqlite3_step(stmt_update_genre) != SQLITE_DONE) {
		throw new std::runtime_error("Something went wrong while performing this update, please try again.");
	}

	sqlite3_finalize(stmt_update_genre);
}

void GameManager::update_game_price(int i_game_id, double d_price) {
	sqlite3_stmt* stmt_update_price;

	std::string str_update_name_sql = "UPDATE games SET price = ? WHERE id = ?";

	if (sqlite3_prepare_v2(_db, str_update_name_sql.c_str(), -1, &stmt_update_price, NULL) != SQLITE_OK) {
		std::string str_error_msg = "Failed to prepare update statement: ";
		str_error_msg = str_error_msg + (char*)sqlite3_errmsg(_db);
		throw new std::runtime_error(str_error_msg);
	}

	if (sqlite3_bind_double(stmt_update_price, 1, d_price) != SQLITE_OK) {
		std::string str_error_msg = "Error while binding price: ";
		str_error_msg = str_error_msg + (char*)sqlite3_errmsg(_db);
		throw new std::runtime_error(str_error_msg);
	}

	if (sqlite3_bind_int(stmt_update_price, 2, i_game_id) != SQLITE_OK) {
		std::string str_error_msg = "Error while binding game id: ";
		str_error_msg = str_error_msg + (char*)sqlite3_errmsg(_db);
		throw new std::runtime_error(str_error_msg);
	}

	if (sqlite3_step(stmt_update_price) != SQLITE_DONE) {
		throw new std::runtime_error("Something went wrong while performing this update, please try again.");
	}

	sqlite3_finalize(stmt_update_price);
}

void GameManager::update_game_rating(int i_game_id, int i_rating_id) {
	sqlite3_stmt* stmt_update_rating;

	std::string str_update_name_sql = "UPDATE games SET age_rating = ? WHERE id = ?";

	if (sqlite3_prepare_v2(_db, str_update_name_sql.c_str(), -1, &stmt_update_rating, NULL) != SQLITE_OK) {
		std::string str_error_msg = "Failed to prepare update statement: ";
		str_error_msg = str_error_msg + (char*)sqlite3_errmsg(_db);
		throw new std::runtime_error(str_error_msg);
	}

	if (sqlite3_bind_int(stmt_update_rating, 1, i_rating_id) != SQLITE_OK) {
		std::string str_error_msg = "Error while binding rating: ";
		str_error_msg = str_error_msg + (char*)sqlite3_errmsg(_db);
		throw new std::runtime_error(str_error_msg);
	}

	if (sqlite3_bind_int(stmt_update_rating, 2, i_game_id) != SQLITE_OK) {
		std::string str_error_msg = "Error while binding game id: ";
		str_error_msg = str_error_msg + (char*)sqlite3_errmsg(_db);
		throw new std::runtime_error(str_error_msg);
	}

	if (sqlite3_step(stmt_update_rating) != SQLITE_DONE) {
		throw new std::runtime_error("Something went wrong while performing this update, please try again.");
	}

	sqlite3_finalize(stmt_update_rating);
}

void GameManager::update_game_copies(int i_game_id, int i_copies) {
	sqlite3_stmt* stmt_update_copies;

	std::string str_update_name_sql = "UPDATE games SET copies = ? WHERE id = ?";

	if (sqlite3_prepare_v2(_db, str_update_name_sql.c_str(), -1, &stmt_update_copies, NULL) != SQLITE_OK) {
		std::string str_error_msg = "Failed to prepare update statement: ";
		str_error_msg = str_error_msg + (char*)sqlite3_errmsg(_db);
		throw new std::runtime_error(str_error_msg);
	}

	if (sqlite3_bind_int(stmt_update_copies, 1, i_copies) != SQLITE_OK) {
		std::string str_error_msg = "Error while binding copies: ";
		str_error_msg = str_error_msg + (char*)sqlite3_errmsg(_db);
		throw new std::runtime_error(str_error_msg);
	}

	if (sqlite3_bind_int(stmt_update_copies, 2, i_game_id) != SQLITE_OK) {
		std::string str_error_msg = "Error while binding game id: ";
		str_error_msg = str_error_msg + (char*)sqlite3_errmsg(_db);
		throw new std::runtime_error(str_error_msg);
	}

	if (sqlite3_step(stmt_update_copies) != SQLITE_DONE) {
		throw new std::runtime_error("Something went wrong while performing this update, please try again.");
	}

	sqlite3_finalize(stmt_update_copies);
}

const std::vector<Rating> GameManager::get_ratings() {
	std::vector<Rating> vec_ratings;

	sqlite3_stmt* stmt_ratings;

	std::string str_sql = "SELECT * FROM ratings";

	sqlite3_prepare_v2(_db, str_sql.c_str(), -1, &stmt_ratings, NULL);
	while (sqlite3_step(stmt_ratings) == SQLITE_ROW) {
		vec_ratings.push_back(Rating(sqlite3_column_int(stmt_ratings, 0), (char*)sqlite3_column_text(stmt_ratings, 1)));
	}

	sqlite3_finalize(stmt_ratings);
	return vec_ratings;
}

const std::vector<Genre> GameManager::get_genres() {
	std::vector<Genre> vec_genres;

	sqlite3_stmt* stmt_genres;

	std::string str_sql = "SELECT * FROM genres ORDER BY genre";

	sqlite3_prepare_v2(_db, str_sql.c_str(), -1, &stmt_genres, NULL);
	while (sqlite3_step(stmt_genres) == SQLITE_ROW) {
		vec_genres.push_back(Genre(sqlite3_column_int(stmt_genres, 0), (char*)sqlite3_column_text(stmt_genres, 1)));
	}

	sqlite3_finalize(stmt_genres);
	return vec_genres;
}

double GameManager::make_purchase() {
	char* errorMessage;
	double d_grand_total = get_basket_total();

	std::stringstream ss;
	ss.setf(std::ios::fixed);
	ss << "INSERT INTO purchases(user_id, total) VALUES (" << _obj_basket.get_user_id() << ", " << std::setprecision(2) << d_grand_total << ")";
	std::string temp = ss.str();

	if (sqlite3_exec(_db, ss.str().c_str(), NULL, NULL, &errorMessage) != SQLITE_OK) {
		throw new std::runtime_error(sqlite3_errmsg(_db));
	}

	int i_purchase_id = (int)sqlite3_last_insert_rowid(_db);

	sqlite3_stmt* stmt_insert_purchase_item;
	std::string str_insert_purchase_item = "INSERT INTO purchase_items(purchase_id, game_name, game_price, game_genre, game_rating, count) VALUES (?, ?, ?, ?, ?, ?)";

	if (sqlite3_prepare_v2(_db, str_insert_purchase_item.c_str(), -1, &stmt_insert_purchase_item, NULL) != SQLITE_OK) {
		std::string str_error_msg = "Failed to prepare insert statement: ";
		str_error_msg = str_error_msg + (char*)sqlite3_errmsg(_db);
		throw new std::runtime_error(str_error_msg);
	}

	for (auto& item : _obj_basket.get_vec_purchase_items()) {
		sqlite3_bind_int(stmt_insert_purchase_item, 1, i_purchase_id);
		sqlite3_bind_text(stmt_insert_purchase_item, 2, item.get_game().get_name().c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_double(stmt_insert_purchase_item, 3, item.get_price());
		sqlite3_bind_text(stmt_insert_purchase_item, 4, item.get_game().get_genre().get_genre().c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(stmt_insert_purchase_item, 5, item.get_game().get_rating().get_rating().c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_int(stmt_insert_purchase_item, 6, item.get_count());

		if (sqlite3_step(stmt_insert_purchase_item) != SQLITE_DONE) {
			throw new std::runtime_error("Something went wrong while performing an insert, please try again.");
		}

		sqlite3_reset(stmt_insert_purchase_item);
	}

	sqlite3_finalize(stmt_insert_purchase_item);

	sqlite3_stmt* stmt_update_game_copies;
	std::string str_update_game_copies = "UPDATE games SET copies = ? WHERE id = ?";

	if (sqlite3_prepare_v2(_db, str_update_game_copies.c_str(), -1, &stmt_update_game_copies, NULL) != SQLITE_OK) {
		std::string str_error_msg = "Failed to prepare insert statement: ";
		str_error_msg = str_error_msg + (char*)sqlite3_errmsg(_db);
		throw new std::runtime_error(str_error_msg);
	}

	for (auto& item : _obj_basket.get_vec_purchase_items()) {
		sqlite3_bind_int(stmt_update_game_copies, 1, item.get_game().get_copies() - item.get_count());
		sqlite3_bind_int(stmt_update_game_copies, 2, item.get_game().get_id());

		if (sqlite3_step(stmt_update_game_copies) != SQLITE_DONE) {
			throw new std::runtime_error("Something went wrong while performing an update, please try again.");
		}

		sqlite3_reset(stmt_update_game_copies);
	}

	sqlite3_finalize(stmt_update_game_copies);

	return d_grand_total;
}

void GameManager::logout() {
	set_initialised(false);
	set_filter_genre(Genre());
	reset_basket();
}