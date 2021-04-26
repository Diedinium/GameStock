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

		TEST_METHOD(register_user_duplicate_email_error) {
			// Arrange
			User user(str_user_full_name, i_random_age, str_user_email, str_user_password, false);

			// Act/Assert
			obj_user_manager.register_user(user);
			Assert::ExpectException<std::invalid_argument>([&] {
				obj_user_manager.register_user(user);
				});
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

		TEST_METHOD(logout) {
			// Arrange
			User user(str_user_full_name, i_random_age, str_user_email, str_user_password, false);

			// Act
			obj_user_manager.register_user(user);
			obj_user_manager.attempt_login(user);
			obj_user_manager.logout();

			//Assert
			Assert::AreEqual(0, obj_user_manager.get_current_user().get_id());
			Assert::IsFalse(obj_user_manager.is_login_valid());
		}

		TEST_METHOD(is_login_valid) {
			// Arrange
			User user(str_user_full_name, i_random_age, str_user_email, str_user_password, false);

			// Act
			obj_user_manager.register_user(user);
			obj_user_manager.attempt_login(user);

			//Assert
			Assert::IsTrue(obj_user_manager.is_login_valid());
		}

		TEST_METHOD(set_login_valid) {
			// Act
			obj_user_manager.set_login_valid(true);

			//Assert
			Assert::IsTrue(obj_user_manager.is_login_valid());
		}

		TEST_METHOD(fetch_users) {
			// Arrange
			User user(str_user_full_name, i_random_age, str_user_email, str_user_password, false);

			// Act
			obj_user_manager.register_user(user);
			obj_user_manager.fetch_users();

			//Assert
			Assert::AreEqual(3, (int)obj_user_manager.get_vec_users().size());
		}

		TEST_METHOD(fetch_users_no_admin) {
			// Arrange
			User user(str_user_full_name, i_random_age, str_user_email, str_user_password, false);

			// Act
			obj_user_manager.register_user(user);
			obj_user_manager.fetch_users(true);

			//Assert
			Assert::AreEqual(2, (int)obj_user_manager.get_vec_users().size());
		}

		TEST_METHOD(get_vec_users) {
			// Arrange
			User user(str_user_full_name, i_random_age, str_user_email, str_user_password, false);

			// Act
			obj_user_manager.register_user(user);
			obj_user_manager.fetch_users();

			//Assert
			Assert::AreEqual(3, (int)obj_user_manager.get_vec_users().size());
		}

		TEST_METHOD(update_user_password) {
			// Arrange
			std::string str_new_password = "changedpassword";
			User user;
			user.set_id(2);
			user.set_password(str_new_password);

			// Act
			obj_user_manager.update_user_password(user);
			obj_user_manager.fetch_users();

			//Assert
			Assert::AreEqual(str_new_password, obj_user_manager.get_vec_users()[1].get_password());
		}

		TEST_METHOD(update_user_age) {
			// Arrange
			int i_new_age = test_util::generate_random_int_range(80, 100);
			User user;
			user.set_id(2);
			user.set_age(i_new_age);

			// Act
			obj_user_manager.update_user_age(user);
			obj_user_manager.fetch_users();

			//Assert
			Assert::AreEqual(i_new_age, obj_user_manager.get_vec_users()[1].get_age());
		}

		TEST_METHOD(update_user_fullname) {
			// Arrange
			std::string str_new_fullname = "New Full Name";
			User user;
			user.set_id(2);
			user.set_full_name(str_new_fullname);

			// Act
			obj_user_manager.update_user_fullname(user);
			obj_user_manager.fetch_users();

			//Assert
			Assert::AreEqual(str_new_fullname, obj_user_manager.get_vec_users()[1].get_full_name());
		}

		TEST_METHOD(update_user_email) {
			// Arrange
			std::string str_new_email = "newemail@email.com";
			User user;
			user.set_id(2);
			user.set_email(str_new_email);

			// Act
			obj_user_manager.update_user_email(user);
			obj_user_manager.fetch_users();

			//Assert
			Assert::AreEqual(str_new_email, obj_user_manager.get_vec_users()[1].get_email());
		}

		TEST_METHOD(update_user_email_duplicate_error) {
			// Arrange
			std::string str_new_email = "admin@gamestock.com";
			User user;
			user.set_id(2);
			user.set_email(str_new_email);

			// Act/Assert
			Assert::ExpectException<std::runtime_error>([&] {
				obj_user_manager.update_user_email(user);
				});
		}

		TEST_METHOD(change_user_admin_status) {
			// Arrange
			User user;
			user.set_id(2);
			user.set_is_admin(true);

			// Act
			obj_user_manager.change_user_admin_status(user);
			obj_user_manager.fetch_users();

			//Assert
			Assert::IsTrue(obj_user_manager.get_vec_users()[1].get_is_admin());
		}

		TEST_METHOD_CLEANUP(cleanup_test) {
			sqlite3_close_v2(obj_db_manager.get_database());
			if (std::filesystem::exists(L"database\\testDatabase.db")) {
				std::filesystem::remove(L"database\\testDatabase.db");
			}
		}
	};
}