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
	vec_valid_key_events.push_back(VK_RETURN);
	vec_valid_key_events.push_back(VK_ESCAPE);
	return vec_valid_key_events;
}