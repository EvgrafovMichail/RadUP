#include "worker_functions.h"


void startSpeakerWorker(
    const std::string& ip, std::uint16_t port, int thread_id
)
{
    const char *comand_turn_on = "$TXMIT,ON*\r\n";
    const char *comand_rotate = "$ANTSW,ON*\r\n";
    UDPSocket speaker;

    speaker.setupAddressToSend(ip, port);
    speaker.sendData(comand_turn_on);

    while (true)
    {
        speaker.sendData(comand_rotate);
        std::cout << "[SPEAKER_" << thread_id << "]: ";
        std::cout << "send data;\n"; 
    }
}


void startListenerWorker(std::uint16_t port, int thread_id)
{
    UDPSocket listener;
    listener.bindSocket(port);

    while (true)
    {
        std::cout << "[LISTENER_" << thread_id << "]: ";
        std::cout << "receive data: ";
        std::cout << listener.receiveData() << ";\n";
    }
}
