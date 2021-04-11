#include "Genre.h"

Genre::Genre() {
	_i_id = 0;
	_str_genre = "";
}

Genre::Genre(std::string str_genre) {
	_i_id = 0;
	_str_genre = str_genre;
}

Genre::Genre(int i_id, std::string str_genre) {
	_i_id = i_id;
	_str_genre = str_genre;
}