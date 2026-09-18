#pragma once
#include <cstdint>
#include <Windows.h>

constexpr const char* PIPE_NAME = "\\\\.\\pipe\\ntftwt_IPC";

enum class command_e : uint32_t {
	ping = 1,
	terminate = 3,
	exe_cmd = 2
};

struct ipc_command {
	command_e _cmd_;
	char text[64];
	DWORD pid;
};
