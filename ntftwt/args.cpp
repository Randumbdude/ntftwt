#include "args.h"
#include <iostream>

int32_t handle_args(int argc, char* argv[]) {
    std::cout << "Total arguments received: " << argc << "\n";

    for (int i = 0; i < argc; ++i) {
        std::cout << "argv[" << i << "]: " << argv[i] << "\n";
    }

    return 0;
}