#pragma once
#include <vector>
#include <algorithm>
#include <stdexcept>
#include "sqlite3.h"
#include "Game.h"
#include "Purchase.h"
#include "PurchaseItem.h"

class GameManager
{
	sqlite3* _db;
	std::vector<Game> _vec_games;
	Purchase _obj_basket;
	bool _bool_initialised = false;
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
};

