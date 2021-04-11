#include "Rating.h"

Rating::Rating() {
	_i_id = 0;
	_str_rating = "";
}

Rating::Rating(std::string str_rating) {
	_i_id = 0;
	_str_rating = str_rating;
}

Rating::Rating(int i_id, std::string str_rating) {
	_i_id = i_id;
	_str_rating = str_rating;
}