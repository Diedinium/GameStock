#include "CppUnitTest.h"
#include "PurchaseItem.h"
#include "TestUtilities.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GameStockTests
{
	TEST_CLASS(PurchaseItemTests)
	{
	public:
		std::string str_game_name = "Game test name";
		std::string str_genre_name = "Genre test name";
		std::string str_rating_name = "Rating test name";
		std::string str_date = "Pretend I'm a date :)";
		int i_random_id = test_util::generate_random_int_range(1, 1000);
		int i_random_purchase_id= test_util::generate_random_int_range(1, 1000);
		int i_random_game_id= test_util::generate_random_int_range(1, 1000);
		int i_random_count = test_util::generate_random_int_range(1, 60);
		double d_random_price = test_util::generate_random_double_range(1, 1000);
		double d_random_total = test_util::generate_random_double_range(1, 1000);

		PurchaseItem obj_purchase_item = PurchaseItem(i_random_id, i_random_purchase_id, i_random_game_id, Game(str_game_name, Genre(str_genre_name), Rating(str_rating_name)), i_random_count, d_random_price);

		TEST_METHOD(constructor_test_3_param) {
			PurchaseItem purchase_item(i_random_game_id, i_random_count, d_random_price);

			Assert::AreEqual(0, purchase_item.get_id());
			Assert::AreEqual(i_random_game_id, purchase_item.get_game_id());
			Assert::AreEqual(0, purchase_item.get_purchase_id());
			Assert::AreEqual(i_random_count, purchase_item.get_count());
			Assert::AreEqual(d_random_price, purchase_item.get_price());
			Assert::AreEqual((double)i_random_count * d_random_price, purchase_item.get_total());
		}

		TEST_METHOD(constructor_test_4_param) {
			PurchaseItem purchase_item(i_random_game_id, Game(str_game_name, Genre(str_genre_name), Rating(str_rating_name)), i_random_count, d_random_price);

			Assert::AreEqual(0, purchase_item.get_id());
			Assert::AreEqual(i_random_game_id, purchase_item.get_game_id());
			Assert::AreEqual(0, purchase_item.get_purchase_id());
			Assert::AreEqual(i_random_count, purchase_item.get_count());
			Assert::AreEqual(d_random_price, purchase_item.get_price());
			Assert::AreEqual((double)i_random_count * d_random_price, purchase_item.get_total());
			Assert::AreEqual(str_game_name, purchase_item.get_game().get_name());
		}

		TEST_METHOD(constructor_test_6_param) {
			PurchaseItem purchase_item(i_random_id, i_random_purchase_id, i_random_game_id, Game(str_game_name, Genre(str_genre_name), Rating(str_rating_name)), i_random_count, d_random_price);

			Assert::AreEqual(i_random_id, purchase_item.get_id());
			Assert::AreEqual(i_random_game_id, purchase_item.get_game_id());
			Assert::AreEqual(i_random_purchase_id, purchase_item.get_purchase_id());
			Assert::AreEqual(i_random_count, purchase_item.get_count());
			Assert::AreEqual(d_random_price, purchase_item.get_price());
			Assert::AreEqual((double)i_random_count * d_random_price, purchase_item.get_total());
			Assert::AreEqual(str_game_name, purchase_item.get_game().get_name());
		}

		TEST_METHOD(constructor_test_7_param) {
			PurchaseItem purchase_item(i_random_id, str_game_name, d_random_price, str_genre_name, str_rating_name, i_random_count, d_random_total);

			Assert::AreEqual(i_random_id, purchase_item.get_id());
			Assert::AreEqual(0, purchase_item.get_game_id());
			Assert::AreEqual(0, purchase_item.get_purchase_id());
			Assert::AreEqual(i_random_count, purchase_item.get_count());
			Assert::AreEqual(d_random_price, purchase_item.get_price());
			Assert::AreEqual(d_random_total, purchase_item.get_total());
			Assert::AreEqual(str_game_name, purchase_item.get_game().get_name());
			Assert::AreEqual(str_genre_name, purchase_item.get_game().get_genre().get_genre());
			Assert::AreEqual(str_rating_name, purchase_item.get_game().get_rating().get_rating());
		}

		TEST_METHOD(get_id) {
			Assert::AreEqual(obj_purchase_item.get_id(), i_random_id);
		}

		TEST_METHOD(get_purchase_id) {
			Assert::AreEqual(obj_purchase_item.get_purchase_id(), i_random_purchase_id);
		}

		TEST_METHOD(get_game_id) {
			Assert::AreEqual(obj_purchase_item.get_game_id(), i_random_game_id);
		}

		TEST_METHOD(get_game) {
			Assert::AreEqual(obj_purchase_item.get_game().get_name(), str_game_name);
		}

		TEST_METHOD(get_price) {
			Assert::AreEqual(obj_purchase_item.get_price(), d_random_price);
		}

		TEST_METHOD(get_total) {
			Assert::AreEqual(obj_purchase_item.get_total(), (double)i_random_count * d_random_price);
		}

		TEST_METHOD(get_total_before_vat) {
			Assert::AreEqual(obj_purchase_item.get_total_before_vat(), ((double)i_random_count * d_random_price) * 0.8);
		}

		TEST_METHOD(get_count) {
			Assert::AreEqual(obj_purchase_item.get_count(), i_random_count);
		}

		TEST_METHOD(set_count) {
			int i_random_set_count = test_util::generate_random_int_range(1, 100);

			obj_purchase_item.set_count(i_random_set_count);

			Assert::AreEqual(obj_purchase_item.get_count(), i_random_set_count);
		}
	};
}