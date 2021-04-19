#pragma once
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <numeric>
#include <sstream>
#include <iomanip>
#include "sqlite3.h"
#include "Game.h"
#include "Rating.h"
#include "Genre.h"
#include "Purchase.h"
#include "PurchaseItem.h"

class GameManager
{
	sqlite3* _db;
	std::vector<Game> _vec_games;
	Purchase _obj_basket;
	Genre _obj_filter_genre;
	bool _bool_initialised = false;
	bool _bool_admin_flag = false;
	int get_games();
public:
	GameManager(sqlite3* db) { _db = db; }

	void initialise_games();
	void refresh_games();

	std::vector<Game>& get_vec_games() { return _vec_games; }
	Purchase& get_basket() { return _obj_basket; }

	void set_basket_user(int i_user_id) { _obj_basket.set_user_id(i_user_id); }
	void add_basket_item(PurchaseItem& obj_purhcase_item);
	void remove_basket_item(int i_game_id);
	double get_basket_total();
	void reset_basket();

	bool get_admin_flag() { return _bool_admin_flag; }
	void set_admin_flag(bool bool_admin_flag) { _bool_admin_flag = bool_admin_flag; }

	void set_initialised(bool bool_initialised) { _bool_initialised = bool_initialised; }

	void add_game(Game& obj_game);
	void update_game_name(int i_game_id, std::string str_game_name);
	void update_game_genre(int i_game_id, int i_genre_id);
	void update_game_price(int i_game_id, double d_price);
	void update_game_rating(int i_game_id, int i_rating_id);
	void update_game_copies(int i_game_id, int i_copies);

	const std::vector<Rating> get_ratings();
	const std::vector<Genre> get_genres();

	double make_purchase();

	Genre& get_filter_genre() { return _obj_filter_genre; }
	void set_filter_genre(Genre obj_filter_genre) { _obj_filter_genre = obj_filter_genre; }

	void logout();
};

