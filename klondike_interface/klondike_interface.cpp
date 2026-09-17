#include <iostream>

#include "ipc_client.h"

int main()
{
    std::cout << "Klondike IPC client starting..." << std::endl;

    IPCClient ipc;

    std::cout << "Sending PING..." << std::endl;

    if (ipc.send(Command_e::ping, 0, "Hello from Klondike"))
    {
        std::cout << "PING sent successfully." << std::endl;
    }
    else
    {
        std::cout << "Failed to send PING." << std::endl;
    }

    return 0;
}