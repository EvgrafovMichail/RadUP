#ifndef UDP_WORKERS 
#define UDP_WORKERS

#pragma comment(lib, "ws2_32.lib")

#include <stdint.h>
#include <string>

#define BUFFER_SIZE 1472

void handleCtrlCInCommandThread(int signal);
void handleCtrlCInServerThread(int signal);

void startCommandThread(const std::string& server_ip, std::uint16_t port);
void startServerThread(std::uint16_t port);

#endif