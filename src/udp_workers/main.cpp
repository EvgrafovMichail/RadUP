#include <windows.h>
#include <iostream>
#include <csignal>
#include <thread>

#include "udp_workers.h"

#define RADAR_IP "223.168.1.168"
#define PORT_COMAND 3367
#define PORT_DATA 7203


BOOL WINAPI handleCtrlC(DWORD signal)
{
    if (signal == CTRL_C_EVENT)
    {
        handleCtrlCInCommandThread(CTRL_C_EVENT);
        handleCtrlCInServerThread(CTRL_C_EVENT);
    }

    return TRUE;
}


int main()
{
    SetConsoleCtrlHandler(handleCtrlC, TRUE);

    std::thread thread_command(startCommandThread, "127.0.0.1", 3367);
    std::thread thread_server(startServerThread, 7203);

    thread_command.join();
    thread_server.join();
}
