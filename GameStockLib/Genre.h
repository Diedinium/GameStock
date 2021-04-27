#pragma once
#include <string>

/// <summary>
/// Class that is used to represent a genre
/// </summary>
class Genre {
	int _i_id;
	std::string _str_genre;
public:
	Genre();
	Genre(std::string str_genre);
	Genre(int i_id, std::string str_genre);

	int get_id() { return _i_id; }
	void set_id(int i_id) { _i_id = i_id; }

	std::string get_genre() { return _str_genre; }
	void set_genre(std::string str_genre) { _str_genre = str_genre; }
};

