#pragma once
#include <iostream>
#include <iomanip>
#include <conio.h>
#include <vector>
#include <chrono>
#include <fstream>
#include "Game.h"
#include "Purchase.h"
#include "PurchaseItem.h"
#include "User.h"

/// <summary>
/// Namespace used to contain all utility related functions, such as calculation templates, or 
/// </summary>
namespace util {
	/// <summary>
	/// Foreach operator that also contains an iterator.
	/// Created simply because I'm lazy.
	/// </summary>
	/// <typeparam name="IteratorT"></typeparam>
	/// <typeparam name="FunctionT"></typeparam>
	/// <param name="first"></param>
	/// <param name="last"></param>
	/// <param name="initial"></param>
	/// <param name="func"></param>
	/// <returns></returns>
	template <typename IteratorT, typename FunctionT>
	FunctionT for_each_iterator(IteratorT first, IteratorT last, int initial, FunctionT func) {
		for (; first != last; ++first, ++initial)
			func(initial, *first);
		return func;
	}

	/// <summary>
	/// Returns bool on if compare is between min and max.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="min"></param>
	/// <param name="max"></param>
	/// <param name="compare"></param>
	/// <returns></returns>
	template <typename T>
	bool is_between(T min, T max, T compare) {
		return compare >= min && compare <= max;
	}

	/// <summary>
	/// Returns true/false on if number is less than or equal to compare.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="number"></param>
	/// <param name="compare"></param>
	/// <returns></returns>
	template <typename T>
	bool is_less_than_or_equal_to(T number, T compare) {
		return number <= compare;
	}

	/// <summary>
	/// Returns bool on if number is more than or equal to compare
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="number"></param>
	/// <param name="compare"></param>
	/// <returns></returns>
	template <typename T>
	bool is_more_than_or_equal_to(T number, T compare) {
		return number >= compare;
	}

	/// <summary>
	/// Returns bool on if number is more than compare.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="number"></param>
	/// <param name="compare"></param>
	/// <returns>bool</returns>
	template <typename T>
	bool is_more_than(T number, T compare) {
		return number > compare;
	}

	/// <summary>
	/// Returns bool on if number is less than compare
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="number"></param>
	/// <param name="compare"></param>
	/// <returns></returns>
	template <typename T>
	bool is_less_than(T number, T compare) {
		return number < compare;
	}

	/// <summary>
	/// Outputs the header for displaying the game table
	/// </summary>
	void output_games_header();

	/// <summary>
	/// Outputs an individual game item (row) for the game table
	/// </summary>
	/// <param name="obj_game"></param>
	void output_game(Game& obj_game);

	/// <summary>
	/// Outputs the header for displaying the basket table
	/// </summary>
	void output_basket_header();

	/// <summary>
	/// Outputs an individual basket item for the basket table
	/// </summary>
	/// <param name="obj_purchase_item"></param>
	void output_basket_item(PurchaseItem& obj_purchase_item);

	/// <summary>
	/// Outputs the header for displaying the user table
	/// </summary>
	void output_users_header(bool display_admin_status = true);

	/// <summary>
	/// Outputs an individual user for the user table
	/// </summary>
	/// <param name="obj_user"></param>
	void output_user(User& obj_user, bool display_admin_status = true);

	/// <summary>
	/// Outputs the header for displaying the purchase table
	/// </summary>
	void output_purchase_header();

	/// <summary>
	/// Outputs an indivual purchase for the purchase table
	/// </summary>
	/// <param name="obj_purchase"></param>
	void output_purchase(Purchase& obj_purchase);

	/// <summary>
	/// Outputs the header for displaying the purchase item/details table
	/// </summary>
	void output_purchase_item_header();
	void output_purchase_item_header(std::ofstream& of_stream);

	/// <summary>
	/// Outputs an individual purchase item (row) for the purchase item/detail table
	/// </summary>
	/// <param name="obj_purchase_item"></param>
	void output_purchase_item(PurchaseItem& obj_purchase_item);
	void output_purchase_item(PurchaseItem& obj_purchase_item, std::ofstream& of_stream);

	/// <summary>
	/// Get the current (system) datetime as the std::tm struct
	/// </summary>
	/// <returns></returns>
	std::tm get_current_datetime();

	/// <summary>
	/// Convert the provided std::tm to a file safe string format
	/// </summary>
	/// <param name="tm"></param>
	/// <returns></returns>
	std::string tm_to_filesafe_str(std::tm& tm);

	/// <summary>
	/// Used to stop console output until user presses any key
	/// </summary>
	void pause();
}


