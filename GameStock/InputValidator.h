#pragma once
#include <Windows.h>
#include <vector>

namespace validate {
	bool get_control_char(KEY_EVENT_RECORD& keyRec, HANDLE& hConsole);
	std::vector<WORD> get_valid_key_events();
};

