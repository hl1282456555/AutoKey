#pragma once

#include <string>
#include <map>
#include <algorithm>

#include <WinUser.h>

struct FCommandInfo
{
	int KeyCode;
	int WaitTime;
};

static std::map<std::string, int> ControlKeyMap = {
	{"F1", VK_F1}, {"F2", VK_F2}, {"F3", VK_F3}, {"F4", VK_F4}, {"F5", VK_F5}, {"F6", VK_F6}, {"F7", VK_F7}, {"F8", VK_F8}, {"F9", VK_F9}, {"F10", VK_F10}, {"F11", VK_F11}, {"F12", VK_F12},
	{"0", 0x30}, {"1", 0x31}, {"2", 0x32}, {"3", 0x33}, {"4", 0x34}, {"5", 0x35}, {"6", 0x36}, {"7", 0x37}, {"8", 0x38}, {"9", 0x39},
	{"A", 0x41}, {"B", 0x42}, {"C", 0x43}, {"D", 0x44}, {"E", 0x45}, {"F", 0x46}, {"G", 0x47}, {"H", 0x48}, {"I", 0x49}, {"J", 0x4A}, {"K", 0x4B}, {"L", 0x4C},
	{"M", 0x4D}, {"N", 0x4E}, {"O", 0x4F}, {"P", 0x50}, {"Q", 0x51}, {"R", 0x52}, {"S", 0x53}, {"T", 0x54}, {"U", 0x55}, {"V", 0x56}, {"W", 0x57}, {"X", 0x58},
	{"Y", 0x59}, {"Z", 0x5A},
	{"N0", VK_NUMPAD0}, {"N1", VK_NUMPAD1}, {"N2", VK_NUMPAD2}, {"N3", VK_NUMPAD3}, {"N4", VK_NUMPAD4}, {"N5", VK_NUMPAD5}, {"N6", VK_NUMPAD6}, {"N7", VK_NUMPAD7},
	{"N8", VK_NUMPAD8}, {"N9", VK_NUMPAD9},
	{"SPACE", VK_SPACE}, {"BACK", VK_BACK}, {"\\", VK_OEM_5}, {"~", VK_OEM_3}, {"ENTER", VK_RETURN}, {"INS", VK_INSERT}, {"HOME", VK_HOME}, {"PAGEUP", VK_PRIOR},
	{"PAGEDOWN", VK_NEXT}, {"DELETE", VK_DELETE}, {"END", VK_END}, {"+", VK_OEM_PLUS}, {"-", VK_OEM_MINUS}, {",", VK_OEM_COMMA}, {".", VK_OEM_PERIOD}
};

bool MapKeyToCode(const std::string& InKey, int& OutCode)
{
	std::string tempKey(InKey);
	std::transform(InKey.begin(), InKey.end(), tempKey.begin(), std::toupper);

	auto FoundIt = std::find_if(ControlKeyMap.begin(), ControlKeyMap.end(), [&](const std::pair<std::string, int>& Value)
		{
			if (InKey.size() != Value.first.size()) {
				return false;
			}

			return Value.first.compare(tempKey) == 0;
		});

	if (FoundIt == ControlKeyMap.end()) {
		return false;
	}

	OutCode = FoundIt->second;

	return true;
}