#pragma once

#include <Windows.h>
#include <cstdint>
#include "ipc.h"

class IPCClient
{
public:
    bool send(const ipc_command ipc_cmd);

private:
    HANDLE pipe = INVALID_HANDLE_VALUE;
};