#include <iostream>
#include "sqlite3.h"
#include "Menu.h"
#include "DatabaseManager.h"
#include "UserManager.h"
#include "ClassContainer.h"

int main()
{
	DatabaseManager obj_database_manager;

	obj_database_manager.create_database("GameStock.db");
	if (obj_database_manager.get_return_code() != SQLITE_OK) {
		std::cout << "Error: " << sqlite3_errmsg(obj_database_manager.get_database());
		return 0;
	}

	obj_database_manager.create_tables_if_not_exist();
	if (obj_database_manager.get_return_code() != SQLITE_OK) {
		std::cout << "Error: " << sqlite3_errmsg(obj_database_manager.get_database());
		return 0;
	}

	obj_database_manager.insert_initial();
	if (obj_database_manager.get_return_code() != SQLITE_OK && obj_database_manager.get_return_code() != SQLITE_ROW) {
		std::cout << "Error: " << sqlite3_errmsg(obj_database_manager.get_database());
		return 0;
	}

	UserManager obj_user_manager = UserManager(obj_database_manager.get_database());

	ClassContainer class_container;
	class_container.ptr_database_manager = &obj_database_manager;
	class_container.ptr_user_manager = &obj_user_manager;

	MenuContainer objMenuContainer = MenuContainer("Welcome to GameStock.\nChoose one of the below options.\n(Esc to exit)\n");
	objMenuContainer.add_menu_item(std::unique_ptr<MenuItem>(new LoginMenu("Login", &class_container)));
	objMenuContainer.add_menu_item(std::unique_ptr<MenuItem>(new RegisterMenu("Register", &class_container)));
	objMenuContainer.add_menu_item(std::unique_ptr<MenuItem>(new DummyMenu("Example item 3", &class_container)));
	objMenuContainer.add_menu_item(std::unique_ptr<MenuItem>(new SubMenuExample("Sub menu example", &class_container)));

	while (!objMenuContainer.get_exit_menu()) {
		system("cls");
		objMenuContainer.execute();
	}
}