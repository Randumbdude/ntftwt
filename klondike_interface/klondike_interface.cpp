#include <cstdio>
#include "ipc_client.h"

int main() {
	printf("Klondike IPC client starting...\n");
	IPCClient ipc;
	printf("Sending PING...\n");
	ipc_command cmd{};
	cmd._cmd_ = command_e::ping;
	cmd.pid = 0;
	cmd.text[0] = '\0';

	if (ipc.send(cmd)) {
		printf("PING sent successfully.\n");
	}
	else {
		printf("Failed to send PING.\n");
	}
	return 0;
}
