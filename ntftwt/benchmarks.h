#pragma once
#include <cstdint>

// Benchmarking functions for AVX2
inline float avx_t_a[8], avx_t_b[8];
int32_t avx2_benchmark(size_t iterations);

// Benchmarking functions for Raylib
int32_t raylib_test();