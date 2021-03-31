#include <iostream>
#include "Menu.h"

int main()
{
	MenuContainer objMenuContainer = MenuContainer("Welcome to GameStock.\nChoose one of the below options.\n(Esc to exit)\n");
	objMenuContainer.add_menu_item(std::unique_ptr<MenuItem>(new DummyMenu("Login")));
	objMenuContainer.add_menu_item(std::unique_ptr<MenuItem>(new DummyMenu("Register")));
	objMenuContainer.add_menu_item(std::unique_ptr<MenuItem>(new DummyMenu("Example item 3")));
	objMenuContainer.add_menu_item(std::unique_ptr<MenuItem>(new SubMenuExample("Sub menu example")));

	while (!objMenuContainer.get_exit_menu()) {
		system("cls");
		objMenuContainer.execute();
	}
}