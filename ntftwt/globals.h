#pragma once
#include <string>
#include <cstdint>

// Keep VERSION as a constexpr string literal to avoid static initialization
// order and multiple-definition issues across translation units.
constexpr const char* VERSION = "0.0.0.1a";

// Avoid including Windows.h in this header to prevent symbol collisions
// (e.g., DrawText, ShowCursor) when other libraries like raylib are used.
// Use a lightweight typedef for HANDLE so this header stays platform-agnostic.
using HANDLE = void*;

// Declare globals here and define them in a single .cpp file to avoid
// multiple-definition linker errors when this header is included by
// multiple translation units.
extern bool is_parentized;
extern HANDLE process_mutex;

// Here we'll have our global methods from any file.
int input_yes_no(const char* prompt);

// Exit codes for the application. Using a struct with static constexpr members
struct exit_codes_t {
	static constexpr int SUCCESS = 0x00000000;
	static constexpr int FAILURE = 0x00000001;
	static constexpr int NO_ARGS = 0x00000002;
	static constexpr int INVALID_ARGS = 0x00000003;
	static constexpr int IN_FI_ERROR = 0x00000004;
	static constexpr int AVX2_UNSUPPORTED = 0x00000005;
	static constexpr int MUTEX_FAILED = 0x00000006;
	static constexpr int PARENT_EXISTS = 0x00000007;
	static constexpr int FUCK = 0x7FFFFFFF;
};
static constexpr exit_codes_t EXIT_CODES{};