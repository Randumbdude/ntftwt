#pragma once
#include <cstdint>
#include "ipc.h"

void run_server();
int32_t check_parentized();
void send_message(command_e cmd, const char* text);