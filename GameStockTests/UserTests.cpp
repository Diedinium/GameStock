#include "CppUnitTest.h"
#include "User.h"
#include "TestUtilities.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GameStockTests
{
	TEST_CLASS(UserTests)
	{
	public:
		int i_random_id = test_util::generate_random_int_range(1, 1000);
		int i_random_age = test_util::generate_random_int_range(1, 1000);
		std::string str_full_name = "Full test name";
		std::string str_email = "Test email";
		std::string str_password = "Test password";
		bool bool_admin_status = true;

		User obj_user = User(i_random_id, str_full_name, i_random_age, str_email, str_password, bool_admin_status);

		TEST_METHOD(default_constructor_test) {
			User user;

			Assert::AreEqual(0, user.get_id());
			Assert::AreEqual(0, user.get_age());
			Assert::AreEqual(std::string(""), user.get_email());
			Assert::AreEqual(std::string(""), user.get_full_name());
			Assert::AreEqual(std::string(""), user.get_password());
			Assert::IsFalse(user.get_is_admin());
		}

		TEST_METHOD(constructor_test_5_param) {
			User user(str_full_name, i_random_age, str_email, str_password, bool_admin_status);

			Assert::AreEqual(0, user.get_id());
			Assert::AreEqual(i_random_age, user.get_age());
			Assert::AreEqual(str_email, user.get_email());
			Assert::AreEqual(str_full_name, user.get_full_name());
			Assert::AreEqual(str_password, user.get_password());
			Assert::IsTrue(user.get_is_admin());
		}

		TEST_METHOD(constructor_test_6_param) {
			User user(i_random_id, str_full_name, i_random_age, str_email, str_password, bool_admin_status);

			Assert::AreEqual(i_random_id, user.get_id());
			Assert::AreEqual(i_random_age, user.get_age());
			Assert::AreEqual(str_email, user.get_email());
			Assert::AreEqual(str_full_name, user.get_full_name());
			Assert::AreEqual(str_password, user.get_password());
			Assert::IsTrue(user.get_is_admin());
		}

		TEST_METHOD(get_id) {
			Assert::AreEqual(obj_user.get_id(), i_random_id);
		}

		TEST_METHOD(get_age) {
			Assert::AreEqual(obj_user.get_age(), i_random_age);
		}

		TEST_METHOD(get_email) {
			Assert::AreEqual(obj_user.get_email(), str_email);
		}

		TEST_METHOD(get_full_name) {
			Assert::AreEqual(obj_user.get_full_name(), str_full_name);
		}

		TEST_METHOD(get_password) {
			Assert::AreEqual(obj_user.get_password(), str_password);
		}

		TEST_METHOD(get_is_admin) {
			Assert::IsTrue(obj_user.get_is_admin());
		}

		TEST_METHOD(set_id) {
			int i_random_set_id = test_util::generate_random_int_range(1, 100);

			obj_user.set_id(i_random_set_id);

			Assert::AreEqual(obj_user.get_id(), i_random_set_id);
		}

		TEST_METHOD(set_age) {
			int i_random_set_age = test_util::generate_random_int_range(1, 100);

			obj_user.set_age(i_random_set_age);

			Assert::AreEqual(obj_user.get_age(), i_random_set_age);
		}

		TEST_METHOD(set_email) {
			std::string str_set_email = "New test email";

			obj_user.set_email(str_set_email);

			Assert::AreEqual(obj_user.get_email(), str_set_email);
		}

		TEST_METHOD(set_full_name) {
			std::string str_set_full_name = "New test name";

			obj_user.set_full_name(str_set_full_name);

			Assert::AreEqual(obj_user.get_full_name(), str_set_full_name);
		}

		TEST_METHOD(set_password) {
			std::string str_set_password = "New test password";

			obj_user.set_password(str_set_password);
			
			Assert::AreEqual(obj_user.get_password(), str_set_password);
		}

		TEST_METHOD(set_is_admin) {
			obj_user.set_is_admin(false);

			Assert::IsFalse(obj_user.get_is_admin());
		}
	};
}