#include "InputValidator.h"

bool validate::get_control_char(KEY_EVENT_RECORD& keyRec, HANDLE& hConsole) {
	DWORD cc;
	INPUT_RECORD irec;
	std::vector<WORD> vec_valid_key_events = get_valid_key_events();

	if (hConsole == NULL)
	{
		return false; // console not found
	}

	for (; ; )
	{
		ReadConsoleInput(hConsole, &irec, 1, &cc);
		if (irec.EventType == KEY_EVENT && ((KEY_EVENT_RECORD&)irec.Event).bKeyDown)
		{
			keyRec = (KEY_EVENT_RECORD&)irec.Event;

			for (auto& key_event : vec_valid_key_events)
			{
				if (keyRec.wVirtualKeyCode == key_event) return true;
			}

			return false;
		}
	}
	return false;
}

std::vector<WORD> validate::get_valid_key_events() {
	std::vector<WORD> vec_valid_key_events;
	vec_valid_key_events.push_back(VK_UP);
	vec_valid_key_events.push_back(VK_DOWN);
	vec_valid_key_events.push_back(VK_LEFT);
	vec_valid_key_events.push_back(VK_RIGHT);
	vec_valid_key_events.push_back(VK_RETURN);
	vec_valid_key_events.push_back(VK_ESCAPE);
	vec_valid_key_events.push_back(VK_F1);
	vec_valid_key_events.push_back(VK_F2);
	return vec_valid_key_events;
}

std::string validate::validate_string() {
	std::string strInput;
	std::getline(std::cin, strInput);
	return strInput;
}

std::string validate::validate_string(int min_length) {
	std::string strInput;

	while (true) {
		std::string strMessage = "Not a valid number";
		std::getline(std::cin, strInput);

		if (util::is_less_than_or_equal_to(strInput.length(), (size_t)min_length)) {
			strMessage = "To short, min length is " + std::to_string(min_length);
			std::cout << strMessage << ", try again: ";
		}
		else {
			return strInput;
		}
	}
}

std::string validate::validate_string(int max_length, bool max_flag) {
	std::string strInput;

	while (true) {
		std::string strMessage = "Not a valid number";
		std::getline(std::cin, strInput);

		if (util::is_more_than_or_equal_to(strInput.length(), (size_t)max_length)) {
			strMessage = "To long, max length is " + std::to_string(max_length);
			std::cout << strMessage << ", try again: ";
		}
		else {
			return strInput;
		}
	}
}

int validate::validate_int() {
	std::string strInput;
	int iInput;

	while (true) {
		std::getline(std::cin, strInput);
		try {
			iInput = std::stoi(strInput);
			return iInput;
		}
		catch (std::exception) {
			std::cout << "Not a valid whole number: ";
		}
	}
}

int validate::validate_int(int min_size) {
	std::string strInput;
	int iInput;

	while (true) {
		std::getline(std::cin, strInput);
		try {
			iInput = std::stoi(strInput);

			if (util::is_less_than(iInput, min_size)) {
				std::cout << "Input cannot be less than" << min_size << ": ";
			}
			else {
				return iInput;
			}
		}
		catch (std::exception) {
			std::cout << "Not a valid whole number: ";
		}
	}
}

int validate::validate_int(int min_size, int max_size) {
	std::string strInput;
	int iInput;

	while (true) {
		std::getline(std::cin, strInput);
		try {
			iInput = std::stoi(strInput);

			if (!util::is_between(min_size, max_size, iInput)) {
				std::cout << "Input not within valid range (" << min_size << " to " << max_size << ") : ";
			}
			else {
				return iInput;
			}
		}
		catch (std::exception) {
			std::cout << "Not a valid number: ";
		}
	}
}

double validate::validate_double() {
	std::string strInput;
	double dInput;

	while (true) {
		std::getline(std::cin, strInput);
		try {
			dInput = std::stod(strInput);

			return dInput;
		}
		catch (std::exception) {
			std::cout << "Not a valid number: ";
		}
	}
}

double validate::validate_double(double min_size) {
	std::string strInput;
	double dInput;

	while (true) {
		std::getline(std::cin, strInput);
		try {
			dInput = std::stod(strInput);

			if (util::is_less_than_or_equal_to(dInput, min_size)) {
				std::cout << "Input cannot be " << std::setprecision(2) << min_size << " or less: ";
			}
			else {
				return dInput;
			}
		}
		catch (std::exception) {
			std::cout << "Not a valid number: ";
		}
	}
}