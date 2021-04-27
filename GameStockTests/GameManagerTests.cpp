#include "CppUnitTest.h"
#include "GameManager.h"
#include "DatabaseManager.h"
#include "PurchaseManager.h"
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
		PurchaseManager obj_purchase_manager = PurchaseManager(NULL);
		int i_random_basket_user_id = test_util::generate_random_int_range(1, 200);
		int i_random_purchase_item_game_id = test_util::generate_random_int_range(500, 2500);
		std::string str_game_name = "Test game name";
		std::string str_genre_name = "Test genre name";
		double d_random_game_price = test_util::generate_random_double_range(10.10, 120.5);
		int i_random_game_copies = test_util::generate_random_int_range(5, 200);
		int i_random_genre_id = test_util::generate_random_int_range(1, 10);
		int i_random_rating_id = test_util::generate_random_int_range(1, 6);
		int i_random_game_id = test_util::generate_random_int_range(250, 1000);
		int i_random_valid_game_id = test_util::generate_random_int_range(1, 4);

		TEST_METHOD_INITIALIZE(init_test) {
			obj_db_manager.connect(test_database_name);
			obj_db_manager.create_tables_if_not_exist();
			obj_db_manager.insert_initial();
			obj_game_manager = GameManager(obj_db_manager.get_database());
			obj_purchase_manager = PurchaseManager(obj_db_manager.get_database());
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

		TEST_METHOD(get_basket) {
			// Act/Assert
			Assert::AreEqual(0, (int)obj_game_manager.get_basket().get_vec_purchase_items().size());
		}

		TEST_METHOD(set_basket_user) {
			// Act
			obj_game_manager.set_basket_user(i_random_basket_user_id);

			// Assert
			Assert::AreEqual(i_random_basket_user_id, obj_game_manager.get_basket().get_user_id());
		}

		TEST_METHOD(add_basket_item) {
			// Arrange
			obj_game_manager.refresh_games();
			Game& game = obj_game_manager.get_vec_games()[2];

			// Act
			obj_game_manager.add_basket_item(PurchaseItem(game.get_id(), game, 5, game.get_price()));

			// Assert
			Assert::AreEqual(game.get_id(), obj_game_manager.get_basket().get_vec_purchase_items()[0].get_game_id());
			Assert::AreEqual(1, (int)obj_game_manager.get_basket().get_vec_purchase_items().size());
		}

		TEST_METHOD(add_basket_item_add_to_existing) {
			// Arrange
			obj_game_manager.refresh_games();
			Game& game = obj_game_manager.get_vec_games()[2];

			// Act
			obj_game_manager.add_basket_item(PurchaseItem(game.get_id(), game, 5, game.get_price()));
			obj_game_manager.add_basket_item(PurchaseItem(game.get_id(), game, 5, game.get_price()));

			// Assert
			Assert::AreEqual(game.get_id(), obj_game_manager.get_basket().get_vec_purchase_items()[0].get_game_id());
			Assert::AreEqual(10, obj_game_manager.get_basket().get_vec_purchase_items()[0].get_count());
			Assert::AreEqual(1, (int)obj_game_manager.get_basket().get_vec_purchase_items().size());
		}

		TEST_METHOD(add_basket_item_error_if_count_exceeded) {
			// Arrange
			obj_game_manager.refresh_games();
			Game& game = obj_game_manager.get_vec_games()[2];

			// Act
			obj_game_manager.add_basket_item(PurchaseItem(game.get_id(), game, 5, game.get_price()));

			// Assert
			Assert::ExpectException<std::runtime_error>([&] {
				obj_game_manager.add_basket_item(PurchaseItem(game.get_id(), game, 1500, game.get_price()));
				});
		}

		TEST_METHOD(remove_basket_item) {
			// Arrange
			obj_game_manager.refresh_games();
			Game& game = obj_game_manager.get_vec_games()[2];

			// Act
			obj_game_manager.add_basket_item(PurchaseItem(game.get_id(), game, 5, game.get_price()));
			obj_game_manager.remove_basket_item(game.get_id());

			// Assert
			Assert::AreEqual(0, (int)obj_game_manager.get_basket().get_vec_purchase_items().size());
		}

		TEST_METHOD(remove_basket_item_error) {
			// Act/Assert
			Assert::ExpectException<std::invalid_argument>([&] {
				obj_game_manager.remove_basket_item(i_random_purchase_item_game_id);
				});
		}

		TEST_METHOD(get_basket_total) {
			// Arrange
			obj_game_manager.refresh_games();
			Game& game = obj_game_manager.get_vec_games()[1];
			Game& game1 = obj_game_manager.get_vec_games()[2];
			double d_expected_total = (game.get_price() * (double)5) + (game1.get_price() * (double)2);

			// Act
			obj_game_manager.add_basket_item(PurchaseItem(game.get_id(), game, 5, game.get_price()));
			obj_game_manager.add_basket_item(PurchaseItem(game1.get_id(), game, 2, game1.get_price()));

			// Assert
			Assert::AreEqual(d_expected_total, obj_game_manager.get_basket_total());
			Assert::AreEqual(2, (int)obj_game_manager.get_basket().get_vec_purchase_items().size());
		}

		TEST_METHOD(reset_basket) {
			// Arrange
			obj_game_manager.refresh_games();
			Game& game = obj_game_manager.get_vec_games()[1];
			Game& game1 = obj_game_manager.get_vec_games()[2];

			// Act
			obj_game_manager.add_basket_item(PurchaseItem(game.get_id(), game, 5, game.get_price()));
			obj_game_manager.add_basket_item(PurchaseItem(game1.get_id(), game, 2, game1.get_price()));
			obj_game_manager.reset_basket();

			// Assert
			Assert::AreEqual((double)0, obj_game_manager.get_basket_total());
			Assert::AreEqual(0, (int)obj_game_manager.get_basket().get_vec_purchase_items().size());
		}

		TEST_METHOD(get_admin_flag) {
			// Act/Assert
			Assert::IsFalse(obj_game_manager.get_admin_flag());
		}

		TEST_METHOD(set_admin_flag) {
			// Act
			obj_game_manager.set_admin_flag(true);

			// Act/Assert
			Assert::IsTrue(obj_game_manager.get_admin_flag());
		}

		TEST_METHOD(set_initialised) {
			// Act
			obj_game_manager.set_initialised(true);
			obj_game_manager.initialise_games();

			// Act/Assert
			Assert::AreEqual(0, (int)obj_game_manager.get_vec_games().size());
		}

		TEST_METHOD(add_game) {
			// Arrange
			Game game(str_game_name, Genre(i_random_genre_id, ""), Rating(i_random_rating_id, ""), d_random_game_price, i_random_game_copies);

			// Act
			obj_game_manager.add_game(game);
			obj_game_manager.initialise_games();

			// Assert 
			Assert::AreEqual(str_game_name, obj_game_manager.get_vec_games()[4].get_name());
			Assert::AreEqual(i_random_genre_id, obj_game_manager.get_vec_games()[4].get_genre().get_id());
			Assert::AreEqual(i_random_rating_id, obj_game_manager.get_vec_games()[4].get_rating().get_id());
			Assert::AreEqual(d_random_game_price, obj_game_manager.get_vec_games()[4].get_price());
			Assert::AreEqual(i_random_game_copies, obj_game_manager.get_vec_games()[4].get_copies());
		}

		TEST_METHOD(delete_game) {
			// Arrange
			Game game;
			game.set_id(3);

			// Act
			obj_game_manager.delete_game(game);
			obj_game_manager.initialise_games();

			// Assert 
			Assert::AreEqual(3, (int)obj_game_manager.get_vec_games().size());
		}

		TEST_METHOD(update_game_name) {
			// Act
			obj_game_manager.update_game_name(i_random_valid_game_id, str_game_name);
			obj_game_manager.initialise_games();

			Game updated_game;
			for (Game& game : obj_game_manager.get_vec_games()) {
				if (game.get_id() == i_random_valid_game_id) updated_game = game;
			}

			// Assert
			Assert::AreEqual(str_game_name, updated_game.get_name());
		}

		TEST_METHOD(update_game_genre) {
			// Act
			obj_game_manager.update_game_genre(i_random_valid_game_id, i_random_genre_id);
			obj_game_manager.initialise_games();

			Game updated_game;
			for (Game& game : obj_game_manager.get_vec_games()) {
				if (game.get_genre().get_id() == i_random_genre_id && game.get_id() == i_random_valid_game_id) updated_game = game;
			}

			// Assert
			Assert::AreEqual(i_random_genre_id, updated_game.get_genre().get_id());
		}

		TEST_METHOD(update_game_price) {
			// Act
			obj_game_manager.update_game_price(i_random_valid_game_id, d_random_game_price);
			obj_game_manager.initialise_games();

			Game updated_game;
			for (Game& game : obj_game_manager.get_vec_games()) {
				if (game.get_price() == d_random_game_price && game.get_id() == i_random_valid_game_id) updated_game = game;
			}

			// Assert
			Assert::AreEqual(d_random_game_price, updated_game.get_price());
		}

		TEST_METHOD(update_game_rating) {
			// Act
			obj_game_manager.update_game_rating(i_random_valid_game_id, i_random_rating_id);
			obj_game_manager.initialise_games();

			Game updated_game;
			for (Game& game : obj_game_manager.get_vec_games()) {
				if (game.get_rating().get_id() == i_random_rating_id && game.get_id() == i_random_valid_game_id) updated_game = game;
			}

			// Assert
			Assert::AreEqual(i_random_rating_id, updated_game.get_rating().get_id());
		}

		TEST_METHOD(update_game_copies) {
			// Act
			obj_game_manager.update_game_copies(i_random_valid_game_id, i_random_game_copies);
			obj_game_manager.initialise_games();

			Game updated_game;
			for (Game& game : obj_game_manager.get_vec_games()) {
				if (game.get_copies() == i_random_game_copies && game.get_id() == i_random_valid_game_id) updated_game = game;
			}

			// Assert
			Assert::AreEqual(i_random_game_copies, updated_game.get_copies());
		}

		TEST_METHOD(add_genre) {
			// Arrange
			Genre genre(str_genre_name);

			// Act
			obj_game_manager.add_genre(genre);
			Genre added_genre;
			for (Genre genre_result : obj_game_manager.get_genres()) {
				if (genre_result.get_id() == 11) added_genre = genre_result;
			}

			// Assert 
			Assert::AreEqual(str_genre_name, added_genre.get_genre());
			Assert::AreEqual(11, added_genre.get_id());
		}

		TEST_METHOD(delete_genre) {
			// Arrange
			Genre genre;
			genre.set_id(i_random_genre_id);

			// Act
			obj_game_manager.delete_genre(genre);
			auto genres = obj_game_manager.get_genres();

			// Assert 
			Assert::AreEqual(9, (int)genres.size());
		}

		TEST_METHOD(update_genre_name) {
			// Act
			obj_game_manager.update_genre_name(i_random_genre_id, str_genre_name);
			obj_game_manager.initialise_games();

			Genre updated_genre;
			for (Genre genre : obj_game_manager.get_genres()) {
				if (genre.get_id() == i_random_genre_id) updated_genre = genre;
			}

			// Assert
			Assert::AreEqual(str_genre_name, updated_genre.get_genre());
		}

		TEST_METHOD(get_ratings) {
			// Act
			auto ratings = obj_game_manager.get_ratings();

			// Assert
			Assert::AreEqual(6, (int)ratings.size());
		}

		TEST_METHOD(get_genres) {
			// Act
			auto genres = obj_game_manager.get_genres();

			// Assert
			Assert::AreEqual(10, (int)genres.size());
		}

		TEST_METHOD(make_purchase) {
			// Arrange
			obj_game_manager.refresh_games();
			Game game = obj_game_manager.get_vec_games()[2];
			User user;
			user.set_id(2);
			obj_game_manager.set_basket_user(user.get_id());

			// Act
			obj_game_manager.add_basket_item(PurchaseItem(game.get_id(), game, 5, game.get_price()));
			obj_game_manager.make_purchase();
			obj_purchase_manager.fetch_purchases(user);
			auto user_purchases = obj_purchase_manager.get_vec_purchases();

			for (Purchase& purchase : user_purchases) {
				obj_purchase_manager.populate_purchase_details(purchase);
			}

			obj_game_manager.refresh_games();

			// Assert
			Assert::AreEqual(game.get_copies() - 5, obj_game_manager.get_vec_games()[2].get_copies());
			Assert::AreEqual(1, (int)user_purchases.size());
			Assert::AreEqual(1, (int)user_purchases[0].get_vec_purchase_items().size());
			Assert::AreEqual(game.get_price() * (double)5, user_purchases[0].get_total());
		}

		TEST_METHOD_CLEANUP(cleanup_test) {
			sqlite3_close_v2(obj_db_manager.get_database());

			if (std::filesystem::exists("database\\testDatabase.db")) {
				std::filesystem::remove("database\\testDatabase.db");
			}
		}
	};
}