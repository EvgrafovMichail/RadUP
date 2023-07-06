#ifndef UDP_WORKERS 
#define UDP_WORKERS

#pragma comment(lib, "ws2_32.lib")

#include <stdint.h>
#include <string>

#define BUFFER_SIZE 1472

void handleCtrlCInCommandWorker(int signal);
void handleCtrlCInServerWorker(int signal);

void startCommandWorker(const std::string& server_ip, std::uint16_t port);
void startServerWorker(std::uint16_t port);

#endif