#pragma once
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <numeric>
#include <sstream>
#include <iomanip>
#include "sqlite3.h"
#include "Game.h"
#include "Rating.h"
#include "Genre.h"
#include "Purchase.h"
#include "PurchaseItem.h"

/// <summary>
/// Class that is used to perform operations against games, and against game sub objects, such as genre and rating
/// </summary>
class GameManager
{
	sqlite3* _db;
	std::vector<Game> _vec_games;
	Purchase _obj_basket;
	Genre _obj_filter_genre;
	bool _bool_initialised = false;
	bool _bool_admin_flag = false;
	int get_games();
public:
	GameManager(sqlite3* db) { _db = db; }

	/// <summary>
	/// Initialises the games into the internal class vector, only initialises when bool_initialised is false
	/// </summary>
	void initialise_games();

	/// <summary>
	/// Acts like a forced refresh of games, will re-fetch games from database regardless of the initialised status
	/// </summary>
	void refresh_games();

	/// <summary>
	/// Returns the games found via the initialise/refresh methods
	/// </summary>
	/// <returns></returns>
	std::vector<Game>& get_vec_games() { return _vec_games; }

	/// <summary>
	/// Gets the basket instance (which is really just a purchase being re-purposed for this)
	/// </summary>
	/// <returns></returns>
	Purchase& get_basket() { return _obj_basket; }

	/// <summary>
	/// Sets the user id in the basket, which is needed when persisting any purchases
	/// </summary>
	/// <param name="i_user_id"></param>
	void set_basket_user(int i_user_id) { _obj_basket.set_user_id(i_user_id); }

	/// <summary>
	/// Adds a purchase item to the basket, combines purchase items if they are the same game, errors if count would be higher than available number of games.
	/// </summary>
	/// <param name="obj_purhcase_item"></param>
	void add_basket_item(PurchaseItem& obj_purhcase_item);

	/// <summary>
	/// Removes a game from the basket (vec_purchase_items)
	/// </summary>
	/// <param name="i_game_id"></param>
	void remove_basket_item(int i_game_id);

	/// <summary>
	/// Calculates the total of all the purcahse items within the basket
	/// </summary>
	/// <returns></returns>
	double get_basket_total();

	/// <summary>
	/// Clears the basket to allow it to be re-used within the same user session
	/// </summary>
	void reset_basket();

	bool get_admin_flag() { return _bool_admin_flag; }

	/// <summary>
	/// Sets the admin flag, which is used while fetching games to decide whether to fetch games with
	/// </summary>
	/// <param name="bool_admin_flag"></param>
	void set_admin_flag(bool bool_admin_flag) { _bool_admin_flag = bool_admin_flag; }

	/// <summary>
	/// Sets initialised status, which is used while deciding if games should be re-fetched from the database
	/// </summary>
	/// <param name="bool_initialised"></param>
	void set_initialised(bool bool_initialised) { _bool_initialised = bool_initialised; }

	/// <summary>
	/// Adds a game to the database, requires games to be refreshed upon success (to show newly added game).
	/// </summary>
	/// <param name="obj_game"></param>
	void add_game(Game& obj_game);

	/// <summary>
	/// Deletes a game from the database, requires games to be refreshed upon success (to update to not display removed game)
	/// </summary>
	/// <param name="obj_game"></param>
	void delete_game(Game& obj_game);

	/// <summary>
	/// Updates the specified game's name in the database
	/// </summary>
	/// <param name="i_game_id"></param>
	/// <param name="str_game_name"></param>
	void update_game_name(int i_game_id, std::string str_game_name);

	/// <summary>
	/// Updates the specified game's genre in the database
	/// </summary>
	/// <param name="i_game_id"></param>
	/// <param name="i_genre_id"></param>
	void update_game_genre(int i_game_id, int i_genre_id);

	/// <summary>
	/// Updates the specified game's price in the database
	/// </summary>
	/// <param name="i_game_id"></param>
	/// <param name="d_price"></param>
	void update_game_price(int i_game_id, double d_price);

	/// <summary>
	/// Updates the specified game's rating in the database.
	/// </summary>
	/// <param name="i_game_id"></param>
	/// <param name="i_rating_id"></param>
	void update_game_rating(int i_game_id, int i_rating_id);

	/// <summary>
	/// Updates the specified game's copies in the database.
	/// </summary>
	/// <param name="i_game_id"></param>
	/// <param name="i_copies"></param>
	void update_game_copies(int i_game_id, int i_copies);

	/// <summary>
	/// Adds a genre to the database
	/// </summary>
	/// <param name="obj_genre"></param>
	void add_genre(Genre& obj_genre);

	/// <summary>
	/// Deletes a genre from the databas
	/// </summary>
	/// <param name="obj_genre"></param>
	void delete_genre(Genre& obj_genre);

	/// <summary>
	/// Updates a genre's name in the database
	/// </summary>
	/// <param name="i_genre_id"></param>
	/// <param name="str_genre_name"></param>
	void update_genre_name(int i_genre_id, std::string str_genre_name);

	/// <summary>
	/// Returns all of the ratings which are currently stored in the database.
	/// </summary>
	/// <returns></returns>
	const std::vector<Rating> get_ratings();

	/// <summary>
	/// Returns all of the genres that are currently stored in the database
	/// </summary>
	/// <returns></returns>
	const std::vector<Genre> get_genres();

	/// <summary>
	/// Used to persist items in a basket to the database, and update the number of copies available of games that have been purchased.
	/// </summary>
	/// <returns></returns>
	double make_purchase();

	Genre& get_filter_genre() { return _obj_filter_genre; }

	/// <summary>
	/// Sets the genre to filter by when fetching games from the database
	/// </summary>
	/// <param name="obj_filter_genre"></param>
	void set_filter_genre(Genre obj_filter_genre) { _obj_filter_genre = obj_filter_genre; }

	/// <summary>
	/// Performs the necessary actions to reset the GameManager's state upon a user logout.
	/// </summary>
	void logout();
};

