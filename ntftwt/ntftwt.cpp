
// ntftwt.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include "args.h"
#include "constants.h"
#include "exit_codes.h"
#include "avx_t.h"

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

	// return
	return EXIT_CODES.SUCCESS;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu