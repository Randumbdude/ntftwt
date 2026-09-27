#include "benchmarks.h"
#include <iostream>
#include <julia.h>

int32_t run_julia_benchmark() {
	std::cout << "Running Julia benchmark..." << std::endl;

    // Load and execute the Julia script
    jl_eval_string("include(\"demo.jl\")");

    // Call a Julia function
    jl_value_t* add_func = jl_get_function(jl_main_module, "add");

    if (add_func)
    {
        jl_value_t* result = jl_call2(add_func, jl_box_int64(10), jl_box_int64(20));

        if (result)
        {
            int64_t value = jl_unbox_int64(result);
            std::cout << "Julia returned: " << value << '\n';
        }
    }

	std::cout << "Julia benchmark completed." << std::endl;

	return 0; // Return 0 to indicate success
}