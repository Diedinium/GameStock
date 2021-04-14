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