#include "CppUnitTest.h"
#include "Purchase.h"
#include "TestUtilities.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GameStockTests
{
	TEST_CLASS(PurchaseTests)
	{
	public:
		std::string str_date = "Pretend I'm a date :)";
		int i_random_id = test_util::generate_random_int_range(1, 1000);
		int i_random_user_id = test_util::generate_random_int_range(1, 1000);
		int i_random_count = test_util::generate_random_int_range(1, 60);
		double d_random_total = test_util::generate_random_double_range(1, 1000);
		std::vector<PurchaseItem> vec_purchase_items = {
			PurchaseItem(test_util::generate_random_int_range(1, 1000), i_random_count, test_util::generate_random_double_range(1, 1000)),
			PurchaseItem(test_util::generate_random_int_range(1, 1000), i_random_count, test_util::generate_random_double_range(1, 1000)),
			PurchaseItem(test_util::generate_random_int_range(1, 1000), i_random_count, test_util::generate_random_double_range(1, 1000)),
			PurchaseItem(test_util::generate_random_int_range(1, 1000), i_random_count, test_util::generate_random_double_range(1, 1000))
		};

		Purchase obj_purchase = Purchase(i_random_id, i_random_user_id, vec_purchase_items, d_random_total, str_date);

		TEST_METHOD(default_constructor_test) {
			Purchase purchase;

			Assert::AreEqual(0, purchase.get_id());
			Assert::AreEqual(0, purchase.get_user_id());
			Assert::AreEqual(0.0, purchase.get_total());
			Assert::AreEqual(std::string(""), purchase.get_date());
			Assert::AreEqual(0, (int)purchase.get_vec_purchase_items().size());
		}

		TEST_METHOD(constructor_test_2_param) {
			Purchase purchase(i_random_user_id, vec_purchase_items);

			Assert::AreEqual(0, purchase.get_id());
			Assert::AreEqual(i_random_user_id, purchase.get_user_id());
			Assert::AreEqual(0.0, purchase.get_total());
			Assert::AreEqual(std::string(""), purchase.get_date());
			Assert::AreEqual(4, (int)purchase.get_vec_purchase_items().size());
		}

		TEST_METHOD(constructor_test_3_param) {
			Purchase purchase(i_random_id, d_random_total, str_date);

			Assert::AreEqual(i_random_id, purchase.get_id());
			Assert::AreEqual(0, purchase.get_user_id());
			Assert::AreEqual(d_random_total, purchase.get_total());
			Assert::AreEqual(str_date, purchase.get_date());
			Assert::AreEqual(0, (int)purchase.get_vec_purchase_items().size());
		}

		TEST_METHOD(constructor_test_5_param) {
			Purchase purchase(i_random_id, i_random_user_id, vec_purchase_items, d_random_total, str_date);

			Assert::AreEqual(i_random_id, purchase.get_id());
			Assert::AreEqual(i_random_user_id, purchase.get_user_id());
			Assert::AreEqual(d_random_total, purchase.get_total());
			Assert::AreEqual(str_date, purchase.get_date());
			Assert::AreEqual(4, (int)purchase.get_vec_purchase_items().size());
		}

		TEST_METHOD(get_id) {
			Assert::AreEqual(obj_purchase.get_id(), i_random_id);
		}

		TEST_METHOD(get_user_id) {
			Assert::AreEqual(obj_purchase.get_user_id(), i_random_user_id);
		}

		TEST_METHOD(get_vec_purchase_items) {
			Assert::AreEqual(obj_purchase.get_vec_purchase_items().size(), vec_purchase_items.size());
		}

		TEST_METHOD(get_total) {
			Assert::AreEqual(obj_purchase.get_total(), d_random_total);
		}

		TEST_METHOD(get_date) {
			Assert::AreEqual(obj_purchase.get_date(), str_date);
		}

		TEST_METHOD(get_total_game_copies) {
			Assert::AreEqual(obj_purchase.get_total_game_copies(), i_random_count * 4);
		}

		TEST_METHOD(set_id) {
			int i_random_set_id = test_util::generate_random_int_range(2000, 3000);

			obj_purchase.set_id(i_random_set_id);

			Assert::AreEqual(obj_purchase.get_id(), i_random_set_id);
		}

		TEST_METHOD(set_user_id) {
			int i_random_set_user_id = test_util::generate_random_int_range(2000, 3000);

			obj_purchase.set_user_id(i_random_set_user_id);

			Assert::AreEqual(obj_purchase.get_user_id(), i_random_set_user_id);
		}

		TEST_METHOD(set_total) {
			double d_random_set_total = test_util::generate_random_double_range(2000, 3000);

			obj_purchase.set_total(d_random_set_total);

			Assert::AreEqual(obj_purchase.get_total(), d_random_set_total);
		}

		TEST_METHOD(set_date) {
			std::string str_set_date = "New test date";

			obj_purchase.set_date(str_set_date);

			Assert::AreEqual(obj_purchase.get_date(), str_set_date);
		}
	};
}