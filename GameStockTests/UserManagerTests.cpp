#include "CppUnitTest.h"
#include "UserManager.h"
#include "DatabaseManager.h"
#include "sqlite3.h"
#include "TestUtilities.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GameStockTests
{
	TEST_CLASS(UserManagerTests)
	{
	public:
		DatabaseManager obj_db_manager;
		std::string test_database_name = "testDatabase.db";
		UserManager obj_user_manager = UserManager(NULL);
		int i_random_age = test_util::generate_random_int_range(1, 50);
		std::string str_user_full_name = "Test User";
		std::string str_user_email = "test@test.com";
		std::string str_user_password = "securepassword";

		TEST_METHOD_INITIALIZE(init_test) {
			obj_db_manager.connect(test_database_name);
			obj_db_manager.create_tables_if_not_exist();
			obj_db_manager.insert_initial();
			obj_user_manager = UserManager(obj_db_manager.get_database());
		}

		TEST_METHOD(register_user) {
			// Arrange
			sqlite3_stmt* stmt_user_query;
			User user(str_user_full_name, i_random_age, str_user_email, str_user_password, false);

			// Act
			obj_user_manager.register_user(user);

			// Assert
			sqlite3_prepare_v2(obj_db_manager.get_database(), ("SELECT * FROM users WHERE email = '" + user.get_email() + "' AND password = '" + user.get_password() + "'").c_str(), -1, &stmt_user_query, NULL);

			if (sqlite3_step(stmt_user_query) != SQLITE_ROW) {
				sqlite3_finalize(stmt_user_query);
				Assert::Fail(L"Registered user could not be found in DB");
			}

			Assert::AreEqual(3, sqlite3_column_int(stmt_user_query, 0));
			Assert::AreEqual(str_user_full_name, std::string((char*)sqlite3_column_text(stmt_user_query, 1)));
			Assert::AreEqual(i_random_age, sqlite3_column_int(stmt_user_query, 2));
			Assert::AreEqual(0, sqlite3_column_int(stmt_user_query, 5));

			sqlite3_finalize(stmt_user_query);
		}

		TEST_METHOD(attempt_login) {
			// Arrange
			User user(str_user_full_name, i_random_age, str_user_email, str_user_password, false);

			// Act
			obj_user_manager.register_user(user);
			obj_user_manager.attempt_login(user);

			//Assert
			Assert::AreEqual(3, obj_user_manager.get_current_user().get_id());
			Assert::AreEqual(str_user_email, obj_user_manager.get_current_user().get_email());
			Assert::AreEqual(str_user_full_name, obj_user_manager.get_current_user().get_full_name());
			Assert::AreEqual(i_random_age, obj_user_manager.get_current_user().get_age());
			Assert::AreEqual(str_user_password, obj_user_manager.get_current_user().get_password());
			Assert::IsFalse(obj_user_manager.get_current_user().get_is_admin());
		}

		TEST_METHOD(attempt_login_invalid) {
			// Arrange
			User user(str_user_full_name, i_random_age, "invalid@email.com", "invalidpassword", false);

			// Act/Assert
			Assert::ExpectException<std::invalid_argument>([&] {
					obj_user_manager.attempt_login(user);
				});
		}

		TEST_METHOD_CLEANUP(cleanup_test) {
			sqlite3_close_v2(obj_db_manager.get_database());
			if (std::filesystem::exists(L"database\\testDatabase.db")) {
				std::filesystem::remove(L"database\\testDatabase.db");
			}
		}
	};
}