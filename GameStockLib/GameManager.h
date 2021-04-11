#pragma once
#include <vector>
#include <stdexcept>
#include "sqlite3.h"
#include "Game.h"

class GameManager
{
	sqlite3* _db;
	std::vector<Game> _vec_games;
public:
	GameManager(sqlite3* db) { _db = db; }

	void get_games();

	std::vector<Game>& get_vec_games() { return _vec_games; }
};

