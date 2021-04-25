#include "CppUnitTest.h"
#include "Game.h"
#include "TestUtilities.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GameStockTests
{
	TEST_CLASS(GameTests)
	{
	public:
		std::string str_game_name = "Test game name";
		std::string str_genre_name = "Genre name";
		std::string str_rating_name = "Rating name";
		double d_random_price = test_util::generate_random_double_range(1.0, 250.50);
		int i_random_copies = test_util::generate_random_int_range(1, 160);
		int i_random_id = test_util::generate_random_int_range(1, 1000);

		Game obj_game = Game(i_random_id, str_game_name, Genre(str_genre_name), Rating(str_rating_name), d_random_price, i_random_copies);

		TEST_METHOD(default_constructor_test) {
			Game game;

			Assert::AreEqual(0, game.get_id());
			Assert::AreEqual(std::string(""), game.get_name());
			Assert::AreEqual(0.0, game.get_price());
			Assert::AreEqual(0, game.get_copies());
		}

		TEST_METHOD(constructor_test_3_param) {
			Game game = Game(str_game_name, Genre(str_genre_name), Rating(str_rating_name));

			Assert::AreEqual(0, game.get_id());
			Assert::AreEqual(str_game_name, game.get_name());
			Assert::AreEqual(0.0, game.get_price());
			Assert::AreEqual(0, game.get_copies());
			Assert::AreEqual(str_genre_name, game.get_genre().get_genre());
			Assert::AreEqual(str_rating_name, game.get_rating().get_rating());
		}

		TEST_METHOD(constructor_test_5_param) {
			Game game = Game(str_game_name, Genre(str_genre_name), Rating(str_rating_name), d_random_price, i_random_copies);

			Assert::AreEqual(0, game.get_id());
			Assert::AreEqual(str_game_name, game.get_name());
			Assert::AreEqual(d_random_price, game.get_price());
			Assert::AreEqual(i_random_copies, game.get_copies());
			Assert::AreEqual(str_genre_name, game.get_genre().get_genre());
			Assert::AreEqual(str_rating_name, game.get_rating().get_rating());
		}

		TEST_METHOD(constructor_test_6_param) {
			Game game = Game(i_random_id, str_game_name, Genre(str_genre_name), Rating(str_rating_name), d_random_price, i_random_copies);

			Assert::AreEqual(i_random_id, game.get_id());
			Assert::AreEqual(str_game_name, game.get_name());
			Assert::AreEqual(d_random_price, game.get_price());
			Assert::AreEqual(i_random_copies, game.get_copies());
			Assert::AreEqual(str_genre_name, game.get_genre().get_genre());
			Assert::AreEqual(str_rating_name, game.get_rating().get_rating());
		}

		TEST_METHOD(get_id) {
			Assert::AreEqual(obj_game.get_id(), i_random_id);
		}

		TEST_METHOD(get_name) {
			Assert::AreEqual(obj_game.get_name(), str_game_name);
		}

		TEST_METHOD(get_genre) {
			Assert::AreEqual(obj_game.get_genre().get_genre(), str_genre_name);
		}

		TEST_METHOD(get_rating) {
			Assert::AreEqual(obj_game.get_rating().get_rating(), str_rating_name);
		}

		TEST_METHOD(get_price) {
			Assert::AreEqual(obj_game.get_price(), d_random_price);
		}

		TEST_METHOD(get_copies) {
			Assert::AreEqual(obj_game.get_copies(), i_random_copies);
		}

		TEST_METHOD(set_id) {
			int i_random_set_id = test_util::generate_random_int_range(2000, 3000);

			obj_game.set_id(i_random_set_id);

			Assert::AreEqual(obj_game.get_id(), i_random_set_id);
		}

		TEST_METHOD(set_name) {
			std::string str_set_name = "New test name";

			obj_game.set_name(str_set_name);

			Assert::AreEqual(obj_game.get_name(), str_set_name);
		}

		TEST_METHOD(set_genre) {
			Genre obj_set_genre = Genre("New test genre name");

			obj_game.set_genre(obj_set_genre);

			Assert::AreEqual(obj_game.get_genre().get_genre(), obj_set_genre.get_genre());
		}

		TEST_METHOD(set_rating) {
			Rating obj_set_rating = Rating("New test genre name");

			obj_game.set_rating(obj_set_rating);

			Assert::AreEqual(obj_game.get_rating().get_rating(), obj_set_rating.get_rating());
		}

		TEST_METHOD(set_price) {
			double d_random_set_price = test_util::generate_random_double_range(100, 300);

			obj_game.set_price(d_random_set_price);

			Assert::AreEqual(obj_game.get_price(), d_random_set_price);
		}

		TEST_METHOD(set_copies) {
			int i_random_set_copies = test_util::generate_random_int_range(100, 300);

			obj_game.set_copies(i_random_set_copies);

			Assert::AreEqual(obj_game.get_copies(), i_random_set_copies);
		}
	};
}