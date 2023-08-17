#ifndef WORKER_FUNCTIONS
#define WORKER_FUNCTIONS

#include <Python.h>
#include <stdint.h>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>
#include <mutex>

#include "../udp_socket/udp_socket.h"


void startSpeakerWorker(const std::string& ip, std::uint16_t port);
void startListenerWorker(
    std::uint16_t port, std::queue<std::string>& data_queue,
    std::mutex& data_queue_mutex
);
void startPainterWorker(
    std::queue<std::string>& data_queue, std::mutex& data_queue_mutex
);

bool _save_to_file(const std::string& path_to_file, const std::string& data);


#endif 
