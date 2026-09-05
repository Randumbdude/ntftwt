// debugger_main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// could not find config file, would you like to create one? (y/n): y

#include <iostream>
#include <string>
#include <iomanip>
#include <unordered_map>
#include "config.h"

static char VERSION[8] = "0.0.0.0";

static void cleanup() {
	std::cout << "Press any key to continue..." << std::endl;
	std::cin.get();
}

int main(int argc, char* argv[])
{
	// enter
	std::cout << "ntftwt debugger" << std::endl;

	// check for arguments
	std::string args;
	std::string pg_path;
	if (argc > 1) {
		// a config file has been specified, load it
		Config config(argv[1]);
		pg_path = config.get("program_path");
		args = config.get("program_args");

		int i = 0;

		if (pg_path.empty())
		{
			i = 1;
			std::cout << "No program path configured." << std::endl;

			config.set("program_path", ".\\ntftwt.exe");
			config.save();

			std::cout << "Configured program path." << std::endl;
		}
		else
		{
			std::cout << "Program path: " << pg_path << std::endl;
		}
		if (args.empty())
		{
			i = 1;
			std::cout << "No program args configured." << std::endl;

			config.set("program_args", "--help");
			config.save();

			std::cout << "Configured program args." << std::endl;
		}
		else
		{
			std::cout << "Program args: " << args << std::endl;
		}

		if (i) return 1;

		// pause argument
		if(argv[2] && std::string(argv[2]) == "--pause")
		{
			std::atexit(cleanup);
		}
	}
	else {
		pg_path = ".\\ntftwt.exe";
		std::cout << "Enter Arguments: ";
		std::getline(std::cin, args);
	}

	std::cout << "Starting ntftwt..." << std::endl;
	std::cout << "<======================================================================================================================>" << std::endl;

	std::string command = pg_path + " " + args;
	int error_level = std::system(command.c_str());
	std::cout << "<======================================================================================================================>" << std::endl;
	std::cout << "Exit Code: 0x"
		<< std::uppercase
		<< std::setfill('0')
		<< std::setw(8)
		<< std::hex
		<< error_level
		<< std::dec
		<< std::endl;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu