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
	/// <param name=""></param>
	/// <returns></returns>
	std::string validate_string(int);

	/// <summary>
	/// Validates that user input is an integer value.
	/// </summary>
	/// <returns></returns>
	int validate_int();

	/// <summary>
	/// Validates that a user input is an int and between or equal to specified min and maximum values
	/// </summary>
	/// <param name=""></param>
	/// <param name=""></param>
	/// <returns></returns>
	int validate_int(int, int);
};

