#include "CppUnitTest.h"
#include "Rating.h"
#include "TestUtilities.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GameStockTests
{
	TEST_CLASS(RatingTests)
	{
	public:
		std::string str_rating_name = "Rating test name";
		int i_random_id = test_util::generate_random_int_range(1, 1000);

		Rating obj_rating = Rating(i_random_id, str_rating_name);

		TEST_METHOD(default_constructor_test) {
			Rating rating;

			Assert::AreEqual(0, rating.get_id());
			Assert::AreEqual(std::string(""), rating.get_rating());
		}

		TEST_METHOD(constructor_test_1_param) {
			Rating rating(str_rating_name);

			Assert::AreEqual(0, rating.get_id());
			Assert::AreEqual(str_rating_name, rating.get_rating());
		}

		TEST_METHOD(constructor_test_2_param) {
			Rating rating(i_random_id, str_rating_name);

			Assert::AreEqual(i_random_id, rating.get_id());
			Assert::AreEqual(str_rating_name, rating.get_rating());
		}

		TEST_METHOD(get_id) {
			Assert::AreEqual(obj_rating.get_id(), i_random_id);
		}

		TEST_METHOD(get_rating_name) {
			Assert::AreEqual(obj_rating.get_rating(), str_rating_name);
		}

		TEST_METHOD(set_id) {
			int i_random_set_id = test_util::generate_random_int_range(2000, 3000);

			obj_rating.set_id(i_random_set_id);

			Assert::AreEqual(obj_rating.get_id(), i_random_set_id);
		}

		TEST_METHOD(set_rating_name) {
			std::string str_set_rating = "New test name";

			obj_rating.set_rating(str_set_rating);

			Assert::AreEqual(obj_rating.get_rating(), str_set_rating);
		}
	};
}