#pragma once
#include <iostream>
#include <iomanip>
#include <conio.h>
#include <vector>
#include "Game.h"
#include "PurchaseItem.h"
#include "User.h"

namespace util {
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

	void output_games_header();
	void output_game(Game& obj_game);

	void output_basket_header();
	void output_basket_item(PurchaseItem& obj_purchase_item);

	void output_users_header();
	void output_user(User& obj_user);

	void pause();
}


