#include "ipc_cmd.h"
#include <windows.h> 
#include <iostream> 
#include "globals.h"
#include "args.h"
#include "sha1.h"

#define ID_CANCEL 1001

LRESULT CALLBACK DialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		// Message
		CreateWindowW(
			L"STATIC",
			L"The promised future never happened.\nPress OK to continue.",
			WS_VISIBLE | WS_CHILD,
			20, 20, 300, 50,
			hwnd,
			NULL,
			NULL,
			NULL
		);

		// Cancel button
		CreateWindowW(
			L"BUTTON",
			L"Cancel",
			WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			120, 85, 100, 30,
			hwnd,
			(HMENU)ID_CANCEL,
			NULL,
			NULL
		);

		break;
	}

	case WM_COMMAND:
		if (LOWORD(wParam) == ID_CANCEL)
		{
			DestroyWindow(hwnd);
		}
		break;

	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProcW(hwnd, msg, wParam, lParam);
}


void ShowDialog()
{
	HINSTANCE hInstance = GetModuleHandleW(NULL);

	// Register window class
	WNDCLASSW wc = {};
	wc.lpfnWndProc = DialogProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = L"error";
	wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

	RegisterClassW(&wc);

	// Create window
	HWND hwnd = CreateWindowExW(
		WS_EX_DLGMODALFRAME,
		L"error",
		L"Command Failed",
		WS_CAPTION | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT,
		340, 160,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	// Message loop
	MSG msg;

	while (GetMessageW(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
}

int ipoc_arg(const char* cmd) {
	if (strcmp(cmd, "f931d1290e11f230a684c06ba04b9bc7938e7b02") == 0) {
		ShowDialog();
		return 0;
	}
	return 1;
}

int check_parentized() {
	HANDLE mutex = OpenMutexA(SYNCHRONIZE, FALSE, "Global\\ntftwt_parentized");
	if (mutex == nullptr) {
		return EXIT_CODES.SUCCESS;
	}
	else {
		CloseHandle(mutex);
		return EXIT_CODES.PARENT_EXISTS;
	}
}

static int execute_command(char* cmd_text) {
	std::cout << "Executing command: " << cmd_text << std::endl;
	SHA1 cmd;
	cmd.update(cmd_text);
	std::cout << cmd.final() << std::endl;
	const char* final_cmd = cmd.final().c_str();
	ipoc_arg(final_cmd);
	return EXIT_CODES.SUCCESS;
}

void run_server() {
	std::cout << "Parentized server running." << std::endl;
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
			switch (message.cmd) {
			case static_cast<uint32_t>(command_e::ping):
				std::cout << "Received Ping.\n";
				std::cout << "PID: " << message.pid << "\n";
				break;
			case static_cast<uint32_t>(command_e::terminate):
				std::cout << "Received terminate\n";
				exit(EXIT_CODES.FUCK);
				break;
			case static_cast<uint32_t>(command_e::exe_cmd):
				std::cout << "Received EXE_CMD\n";
				cmd_to_exe = true;
				// Handle exe_cmd logic here
				break;
			default: std::cout << "Unknown command\n";
				break;
			}
			if (cmd_to_exe)
				execute_command(message.cmd_args);
		}
		DisconnectNamedPipe(pipe);
		CloseHandle(pipe);
	}
}


void send_message(command_e cmd, const char* text) {
	// initialization of ipc_command struct that will be sent
	ipc_command command{};
	command.cmd = static_cast<uint32_t>(cmd);
	command.pid = GetCurrentProcessId();
	strcpy_s(command.cmd_args, sizeof(command.cmd_args), text);

	// now we connect...
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