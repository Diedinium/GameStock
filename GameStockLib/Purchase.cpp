#include "Purchase.h"

Purchase::Purchase() {
	_i_id = 0;
	_i_user_id = 0;
	_str_date = "";
}

Purchase::Purchase(int i_id, double d_total, std::string str_date) {
	_i_id = i_id;
	_d_total = d_total;
	_str_date = str_date;
}

Purchase::Purchase(int i_user_id, std::vector<PurchaseItem> vec_purchase_items) {
	_i_id = 0;
	_i_user_id = i_user_id;
	_vec_purchase_items = vec_purchase_items;
	_str_date = "";
}

Purchase::Purchase(int i_id, int i_user_id, std::vector<PurchaseItem> vec_purchase_items, std::string str_date) {
	_i_id = i_id;
	_i_user_id = i_user_id;
	_vec_purchase_items = vec_purchase_items;
	_str_date = str_date;
}

int Purchase::get_total_game_copies() {
	return std::accumulate(_vec_purchase_items.begin(), _vec_purchase_items.end(), 0, [](int total, PurchaseItem& purchase_item) {
		return total + purchase_item.get_count();
	});
}