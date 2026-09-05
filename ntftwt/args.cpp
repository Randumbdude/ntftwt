#include "args.h"
#include <iostream>
#include <cstring>
#include "exit_codes.h"
#include "constants.h"

static int32_t input_file_command() {
	std::cout << "Input file method called." << std::endl;
	return EXIT_CODES.SUCCESS;
}

static int32_t help_command() {
	std::cout << "Usage: ntftwt [options]\n";
	std::cout << "Options:\n";
	std::cout << "  --help,      Show this help message\n";
	std::cout << "  --version,   Show version information\n";
	return EXIT_CODES.SUCCESS;
}

static int32_t version_command() {
	std::cout << "ntftwt version: " << VERSION << std::endl;
	return EXIT_CODES.SUCCESS;
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
};

size_t command_count = sizeof(commands) / sizeof(commands[0]);

int32_t handle_args(int argc, char* argv[]) {
	std::cout << "Total arguments received: " << argc << std::endl;

	if (argc == 1)
		return EXIT_CODES.NO_ARGS;

	for (int i = 1; i < argc; ++i) {
		std::cout << "argv[" << i << "]: " << argv[i] << std::endl;

		bool matched = false;
		for (size_t j = 0; j < command_count; ++j) {
			if (strcmp(argv[i], commands[j].name) == 0) {
				matched = true;
				std::cout << std::endl;
				return commands[j].method();
			}
		}

		if (!matched) {
			return EXIT_CODES.INVALID_ARGS;
		}
	}
	return EXIT_CODES.FAILURE;
}
