#include "ipc_client.h"
#include <iostream>
#include <cstring>

bool IPCClient::send(const ipc_command ipc_cmd) {
	std::cout << "Connecting to IPC pipe..." << std::endl;
	pipe = CreateFileA(PIPE_NAME, GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);
	if (pipe == INVALID_HANDLE_VALUE)
	{
		std::cerr << "Could not connect to IPC pipe.\n" << "IPC Error: " << GetLastError() << std::endl;
		return false;
	}
	DWORD bytesWritten = 0;
	BOOL success = WriteFile(pipe, &ipc_cmd, sizeof(ipc_command), &bytesWritten, nullptr);
	if (!success)
	{
		std::cerr << "WriteFile failed: " << GetLastError() << std::endl;
		CloseHandle(pipe);
		pipe = INVALID_HANDLE_VALUE;
		return false;
	}
	std::cout << "Struct sent successfully.\n" << "Bytes sent: " << bytesWritten << " / " << sizeof(ipc_command) << std::endl;
	CloseHandle(pipe);
	pipe = INVALID_HANDLE_VALUE;
	return bytesWritten == sizeof(ipc_command);
}