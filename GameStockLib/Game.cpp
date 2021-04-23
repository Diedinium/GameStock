#include "Game.h"

Game::Game() {
	_i_id = 0;
	_str_name = "";
	_d_price = 0;
	_i_copies = 0;
}

Game::Game(std::string str_name, Genre obj_genre, Rating obj_rating) {
	_i_id = 0;
	_str_name = str_name;
	_obj_genre = obj_genre;
	_obj_rating = obj_rating;
	_d_price = 0;
	_i_copies = 0;
}

Game::Game(std::string str_name, Genre obj_genre, Rating obj_rating, double d_price, int i_copies) {
	_i_id = 0;
	_str_name = str_name;
	_obj_genre = obj_genre;
	_obj_rating = obj_rating;
	_d_price = d_price;
	_i_copies = i_copies;
}

Game::Game(int i_id, std::string str_name, Genre obj_genre, Rating obj_rating, double d_price, int i_copies) {
	_i_id = i_id;
	_str_name = str_name;
	_obj_genre = obj_genre;
	_obj_rating = obj_rating;
	_d_price = d_price;
	_i_copies = i_copies;
}