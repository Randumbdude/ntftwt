#include "ipc_client.h"
#include <cstdio>
#include <cstring>

bool IPCClient::send(const ipc_command ipc_cmd) {
	printf("Connecting to IPC pipe...\n");
	pipe = CreateFileA(PIPE_NAME, GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);
	if (pipe == INVALID_HANDLE_VALUE)
	{
		fprintf(stderr, "Could not connect to IPC pipe.\nIPC Error: %lu\n", GetLastError());
		return false;
	}
	DWORD bytesWritten = 0;
	BOOL success = WriteFile(pipe, &ipc_cmd, sizeof(ipc_command), &bytesWritten, nullptr);
	if (!success)
	{
		fprintf(stderr, "WriteFile failed: %lu\n", GetLastError());
		CloseHandle(pipe);
		pipe = INVALID_HANDLE_VALUE;
		return false;
	}
	printf("Struct sent successfully.\nBytes sent: %lu / %zu\n", bytesWritten, sizeof(ipc_command));
	CloseHandle(pipe);
	pipe = INVALID_HANDLE_VALUE;
	return bytesWritten == sizeof(ipc_command);
}
