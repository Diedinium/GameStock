#include "Utilities.h"

void util::pause() {
    std::cout << "Press any key to continue...\n";
    auto value = _getch();
}

void util::output_games_header() {
    std::cout << "-------------------------------------------------------------------------------------------------------------\n";
    std::cout << std::setw(45) << std::left << "Name" << std::setw(20) << std::left << "Genre" << std::setw(7) << std::left << "Rating" << std::setw(7) << std::left << "Price" << std::setw(9) << std::left << "Copies" << "\n";
    std::cout << "-------------------------------------------------------------------------------------------------------------\n";
}

void util::output_game(Game& obj_game) {
		std::cout.precision(2);
		std::cout
			<< std::fixed
			<< std::setw(45) << std::left << obj_game.get_name()
			<< std::setw(20) << std::left << obj_game.get_genre().get_genre()
			<< std::setw(7) << std::left << obj_game.get_rating().get_rating()
			<< std::setw(9) << std::left << obj_game.get_price()
			<< std::setw(7) << std::left << obj_game.get_copies() << "\n";
}