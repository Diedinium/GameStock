#pragma once
#include "UserManager.h"
#include "DatabaseManager.h"
#include "GameManager.h"
#include "PurchaseManager.h"

struct ClassContainer
{
	DatabaseManager& ptr_database_manager;
	UserManager& ptr_user_manager;
	GameManager& ptr_game_manager;
	PurchaseManager& ptr_purchase_manager;
};

