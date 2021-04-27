#pragma once
#include "UserManager.h"
#include "DatabaseManager.h"
#include "GameManager.h"
#include "PurchaseManager.h"

/// <summary>
/// Used as a nice convenient way to group the various manager classes togehter so that they can be accessed/used from various menu pages
/// </summary>
struct ClassContainer
{
	DatabaseManager& ptr_database_manager;
	UserManager& ptr_user_manager;
	GameManager& ptr_game_manager;
	PurchaseManager& ptr_purchase_manager;
};

