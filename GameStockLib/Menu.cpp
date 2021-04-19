#include "Menu.h"

MenuContainer::MenuContainer(std::string const& text) : _str_text(text), _vec_menu_items()
{
	_bool_exit_menu = false;
	_i_highlighted_index = 0;
	_h_output_console = GetStdHandle(STD_OUTPUT_HANDLE);
	_h_input_console = GetStdHandle(STD_INPUT_HANDLE);
}

void MenuContainer::execute()
{
	KEY_EVENT_RECORD key{};

	while (key.wVirtualKeyCode != VK_RETURN) {
		system("cls");
		std::cout << _str_text << "\n";
		util::for_each_iterator(_vec_menu_items.begin(), _vec_menu_items.end(), 0, [&](int index, std::unique_ptr<MenuItem> const& item) {
			if (_i_highlighted_index == index) {
				SetConsoleTextAttribute(_h_output_console, BACKGROUND_BLUE | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_INTENSITY);
				std::cout << "\t" << item->item_text() << "\n";
				SetConsoleTextAttribute(_h_output_console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
			}
			else {
				std::cout << "\t" << item->item_text() << "\n";
			}
			});
		while (!validate::get_control_char(key, _h_input_console));
		if (key.wVirtualKeyCode == VK_DOWN) {
			if (_i_highlighted_index < (int)_vec_menu_items.size() - 1) _i_highlighted_index++;
		}
		else if (key.wVirtualKeyCode == VK_UP) {
			if (_i_highlighted_index > 0) _i_highlighted_index--;
		}
		else if (key.wVirtualKeyCode == VK_ESCAPE) {
			this->set_exit_menu(true);
			break;
		}
	}

	if ((int)_vec_menu_items.size() - 1 >= _i_highlighted_index && _i_highlighted_index >= 0) {
		if (key.wVirtualKeyCode != VK_ESCAPE) {
			_vec_menu_items[_i_highlighted_index]->execute();
		}
	}
	else {
		std::cout << "Not a valid option, please try again.\n";
		util::pause();
	}
};

void MenuContainer::add_menu_item(std::unique_ptr<MenuItem> item) {
	_vec_menu_items.push_back(std::move(item));
};

GeneralMenuItem::GeneralMenuItem(std::string output, ClassContainer& ptr_class_container) : _ptr_class_container(ptr_class_container)
{
	_output = output;
};

void DummyMenu::execute() {
	system("cls");
	std::cout << "Dummy menu option was entered...\n";
	util::pause();
}

void LoginMenu::execute() {
	User obj_user;
	system("cls");
	std::cout << "Follow the prompts to login to GameStock.\n\n";

	/*std::cout << "Please enter your email: ";
	obj_user.set_email(validate::validate_string());

	std::cout << "Please enter your password: ";
	obj_user.set_password(validate::validate_string());*/

	//obj_user.set_email("admin@gamestock.com");
	//obj_user.set_password("somesecureadminpassword");

	obj_user.set_email("email@email.com");
	obj_user.set_password("password");

	try {
		_ptr_class_container.ptr_user_manager.attempt_login(&obj_user);
		bool bool_user_is_admin = _ptr_class_container.ptr_user_manager.get_current_user().get_is_admin();

		MenuContainer obj_menu_container = MenuContainer("Logged in as " + obj_user.get_email() + ".\nChoose one of the below options.\n(Esc to logout)\n");
		if (bool_user_is_admin) {
			obj_menu_container.add_menu_item(std::unique_ptr<MenuItem>(new ViewGamesMenu("Manage games", _ptr_class_container)));
			obj_menu_container.add_menu_item(std::unique_ptr<MenuItem>(new DummyMenu("Manage genres", _ptr_class_container)));
			obj_menu_container.add_menu_item(std::unique_ptr<MenuItem>(new DummyMenu("Manage users", _ptr_class_container)));
			obj_menu_container.add_menu_item(std::unique_ptr<MenuItem>(new DummyMenu("Manage account", _ptr_class_container)));
			obj_menu_container.add_menu_item(std::unique_ptr<MenuItem>(new DummyMenu("Purchase history and reports", _ptr_class_container)));
		}
		else {
			obj_menu_container.add_menu_item(std::unique_ptr<MenuItem>(new ViewGamesMenu("View games", _ptr_class_container)));
			obj_menu_container.add_menu_item(std::unique_ptr<MenuItem>(new DummyMenu("Manage account", _ptr_class_container)));
			obj_menu_container.add_menu_item(std::unique_ptr<MenuItem>(new DummyMenu("View purchase history", _ptr_class_container)));
		}

		while (!obj_menu_container.get_exit_menu()) {
			system("cls");
			obj_menu_container.execute();
		}

		_ptr_class_container.ptr_user_manager.logout();
		_ptr_class_container.ptr_game_manager.logout();
	}
	catch (std::exception& ex) {
		std::cout << "Error: " << ex.what() << "\n";
		std::cout << "\nPlease try again.\n";
		util::pause();
	}
}

void RegisterMenu::execute() {
	User obj_user;
	std::string str_password_confirm = "";

	system("cls");
	std::cout << "Follow the prompts to register as a new user.\n\n";
	std::cout << "Please enter your full name: ";
	obj_user.set_full_name(validate::validate_string());

	std::cout << "Please enter your age: ";
	obj_user.set_age(validate::validate_int(1, 150));

	std::cout << "Please enter your email: ";
	obj_user.set_email(validate::validate_string());

	std::cout << "Please enter your password: ";
	obj_user.set_password(validate::validate_string(8));

	std::cout << "Please confirm your password: ";
	str_password_confirm = validate::validate_string(8);

	while (str_password_confirm != obj_user.get_password()) {
		std::cout << "Passwords do not match, please confirm your password: ";
		str_password_confirm = validate::validate_string();
	}

	try {
		_ptr_class_container.ptr_user_manager.register_user(&obj_user);

		std::cout << "\nRegistration complete. User name is " << obj_user.get_email() << ".\nPlease now attempt to log in on the next screen.\n";

		util::pause();
	}
	catch (std::exception& ex) {
		std::cout << "\nCould not register: " << ex.what() << "\n";
		std::cout << "\nPlease try again.\n";
		util::pause();
	}
}

void ViewGamesMenu::execute() {
	KEY_EVENT_RECORD key{};
	int i_highlighted_index = 0;
	HANDLE h_output_console = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE h_input_console = GetStdHandle(STD_INPUT_HANDLE);
	bool bool_user_is_admin = _ptr_class_container.ptr_user_manager.get_current_user().get_is_admin();
	int i_current_page = 0;
	int i_page_size = 10;
	int i_page_count = 0;

	try {
		_ptr_class_container.ptr_game_manager.set_admin_flag(bool_user_is_admin);
		_ptr_class_container.ptr_game_manager.initialise_games();
		std::vector<Game>& vec_games = _ptr_class_container.ptr_game_manager.get_vec_games();
		std::vector<Game> vec_paged_games;

		while (key.wVirtualKeyCode != VK_ESCAPE) {
			system("cls");
			if (bool_user_is_admin) {
				std::cout << "Manage/Update games\n";
				std::cout << "Use [Arrow Keys] to navigate games/pages, press [Enter] to select game to manage\n";
				std::cout << "Press [Esc] to go back\n";
				std::cout << "Press [F1] to add game\n";
				std::cout << "Press [F2] to filter by genre\n\n";
			}
			else {
				std::cout << "Current games in stock at GameStock\n";
				std::cout << "Use [Arrow Keys] to navigate games/pages, press [Enter] to select game to buy\n";
				std::cout << "Press [Esc] to go back\n";
				std::cout << "Press [F1] to view basket\n";
				std::cout << "Press [F2] to filter by genre\n\n";
			}

			if (vec_games.size() < 1) {
				std::cout << "There are currently no games to display.\n";
			}
			else {
				if (_ptr_class_container.ptr_game_manager.get_filter_genre().get_id() > 0) {
					std::cout << "Current genre filter: " << _ptr_class_container.ptr_game_manager.get_filter_genre().get_genre() << "\n\n";
				}

				util::output_games_header();

				i_page_count = ((int)vec_games.size() + i_page_size - 1) / i_page_size;

				// As protection from index overflows, reset current page if it is more than the zero adjusted page count
				// This is really a mess, but I can't think of many better ways of doing it.
				if (i_current_page > (i_page_count - 1)) i_current_page = 0;

				int i_final_item = 0;
				if (((i_current_page * 10) + 10) > (int)vec_games.size() - 1) {
					i_final_item = (int)vec_games.size();
				}
				else {
					i_final_item = ((i_current_page * 10) + 10);
				}

				vec_paged_games = std::vector<Game>(vec_games.begin() + (i_current_page * 10), vec_games.begin() + i_final_item);

				util::for_each_iterator(vec_paged_games.begin(), vec_paged_games.end(), 0, [&](int index, Game& item) {
					if (i_highlighted_index == index) {
						SetConsoleTextAttribute(h_output_console, BACKGROUND_BLUE | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_INTENSITY);
						util::output_game(item);
						SetConsoleTextAttribute(h_output_console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
					}
					else {
						util::output_game(item);
					}
					});

				std::cout << "\nPage " << i_current_page + 1 << " of " << i_page_count << "\n";
			}
			
			while (!validate::get_control_char(key, h_input_console));

			switch (key.wVirtualKeyCode)
			{
			case VK_DOWN:
				if (i_highlighted_index < (int)vec_paged_games.size() - 1) i_highlighted_index++;
				break;
			case VK_UP:
				if (i_highlighted_index > 0) i_highlighted_index--;
				break;
			case VK_LEFT:
				if (i_current_page > 0) { 
					i_current_page--; 
					i_highlighted_index = 0;
				}
				break;
			case VK_RIGHT:
				if (i_current_page + 1 < i_page_count) {
					i_current_page++;
					i_highlighted_index = 0;
				}
				break;
			case VK_ESCAPE:
				_ptr_class_container.ptr_game_manager.set_filter_genre(Genre());
				_ptr_class_container.ptr_game_manager.set_initialised(false);
				return;
			case VK_F1:
				if (!bool_user_is_admin) { 
					ViewBasketMenu("View Basket", _ptr_class_container).execute(); 
					i_highlighted_index = 0;
				}
				else { 
					AddGameMenu("Add game", _ptr_class_container).execute();
				}
				break;
			case VK_F2:
				SelectGenreFilterMenu("Genre Filter", _ptr_class_container).execute();
				i_highlighted_index = 0;
				break;
			case VK_RETURN:
				if (vec_games.size() < 1) {
					if (bool_user_is_admin) {
						std::cout << "You cannot manage games when there are none to display.\n";
					}
					else {
						std::cout << "You cannot add games to the basket when there are none to display.\n";
					}
					util::pause();
					break;
				}

				if ((int)vec_paged_games.size() - 1 >= i_highlighted_index && i_highlighted_index >= 0) {
					system("cls");
					Game& obj_game = vec_paged_games[i_highlighted_index];

					if (bool_user_is_admin) {
						ManageGameBaseMenu("Manage game", _ptr_class_container, obj_game).execute();
						_ptr_class_container.ptr_game_manager.initialise_games();
						break;
					}
					else {
						if (obj_game.get_copies() < 1) {
							std::cout << "Cannot buy this game, there is no stock remaining.\n";
							util::pause();
							break;
						}

						int i_copies = 0;
						std::cout << "Please enter the number of copies of '" << obj_game.get_name() << "' that you would like to buy\n";
						std::cout << "(Minimum 1, Maximum " << obj_game.get_copies() << ")\n";
						std::cout << "Copies: ";
						i_copies = validate::validate_int(1, obj_game.get_copies());

						PurchaseItem obj_purchase_item = PurchaseItem(obj_game.get_id(), obj_game, i_copies, obj_game.get_price());
						_ptr_class_container.ptr_game_manager.add_basket_item(obj_purchase_item);

						std::cout << i_copies << " copies of '" << obj_game.get_name() << "' succesfully added to basket.\n";
						util::pause();
						break;
					}
				}
				else {
					std::cout << "Not a valid option, please try again.\n";
					util::pause();
					break;
				}
			default:
				break;
			}
		}
	}
	catch (std::exception& ex) {
		std::cout << "Error: " << ex.what() << "\n";
		util::pause();
	}
}

void AddGameMenu::execute() {
	Game obj_game;
	HANDLE h_output_console = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE h_input_console = GetStdHandle(STD_INPUT_HANDLE);

	system("cls");
	std::cout << "Please follow the prompts to add a new game.\n";
	std::cout << "Please enter the game name\n";
	std::cout << "Game Name: ";
	obj_game.set_name(validate::validate_string(1, 45));

	try {
		KEY_EVENT_RECORD key{};
		int i_highlighted_index = 0;
		std::vector<Genre> vec_genres;

		vec_genres = _ptr_class_container.ptr_game_manager.get_genres();

		while (key.wVirtualKeyCode != VK_RETURN) {
			system("cls");
			std::cout << "Please select the game genre for '" << obj_game.get_name() << "'\nNavigate with [Arrow Keys]\nPress [Enter] to select choice\n\n";

			util::for_each_iterator(vec_genres.begin(), vec_genres.end(), 0, [&](int index, Genre& item) {
				if (i_highlighted_index == index) {
					SetConsoleTextAttribute(h_output_console, BACKGROUND_BLUE | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_INTENSITY);
					std::cout << "\t" << item.get_genre() << "\n";
					SetConsoleTextAttribute(h_output_console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
				}
				else {
					std::cout << "\t" << item.get_genre() << "\n";
				}
				});

			while (!validate::get_control_char(key, h_input_console));

			switch (key.wVirtualKeyCode)
			{
			case VK_DOWN:
				if (i_highlighted_index < (int)vec_genres.size() - 1) i_highlighted_index++;
				break;
			case VK_UP:
				if (i_highlighted_index > 0) i_highlighted_index--;
				break;
			case VK_RETURN:
				if ((int)vec_genres.size() - 1 >= i_highlighted_index && i_highlighted_index >= 0) {
					obj_game.set_genre(vec_genres[i_highlighted_index]);
					break;
				}
				else {
					std::cout << "Not a valid option, please try again.\n";
					util::pause();
					break;
				}
			default:
				break;
			}
		}
	}
	catch (std::exception& ex) {
		std::cout << ex.what() << "\n";
		util::pause();
		return;
	}

	try {
		KEY_EVENT_RECORD key{};
		int i_highlighted_index = 0;
		std::vector<Rating> vec_ratings;

		vec_ratings = _ptr_class_container.ptr_game_manager.get_ratings();

		while (key.wVirtualKeyCode != VK_RETURN) {
			system("cls");
			std::cout << "Please select the age rating for '" << obj_game.get_name() << "'\nNavigate with [Arrow Keys]\nPress [Enter] to select choice\n\n";

			util::for_each_iterator(vec_ratings.begin(), vec_ratings.end(), 0, [&](int index, Rating& item) {
				if (i_highlighted_index == index) {
					SetConsoleTextAttribute(h_output_console, BACKGROUND_BLUE | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_INTENSITY);
					std::cout << "\t" << item.get_rating() << "\n";
					SetConsoleTextAttribute(h_output_console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
				}
				else {
					std::cout << "\t" << item.get_rating() << "\n";
				}
				});

			while (!validate::get_control_char(key, h_input_console));

			switch (key.wVirtualKeyCode)
			{
			case VK_DOWN:
				if (i_highlighted_index < (int)vec_ratings.size() - 1) i_highlighted_index++;
				break;
			case VK_UP:
				if (i_highlighted_index > 0) i_highlighted_index--;
				break;
			case VK_RETURN:
				if ((int)vec_ratings.size() - 1 >= i_highlighted_index && i_highlighted_index >= 0) {
					obj_game.set_rating(vec_ratings[i_highlighted_index]);
					break;
				}
				else {
					std::cout << "Not a valid option, please try again.\n";
					util::pause();
					break;
				}
			default:
				break;
			}
		}
	}
	catch (std::exception& ex) {
		std::cout << ex.what() << "\n";
		util::pause();
		return;
	}

	system("cls");
	std::cout << "Please enter the price for '" << obj_game.get_name() << "'\n";
	std::cout << "Game Price: ";
	obj_game.set_price(validate::validate_double(0.0));
	std::cout << "\n";

	std::cout << "Please enter the number of available copies for '" << obj_game.get_name() << "'\n";
	std::cout << "Available Copies: ";
	obj_game.set_copies(validate::validate_int(0));

	try {
		_ptr_class_container.ptr_game_manager.add_game(obj_game);
		_ptr_class_container.ptr_game_manager.set_filter_genre(Genre());
		_ptr_class_container.ptr_game_manager.set_initialised(false);
		_ptr_class_container.ptr_game_manager.initialise_games();
		std::cout << "\n'" << obj_game.get_name() << "' successfully added\nGo to the manage game screen if you wish to manage this game.\n";
		util::pause();
	}
	catch (std::exception& ex) {
		std::cout << ex.what() << "\n";
		util::pause();
	}
}

void SelectGenreFilterMenu::execute() {
	KEY_EVENT_RECORD key{};
	int i_highlighted_index = 0;
	HANDLE h_output_console = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE h_input_console = GetStdHandle(STD_INPUT_HANDLE);
	std::vector<Genre> vec_genres;

	try {
		vec_genres = _ptr_class_container.ptr_game_manager.get_genres();
	}
	catch (std::exception& ex) {
		std::cout << ex.what() << "\n";
		util::pause();
		return;
	}

	while (key.wVirtualKeyCode != VK_RETURN) {
		system("cls");
		std::cout << "Select genre to filter by\nNavigate with [Arrow Keys]\nPress [F1] to clear current filter\nPress [Enter] to select choice\nPress [Esc] to cancel.\n\n";

		if (_ptr_class_container.ptr_game_manager.get_filter_genre().get_id() > 0) {
			std::cout << "Current genre filter: " << _ptr_class_container.ptr_game_manager.get_filter_genre().get_genre() << "\n\n";
		}

		util::for_each_iterator(vec_genres.begin(), vec_genres.end(), 0, [&](int index, Genre& item) {
			if (i_highlighted_index == index) {
				SetConsoleTextAttribute(h_output_console, BACKGROUND_BLUE | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_INTENSITY);
				std::cout << "\t" << item.get_genre() << "\n";
				SetConsoleTextAttribute(h_output_console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
			}
			else {
				std::cout << "\t" << item.get_genre() << "\n";
			}
			});

		while (!validate::get_control_char(key, h_input_console));

		switch (key.wVirtualKeyCode)
		{
		case VK_DOWN:
			if (i_highlighted_index < (int)vec_genres.size() - 1) i_highlighted_index++;
			break;
		case VK_UP:
			if (i_highlighted_index > 0) i_highlighted_index--;
			break;
		case VK_F1:
			_ptr_class_container.ptr_game_manager.set_filter_genre(Genre());
			_ptr_class_container.ptr_game_manager.set_initialised(false);
			_ptr_class_container.ptr_game_manager.initialise_games();
			break;
		case VK_RETURN:
			if ((int)vec_genres.size() - 1 >= i_highlighted_index && i_highlighted_index >= 0) {
				Genre obj_genre = vec_genres[i_highlighted_index];
				_ptr_class_container.ptr_game_manager.set_filter_genre(obj_genre);
				_ptr_class_container.ptr_game_manager.set_initialised(false);
				_ptr_class_container.ptr_game_manager.initialise_games();
				return;
			}
			else {
				std::cout << "Not a valid option, please try again.\n";
				util::pause();
				break;
			}
		case VK_ESCAPE:
			return;
		default:
			break;
		}
	}
}

void ViewBasketMenu::execute() {
	std::vector<PurchaseItem>& vec_basket_items = _ptr_class_container.ptr_game_manager.get_basket().get_vec_purchase_items();

	KEY_EVENT_RECORD key{};
	int i_highlighted_index = 0;
	HANDLE h_output_console = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE h_input_console = GetStdHandle(STD_INPUT_HANDLE);
	double d_basket_total = _ptr_class_container.ptr_game_manager.get_basket_total();

	while (key.wVirtualKeyCode != VK_ESCAPE) {
		system("cls");
		std::cout << "Your current basket items\n";
		std::cout << "Use [Arrow Keys] to navigate basket, press [Enter] to remove basket item.\n";
		std::cout << "Press [Esc] to go back\n";
		std::cout << "Press [F1] to confirm purchase\n\n";


		if (vec_basket_items.size() < 1) {
			std::cout << "There are currently no basket items to display.\n";
		}
		else {
			util::output_basket_header();

			util::for_each_iterator(vec_basket_items.begin(), vec_basket_items.end(), 0, [&](int index, PurchaseItem& item) {
				if (i_highlighted_index == index) {
					SetConsoleTextAttribute(h_output_console, BACKGROUND_BLUE | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_INTENSITY);
					util::output_basket_item(item);
					SetConsoleTextAttribute(h_output_console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
				}
				else {
					util::output_basket_item(item);
				}
				});

			std::cout << "\nTotal: " << d_basket_total << "\n";
		}

		while (!validate::get_control_char(key, h_input_console));

		switch (key.wVirtualKeyCode)
		{
		case VK_DOWN:
			if (i_highlighted_index < (int)vec_basket_items.size() - 1) i_highlighted_index++;
			break;
		case VK_UP:
			if (i_highlighted_index > 0) i_highlighted_index--;
			break;
		case VK_ESCAPE:
			return;
		case VK_F1:
			if (vec_basket_items.size() < 1) {
				std::cout << "You cannot confirm a purchase with an empty basket.\n";
				util::pause();
				break;
			}

			try {
				_ptr_class_container.ptr_game_manager.set_basket_user(_ptr_class_container.ptr_user_manager.get_user_id());
				std::cout << "\nPurchase successfully placed totalling " << std::setprecision(2) << _ptr_class_container.ptr_game_manager.make_purchase() << "\n";
				std::cout << "Please go to the main menu and 'View Purchase History' to see this invoice\n\n";
				_ptr_class_container.ptr_game_manager.set_initialised(false);
				_ptr_class_container.ptr_game_manager.initialise_games();
				_ptr_class_container.ptr_game_manager.reset_basket();
				util::pause();
				return;
			}
			catch (std::exception& ex) {
				std::cout << ex.what() << "\n";
				util::pause();
				break;
			}
		case VK_RETURN:
			if (vec_basket_items.size() < 1) {
				std::cout << "You cannot remove items from the basket when there are none to display.\n";
				util::pause();
				break;
			}

			if ((int)vec_basket_items.size() - 1 >= i_highlighted_index && i_highlighted_index >= 0) {
				system("cls");
				PurchaseItem obj_purchase_item = vec_basket_items[i_highlighted_index];
				std::cout << "Removing '" << obj_purchase_item.get_game().get_name() << "' from basket...\n";

				_ptr_class_container.ptr_game_manager.remove_basket_item(obj_purchase_item.get_game_id());
				if (i_highlighted_index > (int)vec_basket_items.size() - 1) {
					i_highlighted_index--;
				}

				std::cout << "'" << obj_purchase_item.get_game().get_name() << "' removed from basket.\n";
				d_basket_total = _ptr_class_container.ptr_game_manager.get_basket_total();
				util::pause();
				break;
			}
			else {
				std::cout << "Not a valid option, please try again.\n";
				util::pause();
				break;
			}
		default:
			break;
		}
	}
}

void SubMenuExample::execute() {
	MenuContainer obj_menu_container = MenuContainer("Sub menu example.\nChoose one of the below options.\n");
	obj_menu_container.add_menu_item(std::unique_ptr<MenuItem>(new DummyMenu("Example item 1", _ptr_class_container)));
	obj_menu_container.add_menu_item(std::unique_ptr<MenuItem>(new DummyMenu("Example item 2", _ptr_class_container)));

	while (!obj_menu_container.get_exit_menu()) {
		system("cls");
		obj_menu_container.execute();
	}
}

void ManageGameBaseMenu::execute() {
	MenuContainer obj_menu_container = MenuContainer("");
	obj_menu_container.add_menu_item(std::unique_ptr<MenuItem>(new UpdateGameNameMenu("Change name", _ptr_class_container, _obj_game)));
	obj_menu_container.add_menu_item(std::unique_ptr<MenuItem>(new UpdateGameGenreMenu("Change genre", _ptr_class_container, _obj_game)));
	obj_menu_container.add_menu_item(std::unique_ptr<MenuItem>(new UpdateGamePriceMenu("Change price", _ptr_class_container, _obj_game)));
	obj_menu_container.add_menu_item(std::unique_ptr<MenuItem>(new UpdateGameRatingMenu("Change age rating", _ptr_class_container, _obj_game)));
	obj_menu_container.add_menu_item(std::unique_ptr<MenuItem>(new UpdateGameCopiesMenu("Update number of copies available", _ptr_class_container, _obj_game)));

	while (!obj_menu_container.get_exit_menu()) {
		obj_menu_container.set_menu_text("Selected game: " + _obj_game.get_name() + "\nChoose one of the below actions to perform on this game.\nPress [ESC] to cancel\n");
		system("cls");
		obj_menu_container.execute();
	}
}

void UpdateGameNameMenu::execute() {
	std::string str_update_name = "";

	system("cls");
	std::cout << "Updating game name of '" << _obj_game.get_name() << "'\n\n";
	std::cout << "Please enter new name : ";
	str_update_name = validate::validate_string(45, true);

	try {
		_ptr_class_container.ptr_game_manager.update_game_name(_obj_game.get_id(), str_update_name);
		_ptr_class_container.ptr_game_manager.set_initialised(false);
		std::cout << "'" << _obj_game.get_name() << "' updated to '" << str_update_name << "' successfully.\n";
		_obj_game.set_name(str_update_name);
		util::pause();
	}
	catch (std::exception& ex) {
		std::cout << ex.what() << "\n";
		util::pause();
	}
}

void UpdateGameGenreMenu::execute() {
	int i_update_genre = 0;
	KEY_EVENT_RECORD key{};
	int i_highlighted_index = 0;
	HANDLE h_output_console = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE h_input_console = GetStdHandle(STD_INPUT_HANDLE);
	std::vector<Genre> vec_genres;

	try {
		vec_genres = _ptr_class_container.ptr_game_manager.get_genres();
	}
	catch (std::exception& ex) {
		std::cout << ex.what() << "\n";
		util::pause();
		return;
	}

	while (key.wVirtualKeyCode != VK_RETURN) {
		system("cls");
		std::cout << "Updating game genre of '" << _obj_game.get_name() << "'\nNavigate with [Arrow Keys]\nPress [Enter] to select choice\nPress [Esc] to cancel.\n\n";

		util::for_each_iterator(vec_genres.begin(), vec_genres.end(), 0, [&](int index, Genre& item) {
			if (i_highlighted_index == index) {
				SetConsoleTextAttribute(h_output_console, BACKGROUND_BLUE | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_INTENSITY);
				std::cout << "\t" << item.get_genre() << "\n";
				SetConsoleTextAttribute(h_output_console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
			}
			else {
				std::cout << "\t" << item.get_genre() << "\n";
			}
			});

		while (!validate::get_control_char(key, h_input_console));

		switch (key.wVirtualKeyCode)
		{
		case VK_DOWN:
			if (i_highlighted_index < (int)vec_genres.size() - 1) i_highlighted_index++;
			break;
		case VK_UP:
			if (i_highlighted_index > 0) i_highlighted_index--;
			break;
		case VK_RETURN:
			if ((int)vec_genres.size() - 1 >= i_highlighted_index && i_highlighted_index >= 0) {
				try {
					Genre obj_genre = vec_genres[i_highlighted_index];
					_ptr_class_container.ptr_game_manager.update_game_genre(_obj_game.get_id(), obj_genre.get_id());
					_ptr_class_container.ptr_game_manager.set_initialised(false);
					std::cout << "\n'" << _obj_game.get_name() << "' genre successfully updated from " << _obj_game.get_genre().get_genre() << " to " << obj_genre.get_genre() << "\n";
					_obj_game.set_genre(obj_genre);
					util::pause();
					break;
				}
				catch (std::exception& ex) {
					std::cout << ex.what() << "\n";
					util::pause();
					return;
				}
			}
			else {
				std::cout << "Not a valid option, please try again.\n";
				util::pause();
				break;
			}
		case VK_ESCAPE:
			return;
		default:
			break;
		}
	}
}

void UpdateGamePriceMenu::execute() {
	double d_update_price = 0;

	system("cls");
	std::cout << "Updating game price of '" << _obj_game.get_name() << "' (Currently " << _obj_game.get_price() << ")\n\n";
	std::cout << "Please enter new price : ";
	d_update_price = validate::validate_double(0.0);

	try {
		_ptr_class_container.ptr_game_manager.update_game_price(_obj_game.get_id(), d_update_price);
		_ptr_class_container.ptr_game_manager.set_initialised(false);
		std::cout << "'" << _obj_game.get_name() << "' price updated to '" << std::setprecision(2) << d_update_price << "' successfully.\n";
		_obj_game.set_price(d_update_price);
		util::pause();
	}
	catch (std::exception& ex) {
		std::cout << ex.what() << "\n";
		util::pause();
	}
}

void UpdateGameRatingMenu::execute() {
	int i_update_rating = 0;
	KEY_EVENT_RECORD key{};
	int i_highlighted_index = 0;
	HANDLE h_output_console = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE h_input_console = GetStdHandle(STD_INPUT_HANDLE);
	std::vector<Rating> vec_ratings;

	try {
		vec_ratings = _ptr_class_container.ptr_game_manager.get_ratings();
	}
	catch (std::exception& ex) {
		std::cout << ex.what() << "\n";
		util::pause();
		return;
	}

	while (key.wVirtualKeyCode != VK_RETURN) {
		system("cls");
		std::cout << "Updating game age rating of '" << _obj_game.get_name() << "'\nNavigate with [Arrow Keys]\nPress [Enter] to select choice\nPress [Esc] to cancel.\n\n";

		util::for_each_iterator(vec_ratings.begin(), vec_ratings.end(), 0, [&](int index, Rating& item) {
			if (i_highlighted_index == index) {
				SetConsoleTextAttribute(h_output_console, BACKGROUND_BLUE | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_INTENSITY);
				std::cout << "\t" << item.get_rating() << "\n";
				SetConsoleTextAttribute(h_output_console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
			}
			else {
				std::cout << "\t" << item.get_rating() << "\n";
			}
			});

		while (!validate::get_control_char(key, h_input_console));

		switch (key.wVirtualKeyCode)
		{
		case VK_DOWN:
			if (i_highlighted_index < (int)vec_ratings.size() - 1) i_highlighted_index++;
			break;
		case VK_UP:
			if (i_highlighted_index > 0) i_highlighted_index--;
			break;
		case VK_RETURN:
			if ((int)vec_ratings.size() - 1 >= i_highlighted_index && i_highlighted_index >= 0) {
				try {
					Rating obj_rating = vec_ratings[i_highlighted_index];
					_ptr_class_container.ptr_game_manager.update_game_rating(_obj_game.get_id(), obj_rating.get_id());
					_ptr_class_container.ptr_game_manager.set_initialised(false);
					std::cout << "\n'" << _obj_game.get_name() << "' rating successfully updated from " << _obj_game.get_rating().get_rating() << " to " << obj_rating.get_rating() << "\n";
					_obj_game.set_rating(obj_rating);
					util::pause();
					break;
				}
				catch (std::exception& ex) {
					std::cout << ex.what() << "\n";
					util::pause();
					return;
				}
			}
			else {
				std::cout << "Not a valid option, please try again.\n";
				util::pause();
				break;
			}
		case VK_ESCAPE:
			return;
		default:
			break;
		}
	}
}

void UpdateGameCopiesMenu::execute() {
	int i_update_copies = 0;

	system("cls");
	std::cout << "Updating available copies of '" << _obj_game.get_name() << "' (Currently " << _obj_game.get_copies() << ")\n\n";
	std::cout << "Please enter available copies : ";
	i_update_copies = validate::validate_int(0);

	try {
		_ptr_class_container.ptr_game_manager.update_game_copies(_obj_game.get_id(), i_update_copies);
		_ptr_class_container.ptr_game_manager.set_initialised(false);
		std::cout << "'" << _obj_game.get_name() << "' available copies updated to '" << i_update_copies << "' successfully.\n";
		_obj_game.set_copies(i_update_copies);
		util::pause();
	}
	catch (std::exception& ex) {
		std::cout << ex.what() << "\n";
		util::pause();
	}
}