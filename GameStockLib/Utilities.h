#pragma once
#include <iostream>
#include <iomanip>
#include <conio.h>
#include <vector>
#include "Game.h"

namespace util {
	template <typename IteratorT, typename FunctionT>
	FunctionT for_each_iterator(IteratorT first, IteratorT last, int initial, FunctionT func) {
		for (; first != last; ++first, ++initial)
			func(initial, *first);
		return func;
	}

	void output_games_header();
	void output_game(Game&);

	void pause();
}


