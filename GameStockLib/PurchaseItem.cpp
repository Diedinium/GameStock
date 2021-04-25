#include "PurchaseItem.h"

PurchaseItem::PurchaseItem(int i_game_id, int i_count, double d_price) {
	_i_id = 0;
	_i_purchase_id = 0;
	_i_game_id = i_game_id;
	_i_count = i_count;
	_d_price = d_price;
	_d_total = (double)_i_count * _d_price;
}

PurchaseItem::PurchaseItem(int i_game_id, Game obj_game, int i_count, double d_price) {
	_i_id = 0;
	_i_purchase_id = 0;
	_i_game_id = i_game_id;
	_obj_game = obj_game;
	_i_count = i_count;
	_d_price = d_price;
	_d_total = (double)_i_count * _d_price;
}

PurchaseItem::PurchaseItem(int i_id, int i_purchase_id, int i_game_id, Game obj_game, int i_count, double d_price) {
	_i_id = i_id;
	_i_purchase_id = i_purchase_id;
	_i_game_id = i_game_id;
	_obj_game = obj_game;
	_i_count = i_count;
	_d_price = d_price;
	_d_total = (double)_i_count * _d_price;
}

PurchaseItem::PurchaseItem(int i_id, std::string str_game_name, double d_game_price, std::string str_game_genre, std::string str_game_rating, int i_count, double d_total) {
	_i_id = i_id;
	_i_game_id = 0;
	_i_purchase_id = 0;
	_obj_game = Game(str_game_name, Genre(str_game_genre), Rating(str_game_rating));
	_i_count = i_count;
	_d_price = d_game_price;
	_d_total = d_total;
}

void PurchaseItem::set_count(int i_count)
{
	_i_count = i_count; 
	_d_total = (double)_i_count * _d_price;
}