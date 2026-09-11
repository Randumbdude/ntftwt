#pragma once
#include <cstdint>
struct exit_codes_t {
	static constexpr int32_t SUCCESS = 0x00000000;
	static constexpr int32_t FAILURE = 0x00000001;
	static constexpr int32_t NO_ARGS = 0x00000002;
	static constexpr int32_t INVALID_ARGS = 0x00000003;
	static constexpr int32_t IN_FI_ERROR = 0x00000004;
	static constexpr int32_t AVX2_UNSUPPORTED = 0x00000005;
	static constexpr int32_t MUTEX_FAILED = 0x00000006;
	static constexpr int32_t PARENT_EXISTS = 0x00000007;
	static constexpr int32_t FUCK = 0x7FFFFFFF;
};
static constexpr exit_codes_t EXIT_CODES{};