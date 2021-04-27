#pragma once
#include <vector>
#include <stdexcept>
#include <numeric>
#include <filesystem>
#include "sqlite3.h"
#include "Purchase.h"
#include "PurchaseItem.h"
#include "User.h"

/// <summary>
/// Class that is used to perform operations related to purchases
/// </summary>
class PurchaseManager
{
	sqlite3* _db;

	std::vector<Purchase> _vec_purchases;
	std::filesystem::path _saves_path = std::filesystem::path(L"saves");
public:
	PurchaseManager(sqlite3* db) { _db = db; }

	std::vector<Purchase>& get_vec_purchases() { return _vec_purchases; }

	/// <summary>
	/// Gets all of the purchases made by a user from the database
	/// </summary>
	/// <param name="obj_user"></param>
	void fetch_purchases(User& obj_user);

	/// <summary>
	/// For a provided Purchase, fetches the purchase items of this purchase and stores them in the original referenced Purchase
	/// </summary>
	/// <param name="obj_purchase"></param>
	void populate_purchase_details(Purchase& obj_purchase);

	/// <summary>
	/// Gets the total of all the purchase totals currently stored within the object
	/// </summary>
	/// <returns></returns>
	double get_purchase_grand_total();

	/// <summary>
	/// Gets the average of all the purchase totals currenlty stored within the project
	/// </summary>
	/// <returns></returns>
	double get_purchase_average();

	/// <summary>
	/// Get the total number of game copies currently within the purchases
	/// </summary>
	/// <returns></returns>
	int get_total_game_copies();

	std::filesystem::path get_saves_path() { return _saves_path; }

	/// <summary>
	/// Ensures that the saves path exists when exporting purchase related summaries, should be called before writing any files.
	/// </summary>
	void ensure_save_directory_exists();
};

