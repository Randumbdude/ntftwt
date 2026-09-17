#pragma once

#include <Windows.h>
#include <cstdint>

constexpr const char* PIPE_NAME = "\\\\.\\pipe\\ntftwt_IPC";

enum class Command_e : uint32_t
{
    ping = 1,
    set_value = 2,
    die_hard = 3,
    exe_cmd = 4
};

struct ipc_command
{
    uint32_t _cmd_;
    int32_t value;
    char text[64];
};

class IPCClient
{
public:
    bool send(Command_e command, int32_t value = 0, const char* text = "");

private:
    HANDLE pipe = INVALID_HANDLE_VALUE;
};