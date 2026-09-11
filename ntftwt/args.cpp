#ifndef NOMINMAX
#define NOMINMAX
#endif
#include "args.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <limits>
#include "exit_codes.h"
#include "globals.h"
#include "benchmarks.h"

bool is_parentized = false;
HANDLE process_mutex = nullptr;

// flags
static bool next_input = 0;

void cleanup() {
	// Perform any necessary cleanup tasks here
	std::cout << "Cleaning up before exit..." << std::endl;
	CloseHandle(process_mutex);
}

static int32_t input_file_command() {
	std::cout << "Preparing to read input file..." << std::endl;
	next_input = true;
	return EXIT_CODES.SUCCESS;
}
static int32_t input_file_helper(char* argv) {
	std::cout << "Input file: " << argv << std::endl;

	std::ifstream file(argv);

	if (!file.is_open()) {
		next_input = false;
		return EXIT_CODES.IN_FI_ERROR;
	}

	std::string line;

	// Read line 1 -> avx_t_a
	if (std::getline(file, line)) {
		// Remove [ and ]
		for (char& c : line) {
			if (c == '[' || c == ']')
				c = ' ';
		}

		std::stringstream ss(line);
		std::string value;
		int i = 0;

		while (std::getline(ss, value, ',') && i < 8) {
			try {
				avx_t_a[i++] = std::stof(value);
			}
			catch (const std::exception&) {
				file.close();
				next_input = false;
				return EXIT_CODES.IN_FI_ERROR;
			}
		}

		if (i != 8) {
			file.close();
			return EXIT_CODES.IN_FI_ERROR;
		}
	}
	else {
		file.close();
		return EXIT_CODES.IN_FI_ERROR;
	}

	// Read line 2 -> avx_t_b
	if (std::getline(file, line)) {
		// Remove [ and ]
		for (char& c : line) {
			if (c == '[' || c == ']')
				c = ' ';
		}

		std::stringstream ss(line);
		std::string value;
		int i = 0;

		while (std::getline(ss, value, ',') && i < 8) {
			try {
				avx_t_b[i++] = std::stof(value);
			}
			catch (const std::exception&) {
				file.close();
				next_input = false;
				return EXIT_CODES.IN_FI_ERROR;
			}
		}

		if (i != 8) {
			file.close();
			return EXIT_CODES.IN_FI_ERROR;
		}
	}
	else {
		file.close();
		return EXIT_CODES.IN_FI_ERROR;
	}

	file.close();

	std::cout << "Array A: ";
	for (int i = 0; i < 8; ++i)
		std::cout << avx_t_a[i] << " ";

	std::cout << "\nArray B: ";
	for (int i = 0; i < 8; ++i)
		std::cout << avx_t_b[i] << " ";

	std::cout << std::endl;

	next_input = false;
	return EXIT_CODES.SUCCESS;
}

static int32_t help_command() {
	std::cout << "Usage: ntftwt [options]\n";
	std::cout << "Options:\n";
	std::cout << "  --help,      Show this help message\n";
	std::cout << "  --version,   Show version information\n";
	std::cout << "  -i <input file>,   Specify an input file\n";
	std::cout << "  -p,           Parentize process\n";
	exit(EXIT_CODES.SUCCESS);
	return EXIT_CODES.SUCCESS;
}

static int32_t version_command() {
	std::cout << "ntftwt version: " << VERSION << std::endl;
	exit(EXIT_CODES.SUCCESS);
	return EXIT_CODES.SUCCESS;
}

static int32_t parentize_command() {

	// setup mutex for this instance (or try to)
	process_mutex = CreateMutexA(
		nullptr,
		TRUE,
		"ntftwt_parentized"
	);

	if (process_mutex == nullptr)
	{
		std::cerr << "Failed to create mutex\n";
		return EXIT_CODES.MUTEX_FAILED;
	}

	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		std::cout << "Another parent instance is already running!\n";
		CloseHandle(process_mutex);
		return EXIT_CODES.PARENT_EXISTS;
	}

	std::cout << "This is the first instance.\n";

	is_parentized = true;
	atexit(cleanup);
	std::cout << "Successfully parentized the process." << std::endl;
	return EXIT_CODES.SUCCESS;
}

static int32_t castlemania_command() {
	std::cout << "castlemania has been executed." << std::endl;
	return EXIT_CODES.SUCCESS;
}

static int32_t invalid_command() {
	std::cout << "Invalid command. Use --help for usage information." << std::endl;
	return EXIT_CODES.INVALID_ARGS;
}

struct command_t
{
	const char* name;
	int32_t(*method)();
};

command_t commands[] =
{
	{ "-i",    input_file_command },
	{ "--help", help_command },
	{ "--version", version_command },
	{"-p", parentize_command },
	{"--castlemania", castlemania_command }
};

size_t command_count = sizeof(commands) / sizeof(commands[0]);

int32_t handle_args(int argc, char* argv[]) {
	//std::cout << "Total arguments received: " << argc << std::endl;

	if (argc == 1) {
		std::cout << "Usage: ntftwt [options]" << std::endl;
		return EXIT_CODES.NO_ARGS;
	}

	for (int i = 1; i < argc; ++i) {
		//std::cout << "argv[" << i << "]: " << argv[i] << std::endl;

		// catch flags: if a previous option indicated the next argv is data (e.g. -i filename)
		if (next_input) {
			int32_t res = input_file_helper(argv[i]);
			if (res != EXIT_CODES.SUCCESS) {
				return res;
			}
			// successfully handled the input filename; skip further processing of this argv
			continue;
		}

		// rest of logic
		bool matched = false;
		for (size_t j = 0; j < command_count; ++j) {
			if (strcmp(argv[i], commands[j].name) == 0) {
				matched = true;
				//std::cout << std::endl;
				int32_t res = commands[j].method();
				// If this command is an option that expects a following argument (like -i),
				// don't return immediately; allow the outer loop to process the next argv
				// which will be handled by the next_input flag. For other commands, return.
				if (strcmp(commands[j].name, "-i") == 0) {
					break;
				}
				return res;
			}
		}

		if (!matched) {
			invalid_command();
			return EXIT_CODES.INVALID_ARGS;
		}
	}
	return EXIT_CODES.SUCCESS;
}

int32_t arg_yes_no(const char* prompt) {
	std::cout << prompt << " (y/n): ";
	char resp = '\0';
	std::cin >> resp;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	return (resp == 'y' || resp == 'Y') ? 1 : 0;
}