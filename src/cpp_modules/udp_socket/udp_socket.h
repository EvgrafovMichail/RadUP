#ifndef UDP_SOCKET
#define UDP_SOCKET

#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <sstream>
#include <memory>
#include <string>

#define BUFFER_SIZE 1472


class UDPSocket
{
    public:
        UDPSocket();
        ~UDPSocket();
        
        void bindSocket(std::uint16_t port);
        void setupAddressToSend(const std::string& ip, std::uint16_t port);

        void sendData(const char *data);
        std::string receiveData(); 

    private:
        void _cleanupSocket();
        bool _isSocketInitialized();

        int _operation_result;
        bool _wsa_was_initialized;
        bool _socket_was_created;
        bool _socket_was_binded;

        char _buffer[BUFFER_SIZE];

        SOCKET _socket;
        sockaddr_in _address_socket;
        sockaddr_in _address_to_send;
        sockaddr_in _address_to_receive;
};


#endif