#ifndef WORKER_FUNCTIONS
#define WORKER_FUNCTIONS

#include <stdint.h>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>
#include <mutex>
#include <ctime>

#include "../udp_socket/udp_socket.h"

#include <windows.h>

#define QUEUE_SIZE_LIMIT 100
#define REPORT_AMOUNT 1000


void startSpeakerWorker(const std::string& ip, std::uint16_t port);
void startListenerWorker(
    std::uint16_t port, std::queue<std::string>& data_queue,
    std::mutex& data_queue_mutex
);
void startPainterWorker(
    std::queue<std::string>& data_queue, std::mutex& data_queue_mutex
);
void startRadarSimulatorWorker(const std::string& ip, std::uint16_t port);

bool _writeToFile(const std::string& path_to_file, const std::string& data);
void _exit();


#endif 
