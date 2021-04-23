#pragma once
#include <vector>
#include <stdexcept>
#include "sqlite3.h"
#include "Purchase.h"
#include "PurchaseItem.h"
#include "User.h"

class PurchaseManager
{
	sqlite3* _db;

	std::vector<Purchase> _vec_purchases;
	Purchase _obj_purchase_details;
public:
	PurchaseManager(sqlite3* db) { _db = db; }

	std::vector<Purchase>& get_vec_purchases() { return _vec_purchases; }
	void fetch_purchases(User& obj_user);

	void populate_purchase_details(Purchase& obj_purchase);
};

