#include "udp_socket.h"


UDPSocket::UDPSocket():
    _wsa_was_initialized(false), _socket_was_created(false),
    _socket_was_binded(false)
{
    std::cout << "[INFO]: trying to initialize WinSock...";

    WSADATA wsa_data;
    _operation_result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    _wsa_was_initialized = _operation_result == NO_ERROR;

    if (!_wsa_was_initialized)
    {
        std::cout << "failed;\n";
        std::cout << "[ERROR]: initialization  was failed ";
        std::cout << "with code: " << _operation_result << ";\n";
    }

    else
    {
        std::cout << "success;\n";
        std::cout << "[INFO]: WinSock was successfully initialized;\n";
        std::cout << "[INFO]: trying to create socket...";

        _socket = socket(AF_INET, SOCK_DGRAM, 0);
        _socket_was_created = _socket != INVALID_SOCKET;

        if (!_socket_was_created)
        {
            std::cout << "failed;\n";
            std::cout << "[ERROR]: socket creation was failed ";
            std::cout << "with code: " << WSAGetLastError() << ";\n";

            WSACleanup();
            _wsa_was_initialized = false;
        }
        
        else
        {
            std::cout << "success;\n";
            std::cout << "[INFO]: socket was successfully created;\n";
        }
    }
}


UDPSocket::~UDPSocket()
{
    _cleanupSocket();
    std::cout << "[INFO]: socket was cleaned up;" << std::endl;
}


void UDPSocket::bindSocket(std::uint16_t port)
{
    if (!_isSocketInitialized())
        return;

    std::cout << "[INFO]: trying to bind socket...";

    _address_socket.sin_family = AF_INET;
    _address_socket.sin_addr.s_addr = INADDR_ANY;
    _address_socket.sin_port = htons(port);

    _operation_result = bind(
        _socket, (struct sockaddr*) &_address_socket,
        sizeof(_address_socket)
    );

    if (_operation_result != 0)
    {
        std::cout << "failed;\n";
        std::cout << "[ERROR]: binding was failed ";
        std::cout << "with code: " << WSAGetLastError() << ";\n";

        _cleanupSocket();
        return;
    }

    _socket_was_binded = true;

    std::cout << "success;\n";
    std::cout << "[INFO]: socket was successfully binded;\n";
}


void UDPSocket::setupAddressToSend(const std::string& ip, std::uint16_t port)
{
    memset((char*) &_address_to_send, 0, sizeof(_address_to_send));

    _address_to_send.sin_family = AF_INET;
    _address_to_send.sin_port = htons(port);
    _address_to_send.sin_addr.S_un.S_addr = inet_addr(ip.c_str());
}


void UDPSocket::sendData(const char *data)
{
    if (!_isSocketInitialized())
        return;

    _operation_result = sendto(
        _socket, data, strlen(data), 0,
        (struct sockaddr*) &_address_to_send, sizeof(_address_to_send)
    );

    if (_operation_result == SOCKET_ERROR)
    {
        std::cout << "[ERROR]: data sending was failed ";
        std::cout << "with code: " << WSAGetLastError() << ";\n";
    }
}


std::string UDPSocket::receiveData()
{
    if (!_isSocketInitialized())
        return "";

    if (!_socket_was_binded)
    {
        std::cout << "[ERROR]: socket wasn't binded, ";
        std::cout << "impossible to receive any data;\n";

        return "";
    }

    int size_of_address = sizeof(_address_to_receive);
    memset(_buffer, '\0', BUFFER_SIZE);

    _operation_result = recvfrom(
        _socket, _buffer, BUFFER_SIZE, 0,
        (struct sockaddr*) &_address_to_receive, &size_of_address
    );

    if (_operation_result == SOCKET_ERROR)
    {
        std::cout << "[ERROR]: data receiving was failed ";
        std::cout << "with code: " << WSAGetLastError() << ";\n";

        return "";        
    }

    std::stringstream data_received;
    data_received << "[";

    for (int i = 0; i < BUFFER_SIZE; ++i)
    {
        data_received << static_cast<int>(_buffer[i]);

        if (i < BUFFER_SIZE - 1)
            data_received << ", ";
    }

    data_received << "]";
    return data_received.str();
}


void UDPSocket::_cleanupSocket()
{
    if (_socket_was_created)
    {
        closesocket(_socket);
        _socket_was_created = false;
    }

    if (_wsa_was_initialized)
    {
        WSACleanup();
        _wsa_was_initialized = false;
    }
}


bool UDPSocket::_isSocketInitialized()
{
    if (!_wsa_was_initialized)
    {
        std::cout << "[ERROR]: WinSock wasn't initialized;\n";
        return false;
    }

    if (!_socket_was_created)
    {
        std::cout << "[ERROR]: socket wasn't created;\n";
        return false;
    }

    return true;
}
