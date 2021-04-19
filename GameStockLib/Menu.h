#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include "Utilities.h"
#include "InputValidator.h"
#include "User.h"
#include "PurchaseItem.h"
#include "ClassContainer.h"
#include "Game.h"

class MenuItem {
public:
    virtual ~MenuItem() {}
    virtual std::string item_text() const = 0;
    virtual void execute() = 0;
};

class MenuContainer {
private:
    std::string _str_text;
    std::vector<std::unique_ptr<MenuItem>> _vec_menu_items;
    bool _bool_exit_menu;
    int _i_highlighted_index;
    HANDLE _h_output_console;
    HANDLE _h_input_console;
public:
    MenuContainer(std::string const& text);
    void add_menu_item(std::unique_ptr<MenuItem> item);
    void execute();
    bool get_exit_menu() { return _bool_exit_menu; }
    void set_exit_menu(bool exitMenu) { _bool_exit_menu = exitMenu; }
    void set_menu_text(std::string str_text) { _str_text = str_text; }
};

class GeneralMenuItem : public MenuItem {
private:
    std::string _output;
protected:
    ClassContainer& _ptr_class_container;
public:
    GeneralMenuItem(std::string output, ClassContainer& ptr_class_container);
    std::string item_text() const { return _output; }
};

class DummyMenu : public GeneralMenuItem {
public:
    DummyMenu(std::string output, ClassContainer& ptr_class_container) : GeneralMenuItem(output, ptr_class_container) {};
    void execute();
};

/// <summary>
/// Allows a user to attempt to login to the system.
/// </summary>
class LoginMenu : public GeneralMenuItem {
public:
    LoginMenu(std::string output, ClassContainer& ptr_class_container) : GeneralMenuItem(output, ptr_class_container) {};
    void execute();
};

/// <summary>
/// The menu item that when executed allows a user to register a new user
/// </summary>
class RegisterMenu : public GeneralMenuItem {
public:
    RegisterMenu(std::string output, ClassContainer& ptr_class_container) : GeneralMenuItem(output, ptr_class_container) {};
    void execute();
};

/// <summary>
/// When executed displays the current games to a user/administrator and allows various forms of interaction with the listed games.
/// </summary>
class ViewGamesMenu : public GeneralMenuItem {
public:
    ViewGamesMenu(std::string output, ClassContainer& ptr_class_container) : GeneralMenuItem(output, ptr_class_container) {};
    void execute();
};

/// <summary>
/// Displays the users current basket along with options to perform on the basket.
/// </summary>
class ViewBasketMenu : public GeneralMenuItem {
public:
    ViewBasketMenu(std::string output, ClassContainer& ptr_class_container) : GeneralMenuItem(output, ptr_class_container) {};
    void execute();
};

/// <summary>
/// Base menu of other menus related to game management, and displays these other menus upon execution.
/// </summary>
class ManageGameBaseMenu : public GeneralMenuItem {
protected:
    Game& _obj_game;
public:
    ManageGameBaseMenu(std::string output, ClassContainer& ptr_class_container, Game& obj_game) : GeneralMenuItem(output, ptr_class_container), _obj_game(obj_game) {};
    void execute();
};

/// <summary>
/// Implements ManageGameBaseMenu, used to perform an update to a game name
/// </summary>
class UpdateGameNameMenu : public ManageGameBaseMenu {
public:
    UpdateGameNameMenu(std::string output, ClassContainer& ptr_class_container, Game& obj_game) : ManageGameBaseMenu(output, ptr_class_container, obj_game) {};
    void execute();
};

/// <summary>
/// Implements ManageGameBaseMenu, used to perform an update to a game genre
/// </summary>
class UpdateGameGenreMenu : public ManageGameBaseMenu {
public:
    UpdateGameGenreMenu(std::string output, ClassContainer& ptr_class_container, Game& obj_game) : ManageGameBaseMenu(output, ptr_class_container, obj_game) {};
    void execute();
};

/// <summary>
/// Implements ManageGameBaseMenu, used to perform an update to a game price
/// </summary>
class UpdateGamePriceMenu : public ManageGameBaseMenu {
public:
    UpdateGamePriceMenu(std::string output, ClassContainer& ptr_class_container, Game& obj_game) : ManageGameBaseMenu(output, ptr_class_container, obj_game) {};
    void execute();
};

/// <summary>
/// Implements ManageGameBaseMenu, used to perform an update to a game rating
/// </summary>
class UpdateGameRatingMenu : public ManageGameBaseMenu {
public:
    UpdateGameRatingMenu(std::string output, ClassContainer& ptr_class_container, Game& obj_game) : ManageGameBaseMenu(output, ptr_class_container, obj_game) {};
    void execute();
};

/// <summary>
/// Implements ManageGameBaseMenu, used to perform an update to game copies
/// </summary>
class UpdateGameCopiesMenu : public ManageGameBaseMenu {
public:
    UpdateGameCopiesMenu(std::string output, ClassContainer& ptr_class_container, Game& obj_game) : ManageGameBaseMenu(output, ptr_class_container, obj_game) {};
    void execute();
};

class SubMenuExample : public GeneralMenuItem {
public:
    SubMenuExample(std::string output, ClassContainer& ptr_class_container) : GeneralMenuItem(output, ptr_class_container) {};
    void execute();
};

