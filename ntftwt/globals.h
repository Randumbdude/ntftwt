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
int32_t input_yes_no(const char* prompt);