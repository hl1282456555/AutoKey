// AutoKey.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>
#include <fstream>
#include <vector>

#include "Utils.h"
#include "json.hpp"

int main()
{
    std::cout << "Welcome to AutoKey!" << std::endl;
    HWND FFXIVWindow = FindWindowEx(NULL, NULL, NULL, L"×îÖÕ»ÃÏëXIV");
    if (FFXIVWindow == NULL) {
        std::cout << "Failed to find FFXIV window." << std::endl;
        return -1;
    }

    std::cout << "Found FFXIV window." << std::endl;
    std::cout << "Loading the config file." << std::endl;

    using Json = nlohmann::json;

    std::ifstream ConfigFile("Config.json");
    if (!ConfigFile.is_open()) {
        std::cout << "Cannot open the config file." << std::endl;
        return -1;
    }

    int ControlKeyCode;
    bool bShouldLoop = false;
    std::vector<FCommandInfo> Commands;
    try
    {
		Json ConfigJson;
		ConfigFile >> ConfigJson;

        if (ConfigJson.find("loop") != ConfigJson.end()) {
            bShouldLoop = ConfigJson["loop"];
        }

        std::cout << "Loop flag is set to " << (bShouldLoop ? "true." : "false.") << std::endl;

		std::string ControlKeyStr = ConfigJson["controlKey"];
        if (!MapKeyToCode(ControlKeyStr, ControlKeyCode)) {
            std::cout << "'controlKey' is not in right range, please fix it." << std::endl;
            return -1;
        }

        std::cout << "Set the '" << ControlKeyStr << "' key to control the processor." << std::endl;

        std::cout << "Parsing the command list." << std::endl;

        std::vector<Json> CommandListArray = ConfigJson["commandList"];
        if (CommandListArray.empty()) {
            std::cout << "Cannot found any command, please enter your command list, e.g: {\"key\": \"1\", \"waitTime\": 1000}." << std::endl;
            return -1;
        }

        for (auto CommandJson : CommandListArray)
        {
            FCommandInfo NewCommand;
            if (!MapKeyToCode(CommandJson["key"], NewCommand.KeyCode)) {
                std::cout << CommandJson["key"] << " is not available key." << std::endl;
                return -1;
            }

            NewCommand.WaitTime = CommandJson["waitTime"];

            Commands.push_back(std::move(NewCommand));
        }
    }
    catch (const std::exception& Error)
    {
        std::cout << "Fatal error occurred: %s" << Error.what() << std::endl;
        return false;
    }

    bool bStartProcess = false;
    int NextCommandIndex = 0;

    std::cout << "Start to process the command list, you can enter the control key to stop it." << std::endl;
    while (1)
    {
       bool bControlKeyPressed = GetAsyncKeyState(ControlKeyCode) & 0x01;

       if (bControlKeyPressed) {
           bStartProcess = !bStartProcess;
           std::cout << "Control key pressed, will " << (bStartProcess ? "wakeup" : "stop") << " the processor." << std::endl;
       }

        if (!bStartProcess) {
			Sleep(1000);
			NextCommandIndex = 0;
            continue;
        }

        const FCommandInfo Command = Commands[NextCommandIndex];

        PostMessage(FFXIVWindow, WM_KEYDOWN, Command.KeyCode, 0);
        Sleep(100);
        PostMessage(FFXIVWindow, WM_KEYUP, Command.KeyCode, 0);

        Sleep(Command.WaitTime);

        NextCommandIndex++;
        if (NextCommandIndex >= Commands.size()) {
            std::cout << "All commands in the list is finished." << std::endl;
            NextCommandIndex = 0;

            if (!bShouldLoop) {
                bStartProcess = false;
                continue;
            }
        }
    }
}
