#pragma once
#include <vector>
#include <stdexcept>
#include <numeric>
#include <filesystem>
#include "sqlite3.h"
#include "Purchase.h"
#include "PurchaseItem.h"
#include "User.h"

class PurchaseManager
{
	sqlite3* _db;

	std::vector<Purchase> _vec_purchases;
	Purchase _obj_purchase_details;
	std::filesystem::path _saves_path = std::filesystem::path(L"saves");
public:
	PurchaseManager(sqlite3* db) { _db = db; }

	std::vector<Purchase>& get_vec_purchases() { return _vec_purchases; }
	void fetch_purchases(User& obj_user);

	void populate_purchase_details(Purchase& obj_purchase);

	double get_purchase_grand_total();
	double get_purchase_average();
	int get_total_game_copies();

	std::filesystem::path get_saves_path() { return _saves_path; }
	void ensure_save_directory_exists();
};

