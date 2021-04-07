#pragma once
#include "UserManager.h"
#include "DatabaseManager.h"

struct ClassContainer
{
	DatabaseManager* ptr_database_manager;
	UserManager* ptr_user_manager;
};

