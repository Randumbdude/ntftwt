#include "ipc_client.h"

#include <iostream>
#include <cstring>

bool IPCClient::send(
    Command_e command,
    int32_t value,
    const char* text
)
{
    ipc_command message{};

    message._cmd_ = static_cast<uint32_t>(command);
    message.value = value;

    if (text != nullptr)
    {
        strcpy_s(
            message.text,
            sizeof(message.text),
            text
        );
    }

    std::cout << "Connecting to IPC pipe..." << std::endl;

    pipe = CreateFileA(
        PIPE_NAME,
        GENERIC_WRITE,
        0,
        nullptr,
        OPEN_EXISTING,
        0,
        nullptr
    );

    if (pipe == INVALID_HANDLE_VALUE)
    {
        std::cerr
            << "Could not connect to IPC pipe.\n"
            << "IPC Error: "
            << GetLastError()
            << std::endl;

        return false;
    }

    DWORD bytesWritten = 0;

    BOOL success = WriteFile(
        pipe,
        &message,
        sizeof(message),
        &bytesWritten,
        nullptr
    );

    if (!success)
    {
        std::cerr
            << "WriteFile failed: "
            << GetLastError()
            << std::endl;

        CloseHandle(pipe);
        pipe = INVALID_HANDLE_VALUE;

        return false;
    }

    std::cout
        << "Struct sent successfully.\n"
        << "Bytes sent: "
        << bytesWritten
        << " / "
        << sizeof(message)
        << std::endl;

    CloseHandle(pipe);
    pipe = INVALID_HANDLE_VALUE;

    return bytesWritten == sizeof(message);
}