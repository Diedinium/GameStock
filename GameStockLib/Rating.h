#pragma once
#include <string>

/// <summary>
/// Class used to store and represent Ratings
/// </summary>
class Rating {
	int _i_id;
	std::string _str_rating;
public:
	Rating();
	Rating(std::string);
	Rating(int, std::string);

	int get_id() { return _i_id; }
	void set_id(int i_id) { _i_id = i_id; }

	std::string get_rating() { return _str_rating; }
	void set_rating(std::string str_rating) { _str_rating = str_rating; }
};

