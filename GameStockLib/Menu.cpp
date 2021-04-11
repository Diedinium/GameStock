#include "Menu.h"

MenuContainer::MenuContainer(std::string const& text) : _strText(text), _vecMenuItems()
{
	_boolExitMenu = false;
	_iHighlightedIndex = 0;
	_hOutputConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	_hInputConsole = GetStdHandle(STD_INPUT_HANDLE);
}

void MenuContainer::execute()
{
	KEY_EVENT_RECORD key{};

	while (key.wVirtualKeyCode != VK_RETURN) {
		system("cls");
		std::cout << _strText << "\n";
		util::for_each_iterator(_vecMenuItems.begin(), _vecMenuItems.end(), 0, [&](int index, std::unique_ptr<MenuItem> const& item) {
			if (_iHighlightedIndex == index) {
				SetConsoleTextAttribute(_hOutputConsole, BACKGROUND_BLUE | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_INTENSITY);
				std::cout << "\t" << item->item_text() << "\n";
				SetConsoleTextAttribute(_hOutputConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
			}
			else {
				std::cout << "\t" << item->item_text() << "\n";
			}
			});
		while (!validate::get_control_char(key, _hInputConsole));
		if (key.wVirtualKeyCode == VK_DOWN) {
			if (_iHighlightedIndex < (int)_vecMenuItems.size() - 1) _iHighlightedIndex++;
		}
		else if (key.wVirtualKeyCode == VK_UP) {
			if (_iHighlightedIndex > 0) _iHighlightedIndex--;
		}
		else if (key.wVirtualKeyCode == VK_ESCAPE) {
			this->set_exit_menu(true);
			break;
		}
	}

	if ((int)_vecMenuItems.size() > _iHighlightedIndex && _iHighlightedIndex >= 0) {
		if (key.wVirtualKeyCode != VK_ESCAPE) {
			_vecMenuItems[_iHighlightedIndex]->execute();
		}
	}
	else {
		std::cout << "Not a valid option, please try again.\n";
		util::pause();
	}
};

void MenuContainer::add_menu_item(std::unique_ptr<MenuItem> item) {
	_vecMenuItems.push_back(std::move(item));
};

GeneralMenuItem::GeneralMenuItem(std::string output, ClassContainer* ptr_class_container)
{
	_output = output;
	_ptr_class_container = ptr_class_container;
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

	std::cout << "Please enter your email: ";
	obj_user.set_email(validate::validate_string());

	std::cout << "Please enter your password: ";
	obj_user.set_password(validate::validate_string());

	try {
		_ptr_class_container->ptr_user_manager->attempt_login(&obj_user);

		MenuContainer objMenuContainer = MenuContainer("Logged in as " + obj_user.get_email() + ".\nChoose one of the below options.\n(Esc to logout)\n");
		objMenuContainer.add_menu_item(std::unique_ptr<MenuItem>(new DummyMenu("Example item 1", _ptr_class_container)));
		objMenuContainer.add_menu_item(std::unique_ptr<MenuItem>(new DummyMenu("Example item 2", _ptr_class_container)));

		while (!objMenuContainer.get_exit_menu()) {
			system("cls");
			objMenuContainer.execute();
		}

		_ptr_class_container->ptr_user_manager->logout();
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
	obj_user.set_age(validate::validate_int());

	std::cout << "Please enter your email: ";
	obj_user.set_email(validate::validate_string());

	std::cout << "Please enter your password: ";
	obj_user.set_password(validate::validate_string());

	std::cout << "Please confirm your password: ";
	str_password_confirm = validate::validate_string();

	while (str_password_confirm != obj_user.get_password()) {
		std::cout << "Passwords do not match, please confirm your password: ";
		str_password_confirm = validate::validate_string();
	}

	try {
		_ptr_class_container->ptr_user_manager->register_user(&obj_user);

		std::cout << "\nRegistration complete. User name is " << obj_user.get_email() << ".\nPlease now attempt to log in on the next screen.\n";

		util::pause();
	}
	catch (std::exception& ex) {
		std::cout << "\nCould not register: " << ex.what() << "\n";
		std::cout << "\nPlease try again.\n";
		util::pause();
	}
}

void SubMenuExample::execute() {
	MenuContainer objMenuContainer = MenuContainer("Sub menu example.\nChoose one of the below options.\n");
	objMenuContainer.add_menu_item(std::unique_ptr<MenuItem>(new DummyMenu("Example item 1", _ptr_class_container)));
	objMenuContainer.add_menu_item(std::unique_ptr<MenuItem>(new DummyMenu("Example item 2", _ptr_class_container)));

	while (!objMenuContainer.get_exit_menu()) {
		system("cls");
		objMenuContainer.execute();
	}
}