#include "CppUnitTest.h"
#include "Genre.h"
#include "TestUtilities.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GameStockTests
{
	TEST_CLASS(GenreTests)
	{
	public:
		std::string str_genre_name = "Genre name";
		int i_random_id = test_util::generate_random_int_range(1, 1000);

		Genre obj_genre = Genre(i_random_id, str_genre_name);

		TEST_METHOD(default_constructor_test) {
			Genre genre;

			Assert::AreEqual(0, genre.get_id());
			Assert::AreEqual(std::string(""), genre.get_genre());
		}

		TEST_METHOD(constructor_test_1_param) {
			Genre genre(str_genre_name);

			Assert::AreEqual(0, genre.get_id());
			Assert::AreEqual(str_genre_name, genre.get_genre());
		}

		TEST_METHOD(constructor_test_2_param) {
			Genre genre(i_random_id, str_genre_name);

			Assert::AreEqual(i_random_id, genre.get_id());
			Assert::AreEqual(str_genre_name, genre.get_genre());
		}

		TEST_METHOD(get_id) {
			Assert::AreEqual(obj_genre.get_id(), i_random_id);
		}

		TEST_METHOD(get_genre_name) {
			Assert::AreEqual(obj_genre.get_genre(), str_genre_name);
		}

		TEST_METHOD(set_id) {
			int i_random_set_id = test_util::generate_random_int_range(2000, 3000);

			obj_genre.set_id(i_random_set_id);

			Assert::AreEqual(obj_genre.get_id(), i_random_set_id);
		}

		TEST_METHOD(set_genre_name) {
			std::string str_set_genre = "New test name";

			obj_genre.set_genre(str_set_genre);

			Assert::AreEqual(obj_genre.get_genre(), str_set_genre);
		}
	};
}