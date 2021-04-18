#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include "Utilities.h"

namespace validate {
	/// <summary>
	/// Gets the key input from user (records next pressed key as KEY_EVENT_RECORD)
	/// </summary>
	/// <param name="keyRec"></param>
	/// <param name="hConsole"></param>
	/// <returns></returns>
	bool get_control_char(KEY_EVENT_RECORD& keyRec, HANDLE& hConsole);

	/// <summary>
	/// Returns the valid key event options for normal menu situations.
	/// </summary>
	/// <returns></returns>
	std::vector<WORD> get_valid_key_events();

	/// <summary>
	/// Gets user input as a string - doesn't really do any "validation" as any entry can be a valid string.
	/// </summary>
	/// <returns></returns>
	std::string validate_string();

	/// <summary>
	/// Gets user input as a string, but checks that it is above a specified minimum length
	/// </summary>
	/// <param name="min_length"></param>
	/// <returns></returns>
	std::string validate_string(int min_length);

	/// <summary>
	/// Gets user input as a string, but checks it is below or equal to the specified maximum length. 
	/// Bool flag is provided simply to allow different method signatures between min and max versions of this function
	/// </summary>
	/// <param name="max_legnth"></param>
	/// <param name="max_flag"></param>
	/// <returns></returns>
	std::string validate_string(int max_legnth, bool max_flag);

	/// <summary>
	/// Validates that user input is an integer value.
	/// </summary>
	/// <returns></returns>
	int validate_int();

	/// <summary>
	/// Validates that input is an int and is more than
	/// </summary>
	/// <param name="min_size"></param>
	/// <returns></returns>
	int validate_int(int min_size);

	/// <summary>
	/// Validates that a user input is an int and between or equal to specified min and maximum values
	/// </summary>
	/// <param name="min_size"></param>
	/// <param name="max_size"></param>
	/// <returns></returns>
	int validate_int(int min_size, int max_size);

	/// <summary>
	/// Validates that user input is a valid double.
	/// </summary>
	/// <returns></returns>
	double validate_double();

	/// <summary>
	/// Validates that user input is a valid double and more than the minimum value.
	/// </summary>
	/// <param name="min_size"></param>
	/// <returns></returns>
	double validate_double(double min_size);
};

