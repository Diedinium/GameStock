#include "Utilities.h"

void util::pause() {
    std::cout << "Press any key to continue...\n";
    auto value = _getch();
}

void util::output_games_header() {
    std::cout << "------------------------------------------------------------------------------------------\n";
    std::cout << std::setw(45) << std::left << "Name" << std::setw(20) << std::left << "Genre" << std::setw(9) << std::left << "Rating" << std::setw(9) << std::left << "Price" << std::setw(9) << std::left << "Copies" << "\n";
    std::cout << "------------------------------------------------------------------------------------------\n";
}

void util::output_game(Game& obj_game) {
		std::cout.precision(2);
		std::cout
			<< std::fixed
			<< std::setw(45) << std::left << obj_game.get_name()
			<< std::setw(20) << std::left << obj_game.get_genre().get_genre()
			<< std::setw(9) << std::left << obj_game.get_rating().get_rating()
			<< std::setw(9) << std::left << obj_game.get_price()
			<< std::setw(7) << std::left << obj_game.get_copies() << "\n";
}

void util::output_basket_header() {
	std::cout << "-----------------------------------------------------------------------------------\n";
	std::cout << std::setw(45) << std::left << "Game" << std::setw(7) << std::left << "Copies" << std::setw(8) << std::left << "Price" << std::setw(9) << std::left << "Total" << std::setw(14) << std::left << "Total (No VAT)" << "\n";
	std::cout << "-----------------------------------------------------------------------------------\n";
}

void util::output_basket_item(PurchaseItem& obj_purchase_item) {
	std::cout.precision(2);
	std::cout
		<< std::fixed
		<< std::setw(45) << std::left << obj_purchase_item.get_game().get_name()
		<< std::setw(7) << std::left << obj_purchase_item.get_count()
		<< std::setw(8) << std::left << obj_purchase_item.get_price()
		<< std::setw(9) << std::left << obj_purchase_item.get_total()
		<< std::setw(14) << std::left << obj_purchase_item.get_total_before_vat() << "\n";
}

void util::output_users_header(bool display_admin_status) {
	if (display_admin_status) {
		std::cout << "--------------------------------------------------------------------------------------------\n";
		std::cout << std::setw(30) << std::left << "Name" << std::setw(6) << std::left << "Age" << std::setw(45) << std::left << "Email" << std::setw(11) << std::left << "Is Admin?" << "\n";
		std::cout << "--------------------------------------------------------------------------------------------\n";
	}
	else {
		std::cout << "---------------------------------------------------------------------------------\n";
		std::cout << std::setw(30) << std::left << "Name" << std::setw(6) << std::left << "Age" << std::setw(45) << std::left << "Email" << "\n";
		std::cout << "---------------------------------------------------------------------------------\n";
	}
}

void util::output_user(User& obj_user, bool display_admin_status) {
	if (display_admin_status) {
		std::cout.precision(2);
		std::cout
			<< std::fixed
			<< std::setw(30) << std::left << obj_user.get_full_name()
			<< std::setw(6) << std::left << obj_user.get_age()
			<< std::setw(45) << std::left << obj_user.get_email()
			<< std::setw(11) << std::left << (obj_user.get_is_admin() ? "True" : "False") << "\n";
	}
	else {
		std::cout.precision(2);
		std::cout
			<< std::fixed
			<< std::setw(30) << std::left << obj_user.get_full_name()
			<< std::setw(6) << std::left << obj_user.get_age()
			<< std::setw(45) << std::left << obj_user.get_email() << "\n";
	}
}

void util::output_purchase_header() {
	std::cout << "---------------------------------------------\n";
	std::cout << std::setw(25) << std::left << "Purchase date" << std::setw(20) << std::left << "Total" << "\n";
	std::cout << "---------------------------------------------\n";
}

void util::output_purchase(Purchase& obj_purchase) {
	std::cout.precision(2);
	std::cout
		<< std::fixed
		<< std::setw(25) << std::left << obj_purchase.get_date()
		<< std::setw(20) << std::left << obj_purchase.get_total() << "\n";
}

void util::output_purchase_item_header() {
	std::cout << "-------------------------------------------------------------------------------------------------------\n";
	std::cout << std::setw(45) << std::left << "Game Name" << std::setw(9) << std::left << "Copies" << std::setw(15) << std::left << "Game cost" << std::setw(15) << std::left << "Total" << std::setw(20) << std::left << "Total (before VAT)" << "\n";
	std::cout << "-------------------------------------------------------------------------------------------------------\n";
}

void util::output_purchase_item_header(std::ofstream& of_stream) {
	of_stream << "-------------------------------------------------------------------------------------------------------\n";
	of_stream << std::setw(45) << std::left << "Game Name" << std::setw(9) << std::left << "Copies" << std::setw(15) << std::left << "Game cost" << std::setw(15) << std::left << "Total" << std::setw(20) << std::left << "Total (before VAT)" << "\n";
	of_stream << "-------------------------------------------------------------------------------------------------------\n";
}

void util::output_purchase_item(PurchaseItem& obj_purchase_item) {
	std::cout.precision(2);
	std::cout
		<< std::fixed
		<< std::setw(45) << std::left << obj_purchase_item.get_game().get_name()
		<< std::setw(9) << std::left << obj_purchase_item.get_count()
		<< std::setw(15) << std::left << obj_purchase_item.get_price()
		<< std::setw(15) << std::left << obj_purchase_item.get_total()
		<< std::setw(20) << std::left << obj_purchase_item.get_total_before_vat() << "\n";
}

void util::output_purchase_item(PurchaseItem& obj_purchase_item, std::ofstream& of_stream) {
	of_stream.precision(2);
	of_stream
		<< std::fixed
		<< std::setw(45) << std::left << obj_purchase_item.get_game().get_name()
		<< std::setw(9) << std::left << obj_purchase_item.get_count()
		<< std::setw(15) << std::left << obj_purchase_item.get_price()
		<< std::setw(15) << std::left << obj_purchase_item.get_total()
		<< std::setw(20) << std::left << obj_purchase_item.get_total_before_vat() << "\n";
}

std::tm util::get_current_datetime() {
	std::time_t date = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	struct std::tm tm;
	localtime_s(&tm, &date);
	return tm;
}

std::string util::tm_to_filesafe_str(std::tm& tm) {
	std::stringstream ssBuffer;
	ssBuffer << std::put_time(&tm, "%F_%I-%M-%S_%p");
	return ssBuffer.str();
}