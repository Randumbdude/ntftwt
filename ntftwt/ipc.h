#pragma once
#include <cstdint>
#include <Windows.h>

constexpr const char* PIPE_NAME = "\\\\.\\pipe\\ntftwt_IPC";

struct ipc_command {
	uint32_t cmd;
	char cmd_args[64];
	DWORD pid;
	// ai data structs
	
};

enum class command_e : uint32_t {
	ping = 1,
	exe_cmd = 2,
	terminate = 3,
	// types of ai structural data
	klondike_game_data = 4
};