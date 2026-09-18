#include "pch.h"
#include "ipc_cmd.h"
#include "exit_codes.h"
#include "args.h"
#include "sha1.h"
#include "ipc_args.h"

static int32_t execute_command(char* cmd_text) {
	printf("Executing command: %s\n", cmd_text);
	SHA1 cmd;
	cmd.update(cmd_text);
	printf("%s\n", cmd.final().c_str());
	const char* final_cmd = cmd.final().c_str();
	ipoc_arg(final_cmd);
	return EXIT_CODES.SUCCESS;
}

void run_server() {
	printf("Parentized server running.\n");
	while (true) {
		HANDLE pipe = CreateNamedPipeA(PIPE_NAME, PIPE_ACCESS_INBOUND, PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT, 1, sizeof(ipc_command), sizeof(ipc_command), 0, nullptr);
		if (pipe == INVALID_HANDLE_VALUE) {
			fprintf(stderr, "CreateNamedPipe failed: %lu\n", GetLastError());
			return;
		}
		printf("Waiting for another instance...\n");
		BOOL connected = ConnectNamedPipe(pipe, nullptr);
		if (!connected) {
			DWORD error = GetLastError();
			if (error != ERROR_PIPE_CONNECTED) {
				fprintf(stderr, "ConnectNamedPipe failed: %lu\n", error);
				CloseHandle(pipe); continue;
			}
		}
		ipc_command message{};
		DWORD bytesRead = 0;
		BOOL success = ReadFile(pipe, &message, sizeof(message), &bytesRead, nullptr);
		bool cmd_to_exe = false;
		if (success && bytesRead == sizeof(message)) {
			switch (message._cmd_) {
			case command_e::ping:
				printf("Received Ping.\n");
				break;
			case command_e::terminate:
				printf("Received terminate\n");
				exit(EXIT_CODES.FUCK);
				break;
			case command_e::exe_cmd:
				printf("Received EXE_CMD\n");
				cmd_to_exe = true;
				// Handle exe_cmd logic here
				break;
			default: printf("Unknown command\n");
				break;
			}

			if (cmd_to_exe)
				execute_command(message.text);
			else
				printf("Message text: %s\n", message.text);
		}
		DisconnectNamedPipe(pipe);
		CloseHandle(pipe);
	}
}

int32_t check_parentized() {
	HANDLE mutex = OpenMutexA(SYNCHRONIZE, FALSE, "Global\\ntftwt_parentized");
	if (mutex == nullptr) {
		return EXIT_CODES.SUCCESS;
	}
	else {
		CloseHandle(mutex);
		return EXIT_CODES.PARENT_EXISTS;
	}
}

void send_message(command_e cmd, const char* text) {
	// initialization of ipc_command struct that will be sent
	ipc_command command{};
	command._cmd_ = cmd;
	command.pid = GetCurrentProcessId();
	strcpy_s(command.text, sizeof(command.text), text);

	// now we connect...
	printf("Connecting to IPC pipe...\n");
	HANDLE pipe = CreateFileA(PIPE_NAME, GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);
	if (pipe == INVALID_HANDLE_VALUE) {
		fprintf(stderr, "Could not connect to IPC pipe.\n");
		fprintf(stderr, "IPC Error: %lu\n", GetLastError());
		return;
	}
	DWORD bytesWritten = 0;
	BOOL success = WriteFile(pipe, &command, sizeof(command), &bytesWritten, nullptr);
	if (success) {
		printf("Struct sent successfully.\n");
		printf("Bytes sent: %lu\n", bytesWritten);
	}
	else {
		fprintf(stderr, "WriteFile failed: %lu\n", GetLastError());
	}
	CloseHandle(pipe);
}