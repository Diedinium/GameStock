#pragma once
#include <string>
#include "Genre.h"
#include "Rating.h"

class Game
{
	int _i_id;
	std::string _str_name;
	Genre _obj_genre;
	Rating _obj_rating;
	double _d_price;
	int _i_copies;
public:
	Game();
	Game(std::string str_name, Genre obj_genre, Rating obj_rating);
	Game(std::string str_name, Genre obj_genre, Rating obj_rating, double d_price, int i_copies);
	Game(int i_id, std::string str_name, Genre obj_genre, Rating obj_rating, double d_price, int i_copies);

	int get_id() { return _i_id; }
	void set_id(int i_id) { _i_id = i_id; }

	std::string get_name() { return _str_name; }
	void set_name(std::string str_name) { _str_name = str_name; }

	Genre& get_genre() { return _obj_genre; }
	void set_genre(Genre obj_genre) { _obj_genre = obj_genre; }

	Rating& get_rating() { return _obj_rating; }
	void set_rating(Rating obj_rating) { _obj_rating = obj_rating; }

	double get_price() { return _d_price; }
	void set_price(double d_price) { _d_price = d_price; }

	int get_copies() { return _i_copies; }
	void set_copies(int i_copies) { _i_copies = i_copies; }
};



