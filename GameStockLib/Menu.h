#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include "Utilities.h"
#include "InputValidator.h"
#include "User.h"
#include "ClassContainer.h"

class MenuItem {
public:
    virtual ~MenuItem() {}
    virtual std::string item_text() const = 0;
    virtual void execute() = 0;
};

class MenuContainer {
private:
    std::string _strText;
    std::vector<std::unique_ptr<MenuItem>> _vecMenuItems;
    bool _boolExitMenu;
    int _iHighlightedIndex;
    HANDLE _hOutputConsole;
    HANDLE _hInputConsole;
public:
    MenuContainer(std::string const& text);
    void add_menu_item(std::unique_ptr<MenuItem> item);
    void execute();
    bool get_exit_menu() { return _boolExitMenu; }
    void set_exit_menu(bool exitMenu) { _boolExitMenu = exitMenu; }
};

class GeneralMenuItem : public MenuItem {
private:
    std::string _output;
protected:
    ClassContainer* _ptr_class_container;
public:
    GeneralMenuItem(std::string output, ClassContainer* ptr_class_container);
    std::string item_text() const { return _output; }
};

class DummyMenu : public GeneralMenuItem {
public:
    DummyMenu(std::string output, ClassContainer* ptr_class_container) : GeneralMenuItem(output, ptr_class_container) {};
    void execute();
};

/// <summary>
/// Allows a user to attempt to login to the system.
/// </summary>
class LoginMenu : public GeneralMenuItem {
public:
    LoginMenu(std::string output, ClassContainer* ptr_class_container) : GeneralMenuItem(output, ptr_class_container) {};
    void execute();
};

/// <summary>
/// The menu item that when executed allows a user to register a new user
/// </summary>
class RegisterMenu : public GeneralMenuItem {
public:
    RegisterMenu(std::string output, ClassContainer* ptr_class_container) : GeneralMenuItem(output, ptr_class_container) {};
    void execute();
};

class SubMenuExample : public GeneralMenuItem {
public:
    SubMenuExample(std::string output, ClassContainer* ptr_class_container) : GeneralMenuItem(output, ptr_class_container) {};
    void execute();
};

