#include "CppUnitTest.h"
#include "DatabaseManager.h"
#include "DatabaseManager.cpp"
#include <filesystem>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GameStockTests
{
	TEST_CLASS(GameStockTests)
	{
	public:
		DatabaseManager dbManager;
		std::string testDatabaseName = "testDatabase.db";

		TEST_METHOD(CreateDatabase)
		{
			dbManager.create_database(testDatabaseName);
			dbManager.create_tables_if_not_exist();

			Assert::IsTrue(std::filesystem::exists("database\\" + testDatabaseName));
			Assert::AreEqual(SQLITE_OK, dbManager.get_return_code());
		}

		TEST_CLASS_CLEANUP(initDatabaseManagerTests) {
			if (!std::filesystem::exists(L"database")) {
				std::filesystem::remove_all(L"database");
			}
		}
	};
}
