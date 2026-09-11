#include "ipc_cmd.h"
#include <windows.h> 
#include <iostream> 
#include "exit_codes.h"
#include "args.h"

constexpr const char* PIPE_NAME = "\\\\.\\pipe\\ntftwt_IPC";

struct ipc_command {
	uint32_t _cmd_;
	int32_t value;
	char text[64];
};

enum class Command_e : uint32_t {
	ping = 1,
	set_value = 2,
	die_hard = 3,
	exe_cmd = 4
};

int32_t execute_command(char* cmd_text) {
	std::cout << "Executing command: " << cmd_text << std::endl;
	return EXIT_CODES.SUCCESS;
}

void run_server() {
	int32_t myValue = 123;
	std::cout << "Parentized server running." << std::endl;
	std::cout << "Value = " << myValue << std::endl;
	while (true) {
		HANDLE pipe = CreateNamedPipeA(PIPE_NAME, PIPE_ACCESS_INBOUND, PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT, 1, sizeof(ipc_command), sizeof(ipc_command), 0, nullptr);
		if (pipe == INVALID_HANDLE_VALUE) {
			std::cerr << "CreateNamedPipe failed: " << GetLastError() << std::endl;
			return;
		}
		std::cout << "Waiting for another instance..." << std::endl;
		BOOL connected = ConnectNamedPipe(pipe, nullptr);
		if (!connected) {
			DWORD error = GetLastError();
			if (error != ERROR_PIPE_CONNECTED) {
				std::cerr << "ConnectNamedPipe failed: " << error << std::endl;
				CloseHandle(pipe); continue;
			}
		}
		ipc_command message{};
		DWORD bytesRead = 0;
		BOOL success = ReadFile(pipe, &message, sizeof(message), &bytesRead, nullptr);
		bool cmd_to_exe = false;
		if (success && bytesRead == sizeof(message)) {
			switch (static_cast<Command_e>(message._cmd_)) {
			case Command_e::ping:
				std::cout << "Received PING\n";
				break;
			case Command_e::set_value:
				myValue = message.value;
				std::cout << "Value changed to " << myValue << "\n";
				break;
			case Command_e::die_hard:
				std::cout << "Received DIE_HARD\n";
				exit(EXIT_CODES.FUCK);
				break;
			case Command_e::exe_cmd:
				std::cout << "Received EXE_CMD\n";
				cmd_to_exe = true;
				// Handle exe_cmd logic here
				break;
			default: std::cout << "Unknown command\n";
				break;
			}

			if (cmd_to_exe)
				execute_command(message.text);
			else
				std::cout << "Message text: " << message.text << "\n";
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

void send_message(uint32_t cmd, const char* text) {
	ipc_command command{};
	command._cmd_ = cmd;
	command.value = 226;
	strcpy_s(command.text, sizeof(command.text), text);

	std::cout << "Connecting to IPC pipe..." << std::endl;
	HANDLE pipe = CreateFileA(PIPE_NAME, GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);
	if (pipe == INVALID_HANDLE_VALUE) {
		std::cerr << "Could not connect to IPC pipe." << std::endl;
		std::cerr << "IPC Error: " << GetLastError() << std::endl;
		return;
	}
	DWORD bytesWritten = 0;
	BOOL success = WriteFile(pipe, &command, sizeof(command), &bytesWritten, nullptr);
	if (success) {
		std::cout << "Struct sent successfully." << std::endl;
		std::cout << "Bytes sent: " << bytesWritten << std::endl;
	}
	else {
		std::cerr << "WriteFile failed: " << GetLastError() << std::endl;
	}
	CloseHandle(pipe);
}

