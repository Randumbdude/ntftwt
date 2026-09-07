#include "avx_t.h"
#include <iostream>
#include <immintrin.h>
#include <chrono>

int32_t avx2_benchmark(size_t iterations) {
	float result[8];

	// -------------------------
	// Scalar
	// -------------------------

	auto start_scalar = std::chrono::high_resolution_clock::now();
	for (std::size_t it = 0; it < iterations; ++it)
	{
		// repeat for 'iterations' long
		for (int i = 0; i < 8; ++i)
		{
			result[i] = avx_t_a[i] + avx_t_b[i];
		}
	}
	auto end_scalar = std::chrono::high_resolution_clock::now();

	// -------------------------
	// AVX2
	// -------------------------

	auto start_avx = std::chrono::high_resolution_clock::now();
	for (std::size_t it = 0; it < iterations; ++it)
	{
		// repeat for 'iterations' long
		__m256 va = _mm256_load_ps(avx_t_a);
		__m256 vb = _mm256_load_ps(avx_t_b);

		__m256 vc = _mm256_add_ps(va, vb);

		_mm256_store_ps(result, vc);
	}
	auto end_avx = std::chrono::high_resolution_clock::now();

	// -------------------------
	// calculate and print results
	// -------------------------

	double scalar =
		std::chrono::duration<double, std::milli>(
			end_scalar - start_scalar).count();

	double avx =
		std::chrono::duration<double, std::milli>(
			end_avx - start_avx).count();

	std::cout << "AVX2 Benchmark results for " << iterations << " iterations:\n";
	std::cout << "Scalar: " << scalar << " ms\n";
	std::cout << "AVX2:   " << avx << " ms\n";
	std::cout << "Speedup: " << scalar / avx << "x\n";

	return 0;
}