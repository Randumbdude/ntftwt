#pragma once
#include <cstdint>
struct exit_codes_t {
	static constexpr int32_t SUCCESS = 0;
	static constexpr int32_t FAILURE = 1;
};

static constexpr exit_codes_t EXIT_CODES{};