#include "CppUnitTest.h"
#include "GameManager.h"
#include "DatabaseManager.h"
#include "TestUtilities.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GameStockTests
{
	TEST_CLASS(GameManagerTests)
	{
	public:
		DatabaseManager obj_db_manager;
		std::string test_database_name = "testDatabase.db";
		GameManager obj_game_manager = GameManager(NULL);

		TEST_METHOD_INITIALIZE(init_test) {
			obj_db_manager.connect(test_database_name);
			obj_db_manager.create_tables_if_not_exist();
			obj_db_manager.insert_initial();
			obj_game_manager = GameManager(obj_db_manager.get_database());
		}

		TEST_METHOD(initialise_games) {
			// Act
			obj_game_manager.initialise_games();

			// Assert
			Assert::AreEqual(4, (int)obj_game_manager.get_vec_games().size());
		}

		TEST_METHOD(refresh_games) {
			// Act
			obj_game_manager.refresh_games();

			// Assert
			Assert::AreEqual(4, (int)obj_game_manager.get_vec_games().size());
		}

		TEST_METHOD(get_vec_games) {
			// Act
			obj_game_manager.initialise_games();

			// Assert
			Assert::AreEqual(1, obj_game_manager.get_vec_games()[0].get_id());
			Assert::AreEqual(4, (int)obj_game_manager.get_vec_games()[3].get_id());
		}

		TEST_METHOD_CLEANUP(cleanup_test) {
			sqlite3_close_v2(obj_db_manager.get_database());

			if (std::filesystem::exists("database\\testDatabase.db")) {
				std::filesystem::remove("database\\testDatabase.db");
			}
		}
	};
}