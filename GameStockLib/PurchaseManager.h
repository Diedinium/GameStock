#pragma once
#include "sqlite3.h"
#include "Purchase.h"
#include "PurchaseItem.h"

class PurchaseManager
{
	sqlite3* _db;
public:
	PurchaseManager(sqlite3* db) { _db = db; }
};

