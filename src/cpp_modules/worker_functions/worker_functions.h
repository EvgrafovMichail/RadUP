#ifndef WORKER_FUNCTIONS
#define WORKER_FUNCTIONS

#include <iostream>
#include <stdint.h>
#include <string>

#include "../udp_socket/udp_socket.h"


void startSpeakerWorker(const std::string& ip, std::uint16_t port, int thread_id);
void startListenerWorker(std::uint16_t port, int thread_id);


#endif 
