#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>
#include <Windows.h>
#include "Utilities.h"
#include "InputValidator.h"
#include "User.h"
#include "Purchase.h"
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
/// When executed displays the current games to a user/administrator and allows various forms of interaction with the listed games.
/// </summary>
class AddGameMenu : public GeneralMenuItem {
public:
    AddGameMenu(std::string output, ClassContainer& ptr_class_container) : GeneralMenuItem(output, ptr_class_container) {};
    void execute();
};

/// <summary>
/// When executed displays the current games to a user/administrator and allows various forms of interaction with the listed games.
/// </summary>
class SelectGenreFilterMenu : public GeneralMenuItem {
public:
    SelectGenreFilterMenu(std::string output, ClassContainer& ptr_class_container) : GeneralMenuItem(output, ptr_class_container) {};
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
/// When executed allows admin to delete the currently selected game.
/// </summary>
class DeleteGameMenu : public ManageGameBaseMenu {
private:
    MenuContainer& _obj_menu_container;
public:
    DeleteGameMenu(std::string output, ClassContainer& ptr_class_container, Game& obj_game, MenuContainer& obj_menu_container) : ManageGameBaseMenu(output, ptr_class_container, obj_game), _obj_menu_container(obj_menu_container) {};
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

/// <summary>
/// Allows an admin level user to manage available game genres
/// </summary>
class ManageGenresMenu : public GeneralMenuItem {
public:
    ManageGenresMenu(std::string output, ClassContainer& ptr_class_container) : GeneralMenuItem(output, ptr_class_container) {};
    void execute();
};

/// <summary>
/// Allows admin user to specify new game genre to add.
/// </summary>
class AddGenreMenu : public GeneralMenuItem {
public:
    AddGenreMenu(std::string output, ClassContainer& ptr_class_container) : GeneralMenuItem(output, ptr_class_container) {};
    void execute();
};

/// <summary>
/// Base menu of other menus related to genre management, and displays these other menu options upon execution.
/// </summary>
class ManageGenreBaseMenu : public GeneralMenuItem {
protected:
    Genre& _obj_genre;
public:
    ManageGenreBaseMenu(std::string output, ClassContainer& ptr_class_container, Genre& obj_genre) : GeneralMenuItem(output, ptr_class_container), _obj_genre(obj_genre) {};
    void execute();
};

/// <summary>
/// Implements ManageGameBaseMenu, used to perform an update to a game name
/// </summary>
class UpdateGenreNameMenu : public ManageGenreBaseMenu {
public:
    UpdateGenreNameMenu(std::string output, ClassContainer& ptr_class_container, Genre& obj_genre) : ManageGenreBaseMenu(output, ptr_class_container, obj_genre) {};
    void execute();
};

/// <summary>
/// Implements ManageGameBaseMenu, used to perform an update to a game name
/// </summary>
class DeleteGenreMenu : public ManageGenreBaseMenu {
private:
    MenuContainer& _obj_menu_container;
public:
    DeleteGenreMenu(std::string output, ClassContainer& ptr_class_container, Genre& obj_genre, MenuContainer& obj_menu_container) : ManageGenreBaseMenu(output, ptr_class_container, obj_genre), _obj_menu_container(obj_menu_container) {};
    void execute();
};

/// <summary>
/// Displays all users in system and allows each to be selected in order to update them.
/// </summary>
class ManageUsersMenu : public GeneralMenuItem {
public:
    ManageUsersMenu(std::string output, ClassContainer& ptr_class_container) : GeneralMenuItem(output, ptr_class_container) {};
    void execute();
};

/// <summary>
/// Base menu of other menus related to user management, and displays these other menu options upon execution.
/// </summary>
class ManageUserBaseMenu : public GeneralMenuItem {
protected:
    User& _obj_user;
public:
    ManageUserBaseMenu(std::string output, ClassContainer& ptr_class_container, User& obj_user) : GeneralMenuItem(output, ptr_class_container), _obj_user(obj_user) {};
    void execute();
};

/// <summary>
/// Implements ManageUserBaseMenu, used to perform an update to a user name
/// </summary>
class UserUpdateOptionsMenu : public ManageUserBaseMenu {
public:
    UserUpdateOptionsMenu(std::string output, ClassContainer& ptr_class_container, User& obj_user) : ManageUserBaseMenu(output, ptr_class_container, obj_user) {};
    void execute();
};

/// <summary>
/// Implements ManageUserBaseMenu, used to perform an update to a user name
/// </summary>
class UpdateUserNameMenu : public ManageUserBaseMenu {
public:
    UpdateUserNameMenu(std::string output, ClassContainer& ptr_class_container, User& obj_user) : ManageUserBaseMenu(output, ptr_class_container, obj_user) {};
    void execute();
};

/// <summary>
/// Implements ManageUserBaseMenu, used to perform an update to a user age
/// </summary>
class UpdateUserAgeMenu : public ManageUserBaseMenu {
public:
    UpdateUserAgeMenu(std::string output, ClassContainer& ptr_class_container, User& obj_user) : ManageUserBaseMenu(output, ptr_class_container, obj_user) {};
    void execute();
};

/// <summary>
/// Implements ManageUserBaseMenu, used to perform an update to a user email
/// </summary>
class UpdateUserEmailMenu : public ManageUserBaseMenu {
public:
    UpdateUserEmailMenu(std::string output, ClassContainer& ptr_class_container, User& obj_user) : ManageUserBaseMenu(output, ptr_class_container, obj_user) {};
    void execute();
};

/// <summary>
/// Implements ManageUserBaseMenu, used to perform an update to a user password
/// </summary>
class UpdateUserPasswordMenu : public ManageUserBaseMenu {
public:
    UpdateUserPasswordMenu(std::string output, ClassContainer& ptr_class_container, User& obj_user) : ManageUserBaseMenu(output, ptr_class_container, obj_user) {};
    void execute();
};

/// <summary>
/// Implements ManageUserBaseMenu, used to perform an update to user admin status
/// </summary>
class UpdateUserAdminStatusMenu : public ManageUserBaseMenu {
public:
    UpdateUserAdminStatusMenu(std::string output, ClassContainer& ptr_class_container, User& obj_user) : ManageUserBaseMenu(output, ptr_class_container, obj_user) {};
    void execute();
};

class SelectUserPurchasesViewMenu : public GeneralMenuItem {
public:
    SelectUserPurchasesViewMenu(std::string output, ClassContainer& ptr_class_container) : GeneralMenuItem(output, ptr_class_container) {};
    void execute();
};

class ViewUserPurchasesMenu : public GeneralMenuItem {
protected:
    User& _obj_user;
public:
    ViewUserPurchasesMenu(std::string output, ClassContainer& ptr_class_container, User& obj_user) : GeneralMenuItem(output, ptr_class_container), _obj_user(obj_user) {};
    void execute();
};

class ViewUserPurchaseItemsMenu : public GeneralMenuItem {
protected:
    Purchase& _obj_purchase;
public:
    ViewUserPurchaseItemsMenu(std::string output, ClassContainer& ptr_class_container, Purchase& obj_purchase) : GeneralMenuItem(output, ptr_class_container), _obj_purchase(obj_purchase) {};
    void execute();
};

class AllUserPurchaseSummaryMenu : public GeneralMenuItem {
protected:
    std::vector<User>& _vec_users;
public:
    AllUserPurchaseSummaryMenu(std::string output, ClassContainer& ptr_class_container, std::vector<User>& vec_users) : GeneralMenuItem(output, ptr_class_container), _vec_users(vec_users) {};
    void execute();
};

class ViewUserPurchasesSummaryMenu : public ViewUserPurchasesMenu {
public:
    ViewUserPurchasesSummaryMenu(std::string output, ClassContainer& ptr_class_container, User& obj_user) : ViewUserPurchasesMenu(output, ptr_class_container, obj_user) {};
    void execute();
};

class AllUserPurchaseSaveMenu : public AllUserPurchaseSummaryMenu {
public:
    AllUserPurchaseSaveMenu(std::string output, ClassContainer& ptr_class_container, std::vector<User>& vec_users) : AllUserPurchaseSummaryMenu(output, ptr_class_container, vec_users) {};
    void execute();
};

class ViewUserPurchasesSaveMenu : public ViewUserPurchasesMenu {
public:
    ViewUserPurchasesSaveMenu(std::string output, ClassContainer& ptr_class_container, User& obj_user) : ViewUserPurchasesMenu(output, ptr_class_container, obj_user) {};
    void execute();
};

class ViewUserPurchaseItemsSaveMenu : public ViewUserPurchaseItemsMenu {
public:
    ViewUserPurchaseItemsSaveMenu(std::string output, ClassContainer& ptr_class_container, Purchase& obj_purchase) : ViewUserPurchaseItemsMenu(output, ptr_class_container, obj_purchase) {};
    void execute();
};

