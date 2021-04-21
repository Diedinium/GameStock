#include "CppUnitTest.h"
#include "DatabaseManager.h"
#include "DatabaseManager.cpp"
#include <filesystem>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GameStockTests
{
	TEST_CLASS(DatabaseManagerTests)
	{
	public:
		DatabaseManager dbManager;
		std::string testDatabaseName = "testDatabase.db";

		TEST_METHOD(get_database) {
			dbManager.connect(testDatabaseName);

			Assert::IsNotNull(dbManager.get_database());
		}

		TEST_METHOD(get_return_code) {
			dbManager.connect(testDatabaseName);

			Assert::AreEqual(SQLITE_OK, dbManager.get_return_code());
		}

		TEST_METHOD(create_database)
		{
			dbManager.connect(testDatabaseName);
			
			Assert::IsTrue(std::filesystem::exists("database\\" + testDatabaseName));
			Assert::AreEqual(SQLITE_OK, dbManager.get_return_code());
		}

		TEST_METHOD(create_tables_if_not_exist) {
			dbManager.connect(testDatabaseName);
			dbManager.create_tables_if_not_exist();

			Assert::AreEqual(SQLITE_OK, dbManager.get_return_code());
		}

		TEST_METHOD(insert_initial) {
			dbManager.connect(testDatabaseName);
			dbManager.create_tables_if_not_exist();
			dbManager.insert_initial();
			
			sqlite3_stmt* stmt_status;
			std::string str_status_sql = "SELECT * FROM status;";

			sqlite3_prepare_v2(dbManager.get_database(), str_status_sql.c_str(), -1, &stmt_status, NULL);
			int i_return_code = sqlite3_step(stmt_status);
			sqlite3_finalize(stmt_status);

			Assert::AreEqual(SQLITE_OK, dbManager.get_return_code());
			Assert::AreEqual(SQLITE_ROW, i_return_code);
		}

		TEST_METHOD_CLEANUP(test_method_cleanup) {
			sqlite3* db = dbManager.get_database();
			sqlite3_close_v2(db);
		}

		TEST_CLASS_INITIALIZE(init_DatabaseManagerTests) {
			Logger::WriteMessage("DatabaseManagerTests: Initialised and cleaned up previous DB.");
			if (std::filesystem::exists(L"database\\testDatabase.db")) {
				std::filesystem::remove(L"database\\testDatabase.db");
			}
		}
	};
}
