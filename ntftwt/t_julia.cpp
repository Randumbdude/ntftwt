#include "benchmarks.h"
#include <iostream>
#include <julia.h>

// 
// Julia benchmark method
//
int run_julia_benchmark() {
	std::cout << "Running Julia benchmark..." << std::endl;

	// Load and execute the Julia script
	jl_eval_string("include(\"benchmark.jl\")");

	// Call a Julia function
	jl_value_t* main_func = jl_get_function(jl_main_module, "ntftwt_main");

	if (main_func)
	{
		jl_value_t* result = jl_call0(main_func);

		if (result)
		{
			int64_t value = jl_unbox_int64(result);
			std::cout << "Julia returned: " << value << '\n';
		}
	}

	std::cout << "Julia benchmark completed." << std::endl;

	return 0; // Return 0 to indicate success
}