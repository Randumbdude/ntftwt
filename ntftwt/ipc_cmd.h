#pragma once
#include <cstdint>
#include "ipc.h"

// ipc_cmd
void run_server();
int32_t check_parentized();
void send_message(command_e cmd, const char* text);

// ipc_args
int32_t ipoc_arg(const char* cmd);