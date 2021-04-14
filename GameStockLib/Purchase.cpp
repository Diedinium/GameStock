#include "Purchase.h"

Purchase::Purchase() {
	_i_id = 0;
	_i_user_id = 0;
	_str_date = "";
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