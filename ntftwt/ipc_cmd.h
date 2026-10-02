#pragma once
#include <cstdint>
#include "ipc.h"

// ipc_cmd
int check_parentized();
void run_server();
void send_message(command_e cmd, const char* text);