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

	// Only exit when menu option selected
	while (key.wVirtualKeyCode != VK_RETURN) {
		system("cls");
		std::cout << _str_text << "\n";
		// Display menu options
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

		// Determine action to take based on user input
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

	// Execute the selected menu option
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

void LoginMenu::execute() {
	User obj_user;
	system("cls");
	std::cout << "Follow the prompts to login to GameStock.\n\n";

	// Get user to enter details
	std::cout << "Please enter your email: ";
	obj_user.set_email(validate::validate_string(0, 45));

	std::cout << "Please enter your password: ";
	obj_user.set_password(validate::validate_string(7));

	try {
		// Attempt to login with provided details
		_ptr_class_container.ptr_user_manager.attempt_login(obj_user);
		bool bool_user_is_admin = _ptr_class_container.ptr_user_manager.get_current_user().get_is_admin();

		// Display differnet menu options based on if user is an admin or not
		MenuContainer obj_menu_container = MenuContainer("Logged in as " + obj_user.get_email() + ".\nChoose one of the below options.\n(Esc to logout)\n");
		if (bool_user_is_admin) {
			obj_menu_container.add_menu_item(std::unique_ptr<MenuItem>(new ViewGamesMenu("Manage games", _ptr_class_container)));
			obj_menu_container.add_menu_item(std::unique_ptr<MenuItem>(new ManageGenresMenu("Manage genres", _ptr_class_container)));
			obj_menu_container.add_menu_item(std::unique_ptr<MenuItem>(new ManageUsersMenu("Manage users", _ptr_class_container)));
			obj_menu_container.add_menu_item(std::unique_ptr<MenuItem>(new UserUpdateOptionsMenu("Manage account", _ptr_class_container, _ptr_class_container.ptr_user_manager.get_current_user())));
			obj_menu_container.add_menu_item(std::unique_ptr<MenuItem>(new SelectUserPurchasesViewMenu("Purchase history and reports", _ptr_class_container)));
		}
		else {
			obj_menu_container.add_menu_item(std::unique_ptr<MenuItem>(new ViewGamesMenu("View games", _ptr_class_container)));
			obj_menu_container.add_menu_item(std::unique_ptr<MenuItem>(new UserUpdateOptionsMenu("Manage account", _ptr_class_container, _ptr_class_container.ptr_user_manager.get_current_user())));
			obj_menu_container.add_menu_item(std::unique_ptr<MenuItem>(new ViewUserPurchasesMenu("View purchase history", _ptr_class_container, _ptr_class_container.ptr_user_manager.get_current_user())));
		}

		while (!obj_menu_container.get_exit_menu()) {
			system("cls");
			obj_menu_container.execute();
		}

		_ptr_class_container.ptr_user_manager.logout();
		_ptr_class_container.ptr_game_manager.logout();
	}
	// If login fails, show error and allow user to come back and try again
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
	std::cout << "Follow the prompts to register a new user.\n\n";
	std::cout << "Please enter full name: ";
	obj_user.set_full_name(validate::validate_string(1, 30));

	std::cout << "Please enter age: ";
	obj_user.set_age(validate::validate_int(1, 150));

	std::cout << "Please enter email: ";
	obj_user.set_email(validate::validate_string(1, 45));

	std::cout << "Please enter password: ";
	obj_user.set_password(validate::validate_string(8, 256));

	std::cout << "Please confirm password: ";
	str_password_confirm = validate::validate_string(8, 256);

	// Do not allow user to continue until they enter a pssword that matches the previous
	while (str_password_confirm != obj_user.get_password()) {
		std::cout << "Passwords do not match, please confirm password: ";
		str_password_confirm = validate::validate_string(8, 256);
	}

	try {
		// Add user to database
		_ptr_class_container.ptr_user_manager.register_user(obj_user);

		std::cout << "\nRegistration complete. User name is " << obj_user.get_email() << ".\n";

		util::pause();
	}
	// If adding fails (likely matching email address), show error and allow user to try again
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
		// Get current games
		_ptr_class_container.ptr_game_manager.set_admin_flag(bool_user_is_admin);
		_ptr_class_container.ptr_game_manager.initialise_games();
		std::vector<Game>& vec_games = _ptr_class_container.ptr_game_manager.get_vec_games();
		std::vector<Game> vec_paged_games;

		while (key.wVirtualKeyCode != VK_ESCAPE) {
			system("cls");
			// Display different options based on if user is an admin or not
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

				// As protection from index overflows, reset current page if it is more than the zero-index adjusted page count
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
				// Output "paged" games
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

			// Perform relevant action based on user input
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
				// Unset filter when leaving games page
				_ptr_class_container.ptr_game_manager.set_filter_genre(Genre());
				_ptr_class_container.ptr_game_manager.set_initialised(false);
				return;
			case VK_F1:
				// Only allow user to go to basket if they are not an admin, admins use F1 to add a game instead
				if (!bool_user_is_admin) {
					ViewBasketMenu("View Basket", _ptr_class_container).execute();
					i_highlighted_index = 0;
				}
				else {
					AddGameMenu("Add game", _ptr_class_container).execute();
				}
				break;
			case VK_F2:
				// Allow user/admin to select a filter option
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

					// Either manage selected game, or allow user to add to basket
					if (bool_user_is_admin) {
						ManageGameBaseMenu("Manage game", _ptr_class_container, obj_game).execute();
						_ptr_class_container.ptr_game_manager.initialise_games();
						i_highlighted_index = 0;
						break;
					}
					else {
						if (obj_game.get_copies() < 1) {
							std::cout << "Cannot buy this game, there is no stock remaining.\n";
							util::pause();
							break;
						}

						int i_copies = 0;
						std::cout << "Please enter the number of copies of '" << obj_game.get_name() << "' that you would like to add to your basket\n";
						std::cout << "(Minimum 1, Maximum " << obj_game.get_copies() << ")\n";
						std::cout << "Copies: ";
						i_copies = validate::validate_int(1, obj_game.get_copies());

						try {
							// Attempt to add selected game to basket
							PurchaseItem obj_purchase_item = PurchaseItem(obj_game.get_id(), obj_game, i_copies, obj_game.get_price());
							_ptr_class_container.ptr_game_manager.add_basket_item(obj_purchase_item);

							std::cout << i_copies << " copies of '" << obj_game.get_name() << "' succesfully added to basket.\n";
							util::pause();
							break;
						}
						catch (std::exception& ex) {
							std::cout << "Error: " << ex.what() << "\n";
							util::pause();
							break;
						}
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

	// Get user selection for genre (displays list to limit choice to only valid options
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

	// Get user selection for genre (displays list to limit user input)
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
		// Attempt to add game, unset filter and re-initialise games upon success
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

	// Display genres and allow user to select genre to filter by
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
			// Unset the current genre filter
			_ptr_class_container.ptr_game_manager.set_filter_genre(Genre());
			_ptr_class_container.ptr_game_manager.set_initialised(false);
			_ptr_class_container.ptr_game_manager.initialise_games();
			break;
		case VK_RETURN:
			if ((int)vec_genres.size() - 1 >= i_highlighted_index && i_highlighted_index >= 0) {
				// Set selected genre as the filter
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

	// Display current basket state to user
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
				// Attempt to persist basket to database
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
				// Remove basket purchase item
				PurchaseItem obj_purchase_item = vec_basket_items[i_highlighted_index];
				std::cout << "\nRemoving '" << obj_purchase_item.get_game().get_name() << "' from basket...\n";

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

void ManageGameBaseMenu::execute() {
	// Admin options for managing a game
	MenuContainer obj_menu_container = MenuContainer("");
	obj_menu_container.add_menu_item(std::unique_ptr<MenuItem>(new UpdateGameNameMenu("Change name", _ptr_class_container, _obj_game)));
	obj_menu_container.add_menu_item(std::unique_ptr<MenuItem>(new UpdateGameGenreMenu("Change genre", _ptr_class_container, _obj_game)));
	obj_menu_container.add_menu_item(std::unique_ptr<MenuItem>(new UpdateGamePriceMenu("Change price", _ptr_class_container, _obj_game)));
	obj_menu_container.add_menu_item(std::unique_ptr<MenuItem>(new UpdateGameRatingMenu("Change age rating", _ptr_class_container, _obj_game)));
	obj_menu_container.add_menu_item(std::unique_ptr<MenuItem>(new UpdateGameCopiesMenu("Update number of copies available", _ptr_class_container, _obj_game)));
	obj_menu_container.add_menu_item(std::unique_ptr<MenuItem>(new DeleteGameMenu("Delete game", _ptr_class_container, _obj_game, obj_menu_container)));

	while (!obj_menu_container.get_exit_menu()) {
		obj_menu_container.set_menu_text("Selected game: " + _obj_game.get_name() + "\nChoose one of the below actions to perform on this game.\nPress [ESC] to cancel\n");
		system("cls");
		obj_menu_container.execute();
	}
}

void DeleteGameMenu::execute() {
	HANDLE h_input_console = GetStdHandle(STD_INPUT_HANDLE);
	KEY_EVENT_RECORD key{};

	system("cls");
	std::cout << "Are you sure you want to delete '" << _obj_game.get_name() << "'?\n";
	std::cout << "NOTE: Deleting a game will not remove any purchase history that contains this game\n\n";
	std::cout << "Press [Enter] to confirm, or [Esc] to cancel\n";

	// Allow user to either confirm deletion or cancel
	while (key.wVirtualKeyCode != VK_RETURN) {
		while (!validate::get_control_char(key, h_input_console));

		switch (key.wVirtualKeyCode)
		{
		case VK_RETURN:
			try {
				_ptr_class_container.ptr_game_manager.delete_game(_obj_game);
				_ptr_class_container.ptr_game_manager.set_initialised(false);
				_obj_menu_container.set_exit_menu(true);
				std::cout << "\n'" << _obj_game.get_name() << "' successfully deleted.\n";
				util::pause();
				break;
			}
			catch (std::exception& ex) {
				std::cout << ex.what() << "\n";
				util::pause();
				return;
			}
			break;
		case VK_ESCAPE:
			return;
		default:
			break;
		}
	}
}

void UpdateGameNameMenu::execute() {
	std::string str_update_name = "";

	system("cls");
	std::cout << "Updating game name of '" << _obj_game.get_name() << "'\n\n";
	std::cout << "Please enter new name : ";
	str_update_name = validate::validate_string(45, true);

	try {
		// Attempt to persist updated game name
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

	// Try to get genres from the database
	try {
		vec_genres = _ptr_class_container.ptr_game_manager.get_genres();
	}
	catch (std::exception& ex) {
		std::cout << ex.what() << "\n";
		util::pause();
		return;
	}

	// Allow user to select genre to update game to, allow cancel via escape
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
					// Attempt to persist updated game's genre
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
		// Attempt to persist updated game's price
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

	// Attempt to get ratings from the database
	try {
		vec_ratings = _ptr_class_container.ptr_game_manager.get_ratings();
	}
	catch (std::exception& ex) {
		std::cout << ex.what() << "\n";
		util::pause();
		return;
	}

	// Display ratings to user and allow selection, or cancellation
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
					// Attempt to persist game's new rating to database
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
		// Attempt to persist updated game's copies to the database
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

void ManageGenresMenu::execute() {
	KEY_EVENT_RECORD key{};
	int i_highlighted_index = 0;
	HANDLE h_output_console = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE h_input_console = GetStdHandle(STD_INPUT_HANDLE);
	int i_current_page = 0;
	int i_page_size = 10;
	int i_page_count = 0;

	try {
		// Get genres from database
		std::vector<Genre> vec_genres = _ptr_class_container.ptr_game_manager.get_genres();
		std::vector<Genre> vec_paged_genres;

		// List genres to user
		while (key.wVirtualKeyCode != VK_ESCAPE) {
			system("cls");
			std::cout << "Manage/Update genres\n";
			std::cout << "Use [Arrow Keys] to navigate genres/pages, press [Enter] to select genre to manage\n";
			std::cout << "Press [Esc] to go back\n";
			std::cout << "Press [F1] to add new genre\n\n";

			std::cout << "WARNING: In order to preserve data integrity, deleting a genre will also remove ALL games that use this genre.\n\n";

			if (vec_genres.size() < 1) {
				std::cout << "There are currently no genres to display.\n";
			}
			else {
				i_page_count = ((int)vec_genres.size() + i_page_size - 1) / i_page_size;

				// As protection from index overflows, reset current page if it is more than the zero-index adjusted page count
				// This is really a mess, but I can't think of many better ways of doing it.
				if (i_current_page > (i_page_count - 1)) i_current_page = 0;

				int i_final_item = 0;
				if (((i_current_page * 10) + 10) > (int)vec_genres.size() - 1) {
					i_final_item = (int)vec_genres.size();
				}
				else {
					i_final_item = ((i_current_page * 10) + 10);
				}

				vec_paged_genres = std::vector<Genre>(vec_genres.begin() + (i_current_page * 10), vec_genres.begin() + i_final_item);

				util::for_each_iterator(vec_paged_genres.begin(), vec_paged_genres.end(), 0, [&](int index, Genre& item) {
					if (i_highlighted_index == index) {
						SetConsoleTextAttribute(h_output_console, BACKGROUND_BLUE | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_INTENSITY);
						std::cout << "\t" << item.get_genre() << "\n";
						SetConsoleTextAttribute(h_output_console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
					}
					else {
						std::cout << "\t" << item.get_genre() << "\n";
					}
					});

				std::cout << "\nPage " << i_current_page + 1 << " of " << i_page_count << "\n";
			}

			while (!validate::get_control_char(key, h_input_console));

			switch (key.wVirtualKeyCode)
			{
			case VK_DOWN:
				if (i_highlighted_index < (int)vec_paged_genres.size() - 1) i_highlighted_index++;
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
				return;
			case VK_F1:
				// Allow new genre to be added to database
				AddGenreMenu("Add genre", _ptr_class_container).execute();
				vec_genres = _ptr_class_container.ptr_game_manager.get_genres();
				break;
			case VK_RETURN:
				if (vec_genres.size() < 1) {
					std::cout << "You cannot manage genres when there are none to display.\n";
					util::pause();
					break;
				}

				if ((int)vec_paged_genres.size() - 1 >= i_highlighted_index && i_highlighted_index >= 0) {
					// Open manage menu for selected genre
					system("cls");
					Genre& obj_genre = vec_paged_genres[i_highlighted_index];

					ManageGenreBaseMenu("Manage genre", _ptr_class_container, obj_genre).execute();
					vec_genres = _ptr_class_container.ptr_game_manager.get_genres();
					i_highlighted_index = 0;
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
		std::cout << "Error: " << ex.what() << "\n";
		util::pause();
	}
}

void ManageGenreBaseMenu::execute() {
	// Menu options for managing a genre
	MenuContainer obj_menu_container = MenuContainer("");
	obj_menu_container.add_menu_item(std::unique_ptr<MenuItem>(new UpdateGenreNameMenu("Change name", _ptr_class_container, _obj_genre)));
	obj_menu_container.add_menu_item(std::unique_ptr<MenuItem>(new DeleteGenreMenu("Delete genre", _ptr_class_container, _obj_genre, obj_menu_container)));

	while (!obj_menu_container.get_exit_menu()) {
		obj_menu_container.set_menu_text("Selected genre: " + _obj_genre.get_genre() + "\nChoose one of the below actions to perform on this game.\nPress [ESC] to cancel\n");
		system("cls");
		obj_menu_container.execute();
	}
}

void AddGenreMenu::execute() {
	Genre obj_genre;

	system("cls");
	std::cout << "Please follow the prompts to add a new genre.\n";
	std::cout << "Please enter the genre name\n";
	std::cout << "Genre Name: ";
	obj_genre.set_genre(validate::validate_string(1, 20));

	try {
		// Attempt to persist new genre
		_ptr_class_container.ptr_game_manager.add_genre(obj_genre);
		std::cout << "\n'" << obj_genre.get_genre() << "' successfully added.\n";
		util::pause();
	}
	catch (std::exception& ex) {
		std::cout << "\nError: " << ex.what() << "\n";
		util::pause();
	}
}

void DeleteGenreMenu::execute() {
	HANDLE h_input_console = GetStdHandle(STD_INPUT_HANDLE);
	KEY_EVENT_RECORD key{};

	system("cls");
	std::cout << "Are you sure you want to delete '" << _obj_genre.get_genre() << "'?\n";
	std::cout << "WARNING: Deleting this genre will remove ALL games that use this genre in order to preserve data integrity.\nHowever, This does not impact purchase history\n\n";
	std::cout << "Press [Enter] to confirm, or [Esc] to cancel\n";

	// Allow user to accept or cancel deletion of genre
	while (key.wVirtualKeyCode != VK_RETURN) {
		while (!validate::get_control_char(key, h_input_console));

		switch (key.wVirtualKeyCode)
		{
		case VK_RETURN:
			try {
				// Attempt to delete genre from database
				_ptr_class_container.ptr_game_manager.delete_genre(_obj_genre);
				_obj_menu_container.set_exit_menu(true);
				std::cout << "\n'" << _obj_genre.get_genre() << "' successfully deleted.\n";
				util::pause();
				break;
			}
			catch (std::exception& ex) {
				std::cout << "\nError: " << ex.what() << "\n";
				util::pause();
				return;
			}
			break;
		case VK_ESCAPE:
			return;
		default:
			break;
		}
	}
}

void UpdateGenreNameMenu::execute() {
	std::string str_update_genre_name = "";

	system("cls");
	std::cout << "Updating genre '" << _obj_genre.get_genre() << "'\n\n";
	std::cout << "Please enter new genre name : ";
	str_update_genre_name = validate::validate_string(1, 20);

	try {
		// Attempt to update genre's description in database
		_ptr_class_container.ptr_game_manager.update_genre_name(_obj_genre.get_id(), str_update_genre_name);
		std::cout << "'" << _obj_genre.get_genre() << "' updated to '" << str_update_genre_name << "' successfully.\n";
		_obj_genre.set_genre(str_update_genre_name);
		util::pause();
	}
	catch (std::exception& ex) {
		std::cout << "\nError: " << ex.what() << "\n";
		util::pause();
	}
}

void ManageUsersMenu::execute() {
	KEY_EVENT_RECORD key{};
	int i_highlighted_index = 0;
	HANDLE h_output_console = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE h_input_console = GetStdHandle(STD_INPUT_HANDLE);
	int i_current_page = 0;
	int i_page_size = 10;
	int i_page_count = 0;

	try {
		// Get users form database
		_ptr_class_container.ptr_user_manager.fetch_users();
		std::vector<User>& vec_users = _ptr_class_container.ptr_user_manager.get_vec_users();
		std::vector<User> vec_paged_users;

		// List users to user and allow interaction with them via option
		while (key.wVirtualKeyCode != VK_ESCAPE) {
			system("cls");
			std::cout << "Manage/Update users\n";
			std::cout << "Use [Arrow Keys] to navigate users/pages, press [Enter] to select genre to manage\n";
			std::cout << "Press [Esc] to go back\n";
			std::cout << "Press [F1] to add new user\n\n";

			std::cout << "NOTE: Users cannot be deleted, this is in order to preserve purchase history and maintain data integrity\n";
			std::cout << "WARNING: Ensure that you leave at least one user with an admin status, otherwise once you log out there will not be any more admin users\n\n";

			if (vec_users.size() < 1) {
				// This should not be possible, but leaving it here in case something goes wrong while fetching/paging users.
				std::cout << "There are currently no users to display.\n";
			}
			else {
				i_page_count = ((int)vec_users.size() + i_page_size - 1) / i_page_size;

				// As protection from index overflows, reset current page if it is more than the zero-index adjusted page count
				// This is really a mess, but I can't think of many better ways of doing it.
				if (i_current_page > (i_page_count - 1)) i_current_page = 0;

				int i_final_item = 0;
				if (((i_current_page * 10) + 10) > (int)vec_users.size() - 1) {
					i_final_item = (int)vec_users.size();
				}
				else {
					i_final_item = ((i_current_page * 10) + 10);
				}

				vec_paged_users = std::vector<User>(vec_users.begin() + (i_current_page * 10), vec_users.begin() + i_final_item);

				util::output_users_header();
				util::for_each_iterator(vec_paged_users.begin(), vec_paged_users.end(), 0, [&](int index, User& item) {
					if (i_highlighted_index == index) {
						SetConsoleTextAttribute(h_output_console, BACKGROUND_BLUE | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_INTENSITY);
						util::output_user(item);
						SetConsoleTextAttribute(h_output_console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
					}
					else {
						util::output_user(item);
					}
					});

				std::cout << "\nPage " << i_current_page + 1 << " of " << i_page_count << "\n";
			}

			while (!validate::get_control_char(key, h_input_console));

			switch (key.wVirtualKeyCode)
			{
			case VK_DOWN:
				if (i_highlighted_index < (int)vec_paged_users.size() - 1) i_highlighted_index++;
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
				return;
			case VK_F1:
				// Allow a new user to be added upon F1
				RegisterMenu("Add user", _ptr_class_container).execute();
				_ptr_class_container.ptr_user_manager.fetch_users();
				vec_users = _ptr_class_container.ptr_user_manager.get_vec_users();
				break;
			case VK_RETURN:
				if (vec_users.size() < 1) {
					std::cout << "You cannot manage users when there are none to display.\n";
					util::pause();
					break;
				}

				if ((int)vec_paged_users.size() - 1 >= i_highlighted_index && i_highlighted_index >= 0) {
					// Display management menu for selected user
					system("cls");
					User& obj_user = vec_paged_users[i_highlighted_index];

					ManageUserBaseMenu("Manage user", _ptr_class_container, obj_user).execute();
					_ptr_class_container.ptr_user_manager.fetch_users();
					vec_users = _ptr_class_container.ptr_user_manager.get_vec_users();
					i_highlighted_index = 0;
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
		std::cout << "Error: " << ex.what() << "\n";
		util::pause();
	}
}

void ManageUserBaseMenu::execute() {
	// Display management options for selected user
	MenuContainer obj_menu_container = MenuContainer("");
	obj_menu_container.add_menu_item(std::unique_ptr<MenuItem>(new UpdateUserNameMenu("Update full name", _ptr_class_container, _obj_user)));
	obj_menu_container.add_menu_item(std::unique_ptr<MenuItem>(new UpdateUserAgeMenu("Update age", _ptr_class_container, _obj_user)));
	obj_menu_container.add_menu_item(std::unique_ptr<MenuItem>(new UpdateUserEmailMenu("Update email", _ptr_class_container, _obj_user)));
	obj_menu_container.add_menu_item(std::unique_ptr<MenuItem>(new UpdateUserPasswordMenu("Update password", _ptr_class_container, _obj_user)));
	obj_menu_container.add_menu_item(std::unique_ptr<MenuItem>(new UpdateUserAdminStatusMenu("Update admin status", _ptr_class_container, _obj_user)));

	while (!obj_menu_container.get_exit_menu()) {
		obj_menu_container.set_menu_text("Selected user: " + _obj_user.get_email() + "\nChoose one of the below actions to perform against this user.\nPress [ESC] to cancel\n");
		system("cls");
		obj_menu_container.execute();
	}
}

void UserUpdateOptionsMenu::execute() {
	// Display user self-update options (an admin or user can manage their own details while logged in)
	MenuContainer obj_menu_container = MenuContainer("Updating your account details\nChoose one of the below actions to perform against your account.\nPress [ESC] to cancel\n");
	obj_menu_container.add_menu_item(std::unique_ptr<MenuItem>(new UpdateUserNameMenu("Update full name", _ptr_class_container, _obj_user)));
	obj_menu_container.add_menu_item(std::unique_ptr<MenuItem>(new UpdateUserAgeMenu("Update age", _ptr_class_container, _obj_user)));
	obj_menu_container.add_menu_item(std::unique_ptr<MenuItem>(new UpdateUserEmailMenu("Update email", _ptr_class_container, _obj_user)));
	obj_menu_container.add_menu_item(std::unique_ptr<MenuItem>(new UpdateUserPasswordMenu("Update password", _ptr_class_container, _obj_user)));

	while (!obj_menu_container.get_exit_menu()) {
		system("cls");
		obj_menu_container.execute();
	}
}

void UpdateUserNameMenu::execute() {
	system("cls");
	std::string str_previous_name = _obj_user.get_full_name();
	std::cout << "Updating full name, current name: '" << _obj_user.get_full_name() << "'\n\n";
	std::cout << "Please enter new full name: ";
	_obj_user.set_full_name(validate::validate_string(1, 30));

	try {
		// Attempt to persist user's new full name to database
		_ptr_class_container.ptr_user_manager.update_user_fullname(_obj_user);
		std::cout << "\nFull name updated to '" << _obj_user.get_full_name() << "' successfully.\n";
		util::pause();
	}
	catch (std::exception& ex) {
		_obj_user.set_email(str_previous_name);
		std::cout << "\nError: " << ex.what() << "\n";
		util::pause();
	}
}

void UpdateUserAgeMenu::execute() {
	system("cls");
	int i_previous_age = _obj_user.get_age();
	std::cout << "Updating age, current age: '" << _obj_user.get_age() << "'\n\n";
	std::cout << "Please enter new age: ";
	_obj_user.set_age(validate::validate_int(1, 150));

	try {
		// Attempt to persist user's new age to database
		_ptr_class_container.ptr_user_manager.update_user_age(_obj_user);
		std::cout << "\nAge updated to '" << _obj_user.get_age() << "' successfully.\n";
		util::pause();
	}
	catch (std::exception& ex) {
		_obj_user.set_age(i_previous_age);
		std::cout << "\nError: " << ex.what() << "\n";
		util::pause();
	}
}

void UpdateUserEmailMenu::execute() {
	system("cls");
	std::string str_previous_email = _obj_user.get_email();
	std::cout << "Updating email, current email: '" << _obj_user.get_email() << "'\nPlease note that this email must be unique\n\n";
	std::cout << "Please enter new email: ";
	_obj_user.set_email(validate::validate_string(1, 45));

	try {
		// Attempt to persist user's new email to database
		_ptr_class_container.ptr_user_manager.update_user_email(_obj_user);
		std::cout << "\nEmail updated to '" << _obj_user.get_email() << "' successfully.\n";
		util::pause();
	}
	catch (std::exception& ex) {
		_obj_user.set_email(str_previous_email);
		std::cout << "\nError: " << ex.what() << "\n";
		util::pause();
	}
}

void UpdateUserPasswordMenu::execute() {
	system("cls");
	std::string str_previous_password = _obj_user.get_password();
	std::string str_confirm_password;
	std::cout << "Updating password\n\n";
	std::cout << "Please enter new password: ";
	_obj_user.set_password(validate::validate_string(8, 256));

	std::cout << "\nPlease confirm password: ";
	str_confirm_password = validate::validate_string(8, 256);

	// Reattempt until user enters matching password
	while (_obj_user.get_password() != str_confirm_password) {
		std::cout << "\nPasswords do not match, please try again\n";
		std::cout << "\nPlease confirm password: ";
		str_confirm_password = validate::validate_string(8, 256);
	}

	try {
		// Attempt to persist user's new password to database
		_ptr_class_container.ptr_user_manager.update_user_password(_obj_user);
		std::cout << "\nPassword updated successfully.\n";
		util::pause();
	}
	catch (std::exception& ex) {
		_obj_user.set_password(str_previous_password);
		std::cout << "\nError: " << ex.what() << "\n";
		util::pause();
	}
}

void UpdateUserAdminStatusMenu::execute() {
	HANDLE h_input_console = GetStdHandle(STD_INPUT_HANDLE);
	KEY_EVENT_RECORD key{};
	bool bool_previous_admin_status = _obj_user.get_is_admin();

	system("cls");
	std::cout << "Are you sure you want to change admin status of '" << _obj_user.get_email() << "' from " << (_obj_user.get_is_admin() ? "True" : "False") << " to " << (!_obj_user.get_is_admin() ? "True" : "False") << "?\n";
	std::cout << "Press [Enter] to confirm, or [Esc] to cancel\n";

	// Gets admin to confirm that they want to invert a users admin status from/to an admin
	while (key.wVirtualKeyCode != VK_RETURN) {
		while (!validate::get_control_char(key, h_input_console));

		switch (key.wVirtualKeyCode)
		{
		case VK_RETURN:
			try {
				// Attempt to persist opposite of current admin status
				_obj_user.set_is_admin(!_obj_user.get_is_admin());
				_ptr_class_container.ptr_user_manager.change_user_admin_status(_obj_user);
				std::cout << "\n'" << _obj_user.get_email() << "' admin status successfully updated.\n";
				util::pause();
				break;
			}
			catch (std::exception& ex) {
				_obj_user.set_is_admin(bool_previous_admin_status);
				std::cout << "\nError: " << ex.what() << "\n";
				util::pause();
				return;
			}
			break;
		case VK_ESCAPE:
			return;
		default:
			break;
		}
	}
}

void SelectUserPurchasesViewMenu::execute() {
	KEY_EVENT_RECORD key{};
	int i_highlighted_index = 0;
	HANDLE h_output_console = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE h_input_console = GetStdHandle(STD_INPUT_HANDLE);
	int i_current_page = 0;
	int i_page_size = 10;
	int i_page_count = 0;

	try {
		// Get all users
		_ptr_class_container.ptr_user_manager.fetch_users(true);
		std::vector<User>& vec_users = _ptr_class_container.ptr_user_manager.get_vec_users();
		std::vector<User> vec_paged_users;

		// Display users to user so they can select the user who's purchases they would like to view
		while (key.wVirtualKeyCode != VK_ESCAPE) {
			system("cls");
			std::cout << "Select user who's purchases you would like to view\n";
			std::cout << "Use [Arrow Keys] to navigate users/pages, press [Enter] to view user purchase history\n";
			std::cout << "Press [Esc] to go back\n";
			std::cout << "Press [F1] to generate all user purchases summary\n\n";

			std::cout << "NOTE: There are 3 levels of report you can create/view; all purchases summary, user specific summary or purchase specific summary,\n";
			std::cout << "      you must select a user in order to view the latter two types of summary (user specific and purchase specific).\n";

			if (vec_users.size() < 1) {
				// This should not be possible, but leaving it here in case something goes wrong while fetching/paging users.
				std::cout << "There are currently no non-admin users to display.\n";
			}
			else {
				i_page_count = ((int)vec_users.size() + i_page_size - 1) / i_page_size;

				// As protection from index overflows, reset current page if it is more than the zero-index adjusted page count
				// This is really a mess, but I can't think of many better ways of doing it.
				if (i_current_page > (i_page_count - 1)) i_current_page = 0;

				int i_final_item = 0;
				if (((i_current_page * 10) + 10) > (int)vec_users.size() - 1) {
					i_final_item = (int)vec_users.size();
				}
				else {
					i_final_item = ((i_current_page * 10) + 10);
				}

				vec_paged_users = std::vector<User>(vec_users.begin() + (i_current_page * 10), vec_users.begin() + i_final_item);

				util::output_users_header(false);
				util::for_each_iterator(vec_paged_users.begin(), vec_paged_users.end(), 0, [&](int index, User& item) {
					if (i_highlighted_index == index) {
						SetConsoleTextAttribute(h_output_console, BACKGROUND_BLUE | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_INTENSITY);
						util::output_user(item, false);
						SetConsoleTextAttribute(h_output_console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
					}
					else {
						util::output_user(item, false);
					}
					});

				std::cout << "\nPage " << i_current_page + 1 << " of " << i_page_count << "\n";
			}

			while (!validate::get_control_char(key, h_input_console));

			switch (key.wVirtualKeyCode)
			{
			case VK_DOWN:
				if (i_highlighted_index < (int)vec_paged_users.size() - 1) i_highlighted_index++;
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
				return;
			case VK_F1:
				// View summary of all user purchases
				if (vec_users.size() > 0) AllUserPurchaseSummaryMenu("User purchases summary", _ptr_class_container, vec_users).execute();
				break;
			case VK_RETURN:
				if (vec_users.size() < 1) {
					std::cout << "You cannot select a user when there are none to display.\n";
					util::pause();
					break;
				}

				if ((int)vec_paged_users.size() - 1 >= i_highlighted_index && i_highlighted_index >= 0) {
					// View an individual users purchase history
					system("cls");
					User& obj_user = vec_paged_users[i_highlighted_index];

					ViewUserPurchasesMenu("View user purchases", _ptr_class_container, obj_user).execute();
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
		std::cout << "Error: " << ex.what() << "\n";
		util::pause();
	}
}

void ViewUserPurchasesMenu::execute() {
	KEY_EVENT_RECORD key{};
	int i_highlighted_index = 0;
	HANDLE h_output_console = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE h_input_console = GetStdHandle(STD_INPUT_HANDLE);
	int i_current_page = 0;
	int i_page_size = 10;
	int i_page_count = 0;
	bool bool_admin_status = _ptr_class_container.ptr_user_manager.get_current_user().get_is_admin();

	try {
		// Fetch all purchases of provided user
		_ptr_class_container.ptr_purchase_manager.fetch_purchases(_obj_user);
		std::vector<Purchase>& vec_purchases = _ptr_class_container.ptr_purchase_manager.get_vec_purchases();
		std::vector<Purchase> vec_paged_purchases;

		// List found purchases to user
		while (key.wVirtualKeyCode != VK_ESCAPE) {
			system("cls");
			if (bool_admin_status) {
				std::cout << "Viewing purchase history of: '" << _obj_user.get_email() << "'\n";
			}
			else {
				std::cout << "Select a purchase history you would like to view\n";
			}
			std::cout << "Use [Arrow Keys] to navigate purchases/pages, press [Enter] to view purchase items/summary\n";
			std::cout << "Press [Esc] to go back\n";
			std::cout << "Press [F1] to generate summary of all purchases\n\n";

			if (vec_purchases.size() < 1) {
				// This should not be possible, but leaving it here in case something goes wrong while fetching/paging users.
				std::cout << "There are currently no purchases to display.\n";
			}
			else {
				i_page_count = ((int)vec_purchases.size() + i_page_size - 1) / i_page_size;

				// As protection from index overflows, reset current page if it is more than the zero-index adjusted page count
				// This is really a mess, but I can't think of many better ways of doing it.
				if (i_current_page > (i_page_count - 1)) i_current_page = 0;

				int i_final_item = 0;
				if (((i_current_page * 10) + 10) > (int)vec_purchases.size() - 1) {
					i_final_item = (int)vec_purchases.size();
				}
				else {
					i_final_item = ((i_current_page * 10) + 10);
				}

				vec_paged_purchases = std::vector<Purchase>(vec_purchases.begin() + (i_current_page * 10), vec_purchases.begin() + i_final_item);

				util::output_purchase_header();
				util::for_each_iterator(vec_paged_purchases.begin(), vec_paged_purchases.end(), 0, [&](int index, Purchase& item) {
					if (i_highlighted_index == index) {
						SetConsoleTextAttribute(h_output_console, BACKGROUND_BLUE | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_INTENSITY);
						util::output_purchase(item);
						SetConsoleTextAttribute(h_output_console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
					}
					else {
						util::output_purchase(item);
					}
					});

				std::cout << "\nPage " << i_current_page + 1 << " of " << i_page_count << "\n";
			}

			while (!validate::get_control_char(key, h_input_console));

			switch (key.wVirtualKeyCode)
			{
			case VK_DOWN:
				if (i_highlighted_index < (int)vec_paged_purchases.size() - 1) i_highlighted_index++;
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
				return;
			case VK_F1:
				// Allow summary of purchases to only be shown when there is one or more purchase
				if (vec_purchases.size() > 0) {
					ViewUserPurchasesSummaryMenu("Purchase summary", _ptr_class_container, _obj_user).execute();
				}
				break;
			case VK_RETURN:
				if (vec_purchases.size() < 1) {
					std::cout << "You cannot select a purchase when there are none to display.\n";
					util::pause();
					break;
				}

				if ((int)vec_paged_purchases.size() - 1 >= i_highlighted_index && i_highlighted_index >= 0) {
					// Allow user to go into more detail, and view the individual purchase items for a Purchase
					system("cls");
					Purchase& obj_purchase = vec_paged_purchases[i_highlighted_index];

					ViewUserPurchaseItemsMenu("View user purchase details", _ptr_class_container, obj_purchase).execute();
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
		std::cout << "Error: " << ex.what() << "\n";
		util::pause();
	}
}

void ViewUserPurchaseItemsMenu::execute() {
	KEY_EVENT_RECORD key{};
	HANDLE h_input_console = GetStdHandle(STD_INPUT_HANDLE);

	try {
		// Get purchase details for provided purchase
		_ptr_class_container.ptr_purchase_manager.populate_purchase_details(_obj_purchase);
		std::vector<PurchaseItem>& vec_purchase_items = _obj_purchase.get_vec_purchase_items();

		// Display purchase items to user
		while (key.wVirtualKeyCode != VK_ESCAPE) {
			system("cls");
			std::cout << "Summary of purchase placed on: " << _obj_purchase.get_date() << "\n\n";

			util::output_purchase_item_header();
			util::for_each_iterator(vec_purchase_items.begin(), vec_purchase_items.end(), 0, [&](int index, PurchaseItem& item) {
				util::output_purchase_item(item);
				});

			std::cout << "\n";
			std::cout.precision(2);
			std::cout <<
				std::fixed <<
				std::setw(16) << std::left << "Purchase total: " <<
				std::setw(15) << std::left << _obj_purchase.get_total() <<
				std::setw(19) << std::left << "Total Game copies: " <<
				std::setw(10) << std::left << _obj_purchase.get_total_game_copies() << "\n";

			std::cout << "\nPress [Esc] to go back\n";
			std::cout << "Press [F1] to save this purchase items invoice\n\n";

			while (!validate::get_control_char(key, h_input_console));

			switch (key.wVirtualKeyCode)
			{
			case VK_ESCAPE:
				return;
			case VK_F1:
				// Allow user to save the output as a summary txt file
				ViewUserPurchaseItemsSaveMenu("Save purchase summary", _ptr_class_container, _obj_purchase).execute();
				break;
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

void AllUserPurchaseSummaryMenu::execute() {
	KEY_EVENT_RECORD key{};
	HANDLE h_input_console = GetStdHandle(STD_INPUT_HANDLE);

	try {
		// Allow summary to be viewed until user presses escape
		while (key.wVirtualKeyCode != VK_ESCAPE) {
			double d_all_purchase_total = 0;
			system("cls");
			std::cout << "All user purchases summary\n";
			std::cout << "This summary shows each user and if they have made any purchases displays each purchase with a calculated total.\nSee the end of the report for a grand total/average\n";

			// Assumes users are already popualted if we've got this far
			util::for_each_iterator(_vec_users.begin(), _vec_users.end(), 0, [&](int index, User& user) {
				std::cout << "\nSummary for user: " << user.get_email() << "\n\n";
				// Fetch the purchases of this user
				_ptr_class_container.ptr_purchase_manager.fetch_purchases(user);
				std::vector<Purchase>& vec_user_purchases = _ptr_class_container.ptr_purchase_manager.get_vec_purchases();

				if (vec_user_purchases.size() > 0) {
					std::cout << "Purchases: \n";
					// Output all the user's purchases
					for (Purchase& purchase : vec_user_purchases) {
						std::cout.precision(2);
						std::cout <<
							std::fixed <<
							std::setw(6) << std::left << "Date: " <<
							std::setw(25) << std::left << purchase.get_date() <<
							std::setw(7) << std::left << "Total: " <<
							std::setw(15) << std::left << purchase.get_total() << "\n";
					}

					std::cout.precision(2);
					std::cout <<
						std::fixed <<
						std::setw(23) << std::left << "\nUser purchases total: " <<
						std::setw(15) << std::left << _ptr_class_container.ptr_purchase_manager.get_purchase_grand_total() <<
						std::setw(27) << std::left << "User purchases average: " <<
						std::setw(15) << std::left << _ptr_class_container.ptr_purchase_manager.get_purchase_average() << "\n";

					d_all_purchase_total = d_all_purchase_total + _ptr_class_container.ptr_purchase_manager.get_purchase_grand_total();
				}
				else {
					std::cout << "This user has not yet made any purchases\n";
				}
				std::cout << "__________________________________________________________________________________________\n";
				});

			// Output totals
			std::cout << "\nAll purchases total: " << d_all_purchase_total << "\n";
			std::cout << "All purchases average: " << d_all_purchase_total / _vec_users.size() << "\n";

			std::cout << "\nNOTE: The above average assumes that for users who have made no purchases the total of their purchases is zero.\n";

			std::cout << "\nPress [Esc] to go back\n";
			std::cout << "Press [F1] to generate all user purchases summary\n\n";

			while (!validate::get_control_char(key, h_input_console));

			switch (key.wVirtualKeyCode)
			{
			case VK_ESCAPE:
				return;
			case VK_F1:
				// Allow this summary to be saved to file.
				AllUserPurchaseSaveMenu("Save all user purchase summary", _ptr_class_container, _vec_users).execute();
				break;
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

void ViewUserPurchasesSummaryMenu::execute() {
	KEY_EVENT_RECORD key{};
	HANDLE h_input_console = GetStdHandle(STD_INPUT_HANDLE);

	try {
		// Get all the purchases for a user
		_ptr_class_container.ptr_purchase_manager.fetch_purchases(_obj_user);
		std::vector<Purchase>& vec_purchases = _ptr_class_container.ptr_purchase_manager.get_vec_purchases();

		// Populate the purchase items/details for each of hte purchases 
		for (Purchase& purchase : vec_purchases) {
			_ptr_class_container.ptr_purchase_manager.populate_purchase_details(purchase);
		}

		// List each purchase and it's purchase items/details to user
		while (key.wVirtualKeyCode != VK_ESCAPE) {
			system("cls");
			std::cout << "Purchase summary for: " << _obj_user.get_email() << "\n";
			std::cout << "\nBelow is a summary of each purchase made, the items in the purchase, total of the individual purchase, purchase total, game copies total and a grand total at the end.\n";

			if (vec_purchases.size() < 1) {
				// This should not be possible, but leaving it here in case something goes wrong.
				std::cout << "There are currently no purchases to display.\n";
			}
			else {
				util::for_each_iterator(vec_purchases.begin(), vec_purchases.end(), 0, [&](int index, Purchase& purchase) {
					std::cout << "\n";
					std::cout << "Purchase date: " << purchase.get_date() << "\n";

					util::output_purchase_item_header();
					for (PurchaseItem& purchase_item : purchase.get_vec_purchase_items()) {
						util::output_purchase_item(purchase_item);
					}

					std::cout << "\n";
					std::cout.precision(2);
					std::cout <<
						std::fixed <<
						std::setw(16) << std::left << "Purchase total: " <<
						std::setw(15) << std::left << purchase.get_total() <<
						std::setw(19) << std::left << "Total Game copies: " <<
						std::setw(10) << std::left << purchase.get_total_game_copies() << "\n";
					std::cout << "_______________________________________________________________________________________________________\n";
					});

				// Output totals
				std::cout << std::setprecision(2) << std::fixed << "\nPurchases grand total: " << _ptr_class_container.ptr_purchase_manager.get_purchase_grand_total() << "\n";
				std::cout << std::setprecision(2) << std::fixed << "Purchases grand total (Before VAT): " << _ptr_class_container.ptr_purchase_manager.get_purchase_grand_total() * 0.8 << "\n";
				std::cout << std::setprecision(2) << std::fixed << "Average purchase total: " << _ptr_class_container.ptr_purchase_manager.get_purchase_average() << "\n";
				std::cout << std::setprecision(2) << std::fixed << "Total game copies: " << _ptr_class_container.ptr_purchase_manager.get_total_game_copies() << "\n";

				std::cout << "\nPress [Esc] to go back\n";
				std::cout << "Press [F1] to save this user purchases summary\n";
			}

			while (!validate::get_control_char(key, h_input_console));

			switch (key.wVirtualKeyCode)
			{
			case VK_ESCAPE:
				return;
			case VK_F1:
				// Allow user to save this summary as a text file
				if (vec_purchases.size() > 0) ViewUserPurchasesSaveMenu("Save user purchases summary", _ptr_class_container, _obj_user).execute();
				break;
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

void AllUserPurchaseSaveMenu::execute() {
	std::tm tm_current_datetime = util::get_current_datetime();
	std::string str_current_datetime = util::tm_to_filesafe_str(tm_current_datetime);
	std::string str_file_name = "AllUserPurchasesReport_" + str_current_datetime + ".txt";
	std::filesystem::path path_file_to_write = _ptr_class_container.ptr_purchase_manager.get_saves_path() / str_file_name;

	std::cout << "\nAttempting to save all user purchases report...\n";

	try {
		_ptr_class_container.ptr_purchase_manager.ensure_save_directory_exists();
		// Repeat same output logic ans normal all user purchase summary, but this time so it can be written to a text file
		double d_all_purchase_total = 0;
		std::ofstream of_stream(path_file_to_write);

		of_stream.imbue(std::locale("en_GB"));
		of_stream << "Report generated at: " << std::put_time(&tm_current_datetime, "%c") << "\n";
		of_stream << "Report generated by: " << _ptr_class_container.ptr_user_manager.get_current_user().get_email() << "\n\n";

		of_stream << "All user purchases summary\n";
		of_stream << "This summary shows each user and if they have made any purchases displays each purchase with a calculated total.\nSee the end of the report for a grand total/average\n";

		util::for_each_iterator(_vec_users.begin(), _vec_users.end(), 0, [&](int index, User& user) {
			of_stream << "\nSummary for user: " << user.get_email() << "\n\n";
			_ptr_class_container.ptr_purchase_manager.fetch_purchases(user);
			std::vector<Purchase>& vec_user_purchases = _ptr_class_container.ptr_purchase_manager.get_vec_purchases();

			if (vec_user_purchases.size() > 0) {
				of_stream << "Purchases: \n";
				for (Purchase& purchase : vec_user_purchases) {
					of_stream.precision(2);
					of_stream <<
						std::fixed <<
						std::setw(6) << std::left << "Date: " <<
						std::setw(25) << std::left << purchase.get_date() <<
						std::setw(7) << std::left << "Total: " <<
						std::setw(15) << std::left << purchase.get_total() << "\n";
				}

				of_stream.precision(2);
				of_stream <<
					std::fixed <<
					std::setw(23) << std::left << "\nUser purchases total: " <<
					std::setw(15) << std::left << _ptr_class_container.ptr_purchase_manager.get_purchase_grand_total() <<
					std::setw(27) << std::left << "User purchases average: " <<
					std::setw(15) << std::left << _ptr_class_container.ptr_purchase_manager.get_purchase_average() << "\n";

				d_all_purchase_total = d_all_purchase_total + _ptr_class_container.ptr_purchase_manager.get_purchase_grand_total();
			}
			else {
				of_stream << "This user has not yet made any purchases\n";
			}
			of_stream << "__________________________________________________________________________________________\n";
			});

		of_stream << "\nAll purchases total: " << d_all_purchase_total << "\n";
		of_stream << "All purchases average: " << d_all_purchase_total / _vec_users.size() << "\n";

		of_stream << "\nNOTE: The above average assumes that for users who have made no purchases the total of their purchases is zero.\n";

		of_stream.close();

		std::cout << "All user purchases report saved as " << str_file_name << "\n";
		std::cout << "NOTE: The location for this save is in the saves directory where the GameStock.exe was run from\n\n";
		util::pause();
	}
	catch (std::exception& ex) {
		std::cout << "Error: " << ex.what() << "\n";
		util::pause();
	}
}

void ViewUserPurchasesSaveMenu::execute() {
	std::tm tm_current_datetime = util::get_current_datetime();
	std::string str_current_datetime = util::tm_to_filesafe_str(tm_current_datetime);
	std::string str_file_name = "UserPurchasesReport_" + str_current_datetime + ".txt";
	std::filesystem::path path_file_to_write = _ptr_class_container.ptr_purchase_manager.get_saves_path() / str_file_name;

	std::cout << "\nAttempting to save user purchases report...\n";

	try {
		_ptr_class_container.ptr_purchase_manager.ensure_save_directory_exists();
		// Repeat same logic as normal user purchases summary menu, but this time output to a file. Assume purchases have already had their details populated if we've got this far
		std::vector<Purchase>& vec_purchases = _ptr_class_container.ptr_purchase_manager.get_vec_purchases();
		std::ofstream of_stream(path_file_to_write);

		of_stream.imbue(std::locale("en_GB"));
		of_stream << "Report generated at: " << std::put_time(&tm_current_datetime, "%c") << "\n";
		of_stream << "Report generated by: " << _ptr_class_container.ptr_user_manager.get_current_user().get_email() << "\n\n";

		of_stream << "Purchase summary for: " << _obj_user.get_email() << "\n";
		of_stream << "\nBelow is a summary of each purchase made, the items in the purchase, total of the individual purchase, purchase total, game copies total and a grand total at the end.\n";

		util::for_each_iterator(vec_purchases.begin(), vec_purchases.end(), 0, [&](int index, Purchase& purchase) {
			of_stream << "\n";
			of_stream << "Purchase date: " << purchase.get_date() << "\n";

			util::output_purchase_item_header(of_stream);
			for (PurchaseItem& purchase_item : purchase.get_vec_purchase_items()) {
				util::output_purchase_item(purchase_item, of_stream);
			}

			of_stream << "\n";
			of_stream.precision(2);
			of_stream <<
				std::fixed <<
				std::setw(16) << std::left << "Purchase total: " <<
				std::setw(15) << std::left << purchase.get_total() <<
				std::setw(19) << std::left << "Total Game copies: " <<
				std::setw(10) << std::left << purchase.get_total_game_copies() << "\n";
			of_stream << "_______________________________________________________________________________________________________\n";
			});

		of_stream << std::setprecision(2) << std::fixed << "\nPurchases grand total: " << _ptr_class_container.ptr_purchase_manager.get_purchase_grand_total() << "\n";
		of_stream << std::setprecision(2) << std::fixed << "Purchases grand total (Before VAT): " << _ptr_class_container.ptr_purchase_manager.get_purchase_grand_total() * 0.8 << "\n";
		of_stream << std::setprecision(2) << std::fixed << "Average purchase total: " << _ptr_class_container.ptr_purchase_manager.get_purchase_average() << "\n";
		of_stream << std::setprecision(2) << std::fixed << "Total game copies: " << _ptr_class_container.ptr_purchase_manager.get_total_game_copies() << "\n";

		of_stream.close();

		std::cout << "User purchases report saved as " << str_file_name << "\n";
		std::cout << "NOTE: The location for this save is in the saves directory where the GameStock.exe was run from\n\n";
		util::pause();
	}
	catch (std::exception& ex) {
		std::cout << "Error: " << ex.what() << "\n";
		util::pause();
	}
}

void ViewUserPurchaseItemsSaveMenu::execute() {
	std::tm tm_current_datetime = util::get_current_datetime();
	std::string str_current_datetime = util::tm_to_filesafe_str(tm_current_datetime);
	std::string str_file_name = "PurchaseReport_" + str_current_datetime + ".txt";
	std::filesystem::path path_file_to_write = _ptr_class_container.ptr_purchase_manager.get_saves_path() / str_file_name;

	std::cout << "\nAttempting to save purchase items report...\n";

	try {
		_ptr_class_container.ptr_purchase_manager.ensure_save_directory_exists();
		// Output the same as the purchase items summary, but this time to a file, assuming that purchase items for a provided purchase have already been fetched if we've got this far.
		std::vector<PurchaseItem>& vec_purchase_items = _obj_purchase.get_vec_purchase_items();
		std::ofstream of_stream(path_file_to_write);

		of_stream.imbue(std::locale("en_GB"));
		of_stream << "Report generated at: " << std::put_time(&tm_current_datetime, "%c") << "\n";
		of_stream << "Report generated by: " << _ptr_class_container.ptr_user_manager.get_current_user().get_email() << "\n\n";
		of_stream << "Summary of purchase placed on: " << _obj_purchase.get_date() << "\n";

		util::output_purchase_item_header(of_stream);
		util::for_each_iterator(vec_purchase_items.begin(), vec_purchase_items.end(), 0, [&](int index, PurchaseItem& item) {
			util::output_purchase_item(item, of_stream);
			});

		of_stream << "\n";
		of_stream.precision(2);
		of_stream <<
			std::fixed <<
			std::setw(16) << std::left << "Purchase total: " <<
			std::setw(15) << std::left << _obj_purchase.get_total() <<
			std::setw(19) << std::left << "Total Game copies: " <<
			std::setw(10) << std::left << _obj_purchase.get_total_game_copies() << "\n";

		of_stream.close();

		std::cout << "Purchase Items report saved as " << str_file_name << "\n";
		std::cout << "NOTE: The location for this save is in the saves directory where the GameStock.exe was run from\n\n";
		util::pause();
	}
	catch (std::exception& ex) {
		std::cout << "Error: " << ex.what() << "\n";
		util::pause();
	}
}