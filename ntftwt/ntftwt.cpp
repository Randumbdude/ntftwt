
// ntftwt.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#include <string>
#include "args.h"
#include "globals.h"
#include "exit_codes.h"
#include "benchmarks.h"
#include "ipc_cmd.h"

int main(int argc, char* argv[]) {

	std::cout << "ntftwt" << std::endl;

	// arguments for main entry point
	if (int32_t i = handle_args(argc, argv))
		return i;

	// check for zvx2 support
#ifdef __AVX2__
	std::cout << "AVX2 is supported!" << std::endl;
#else
	std::cout << "AVX2 is not supported!" << std::endl;
	return EXIT_CODES.AVX2_UNSUPPORTED;
#endif

	// prompt user to run the AVX2 benchmark
	if (arg_yes_no("Would you like to run the AVX2 benchmark?"))
		if (int32_t i = avx2_benchmark(100000000))
			return i;

	// prompt user to run the Raylib graphics test
	if (arg_yes_no("Would you like to run the Raylib graphics test?"))
		if (int32_t i = raylib_test())
			return i;

	// now we check if the process is parentized
	if (is_parentized) {
		std::cout << "Process is parentized." << std::endl;
		run_server();
	}
	else {
		std::cout << "Process is not parentized." << std::endl;
		std::cout << "Checking existance of parentized instance..." << std::endl;
		if (check_parentized()) {
			std::cout << "No parentized instance found." << std::endl;
		}
		else {
			std::cout << "Enter command code: "; 
			char resp = '\0'; 
			std::cin >> resp; 
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
			char message[64]{}; 
			std::cout << "Enter message to send: "; 
			std::cin.getline(message, sizeof(message)); 
			send_message((resp - '0'), message);
		}
	}
	// return
	return EXIT_CODES.SUCCESS;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu