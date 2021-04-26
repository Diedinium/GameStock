#include "CppUnitTest.h"
#include "PurchaseManager.h"
#include "DatabaseManager.h"
#include "TestUtilities.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GameStockTests
{
	TEST_CLASS(PurchaseManagerTests)
	{
	public:
		DatabaseManager obj_db_manager;
		std::string test_database_name = "testDatabase.db";
		PurchaseManager obj_purchase_manager = PurchaseManager(NULL);

		TEST_METHOD_INITIALIZE(init_test) {
			char* errorMessage;
			obj_db_manager.connect(test_database_name);
			obj_db_manager.create_tables_if_not_exist();
			obj_db_manager.insert_initial();
			obj_purchase_manager = PurchaseManager(obj_db_manager.get_database());

			std::string str_insert_sql =
				"INSERT INTO purchases(user_id, total) VALUES (2, 5000);" \
				"INSERT INTO purchase_items(purchase_id, game_name, game_price, game_genre, game_rating, count) VALUES(1, 'Test Game 1', 1000, 1, 1, 3);" \
				"INSERT INTO purchase_items(purchase_id, game_name, game_price, game_genre, game_rating, count) VALUES(1, 'Test Game 2', 1000, 1, 1, 2);" \
				"INSERT INTO purchases(user_id, total) VALUES (2, 10000);" \
				"INSERT INTO purchase_items(purchase_id, game_name, game_price, game_genre, game_rating, count) VALUES(2, 'Test Game 3', 500, 1, 1, 8);" \
				"INSERT INTO purchase_items(purchase_id, game_name, game_price, game_genre, game_rating, count) VALUES(2, 'Test Game 4', 500, 1, 1, 12);";

			sqlite3_exec(obj_db_manager.get_database(), str_insert_sql.c_str(), NULL, NULL, &errorMessage);
		}

		TEST_METHOD(get_vec_purchases) {
			// Arrange
			User user;
			user.set_id(2);

			// Act
			obj_purchase_manager.fetch_purchases(user);

			// Assert
			Assert::AreEqual(1, obj_purchase_manager.get_vec_purchases()[0].get_id());
			Assert::AreEqual(2, obj_purchase_manager.get_vec_purchases()[1].get_id());
		}

		TEST_METHOD(fetch_purchases) {
			// Arrange
			User user;
			user.set_id(2);

			// Act
			obj_purchase_manager.fetch_purchases(user);

			// Assert
			Assert::AreEqual(2, (int)obj_purchase_manager.get_vec_purchases().size());
		}

		TEST_METHOD(populate_purchase_details) {
			// Arrange
			Purchase purchase(2, 10000, "Some date");

			// Act
			obj_purchase_manager.populate_purchase_details(purchase);

			// Assert
			Assert::AreEqual(2, (int)purchase.get_vec_purchase_items().size());
		}

		TEST_METHOD(get_purchase_grand_total) {
			// Arrange
			User user;
			user.set_id(2);

			// Act
			obj_purchase_manager.fetch_purchases(user);

			// Assert
			Assert::AreEqual((double)15000, obj_purchase_manager.get_purchase_grand_total());
		}

		TEST_METHOD(get_purchase_average) {
			// Arrange
			User user;
			user.set_id(2);

			// Act
			obj_purchase_manager.fetch_purchases(user);

			// Assert
			Assert::AreEqual((double)7500, obj_purchase_manager.get_purchase_average());
		}

		TEST_METHOD(get_total_game_copies) {
			// Arrange
			User user;
			user.set_id(2);

			// Act
			obj_purchase_manager.fetch_purchases(user);

			for (Purchase& purchase : obj_purchase_manager.get_vec_purchases()) {
				obj_purchase_manager.populate_purchase_details(purchase);
			}

			// Assert
			Assert::AreEqual(25, obj_purchase_manager.get_total_game_copies());
		}

		TEST_METHOD(get_saves_path) {
			std::filesystem::path p_test_path = "saves";

			// Act/Assert
			Assert::AreEqual(p_test_path.string(), obj_purchase_manager.get_saves_path().string());
		}

		TEST_METHOD(ensure_save_directory_exists) {
			Assert::IsFalse(std::filesystem::exists("saves"));

			//Act
			obj_purchase_manager.ensure_save_directory_exists();

			// Act/Assert
			Assert::IsTrue(std::filesystem::exists("saves"));
		}

		TEST_METHOD_CLEANUP(cleanup_test) {
			sqlite3_close_v2(obj_db_manager.get_database());

			if (std::filesystem::exists("database\\testDatabase.db")) {
				std::filesystem::remove("database\\testDatabase.db");
			}

			if (std::filesystem::exists("saves")) {
				std::filesystem::remove_all("saves");
			}
		}
	};
}