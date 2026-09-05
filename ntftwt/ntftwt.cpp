
// ntftwt.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "args.h"
#include "constants.h"
#include "exit_codes.h"

int main(int argc, char* argv[]) {

	std::cout << "ntftwt" << std::endl;

	// arguments for main entry point
	if (int i = handle_args(argc, argv))
		return i;

	//std::cout << "Exit Code: " << EXIT_CODES.SUCCESS << std::endl;

	return EXIT_CODES.SUCCESS;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu