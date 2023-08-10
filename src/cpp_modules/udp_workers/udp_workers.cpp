#include <winsock2.h>
#include <iostream>
#include <stdlib.h>
#include <csignal>

#include "udp_workers.h"


bool should_send_command = true;
bool should_receive_data = true;


void handleCtrlCInCommandWorker(int signal)
{
    should_send_command = false;
}


void handleCtrlCInServerWorker(int signal)
{
    should_receive_data = false;
}


void startCommandWorker(const std::string& server_ip, std::uint16_t port)
{
    // инициализируем библиотеку для работы с UDP
    WSADATA wsa_data;
    int operation_result;

    std::signal(SIGINT, handleCtrlCInCommandWorker);

    std::cout << "[INFO]: trying to initialize Winsock...\n";
    operation_result = WSAStartup(MAKEWORD(2, 2), &wsa_data);

    if (operation_result != 0)
    {
        std::cout << "[ERROR]: initialization was failed ";
        std::cout << "with code: " << WSAGetLastError() << ";" << std::endl;

        exit(EXIT_FAILURE);
    }

    // создаем сокет для отправки команд управления
    std::cout << "[INFO]: WinSock was succesfully initialized;\n";
    std::cout << "[INFO]: trying to create socket...\n";

    SOCKET command_socket = socket(AF_INET, SOCK_DGRAM, 0);

    if (command_socket == INVALID_SOCKET)
    {
        std::cout << "[ERROR]: socket creation was failed ";
        std::cout << "with code: " << WSAGetLastError() << ";" << std::endl;

        WSACleanup();
        exit(EXIT_FAILURE);
    }

    std::cout << "[INFO]: socket was successfully created;\n";
    std::cout << "[INFO]: make necessary preparations;\n";

    // заполняем данные о радаре, как об устройстве в сети
    sockaddr_in server_address;

    memset((char *) &server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.S_un.S_addr = inet_addr(server_ip.c_str());

    // описываем команды для включения излучения и вращения антенны
    const char *command_transmition = "$TXMIT,ON*\r\n";
    const char *command_rotation = "$ANTSW,ON*\r\n";

    // отправляем команду на включение излючения
    std::cout << "[INFO]: trying to turn on the transmition...\n";

    operation_result = sendto(
        command_socket, command_transmition, strlen(command_transmition),
        0, (struct sockaddr*) &server_address, sizeof(server_address)
    );

    if (operation_result == SOCKET_ERROR)
    {
        std::cout << "[ERROR]: data sending was failed ";
        std::cout << "with code: " << WSAGetLastError() << ";" << std::endl;

        closesocket(command_socket);
        WSACleanup();

        exit(EXIT_FAILURE);
    }

    std::cout << "[INFO]: transmition was successfully turned on;\n";
    std::cout << "[INFO]: start rotation;\n";

    // отправляем команды на вращение радиолокатора
    while (should_send_command)
    {
        operation_result = sendto(
            command_socket, command_rotation, strlen(command_rotation),
            0, (struct sockaddr*) &server_address, sizeof(server_address)
        );

        if (operation_result == SOCKET_ERROR)
        {
            std::cout << "[ERROR]: data sending was failed ";
            std::cout << "with code: " << WSAGetLastError() << ";" << std::endl;

            closesocket(command_socket);
            WSACleanup();

            exit(EXIT_FAILURE);
        }
    }

    std::cout << "[INFO]: stop rotation;\n";
    std::cout << "[INFO]: stop command thread;" << std::endl;
    
    closesocket(command_socket);
    WSACleanup();
}


void startServerWorker(std::uint16_t port)
{
    // инициализируем библиотеку для работы с UDP
    WSADATA wsa_data;
    int operation_result;

    std::signal(SIGINT, handleCtrlCInServerWorker);

    std::cout << "[INFO]: trying to initialize Winsock...\n";
    operation_result = WSAStartup(MAKEWORD(2, 2), &wsa_data);

    if (operation_result != 0)
    {
        std::cout << "[ERROR]: initialization was failed ";
        std::cout << "with code: " << WSAGetLastError() << ";" << std::endl;

        exit(EXIT_FAILURE);
    }

    std::cout << "[INFO]: WinSock was succesfully initialized;\n";
    std::cout << "[INFO]: trying to create socket...\n";

    SOCKET reciever_socket = socket(AF_INET, SOCK_DGRAM, 0);

    if (reciever_socket == INVALID_SOCKET)
    {
        std::cout << "[ERROR]: socket creation was failed ";
        std::cout << "with code: " << WSAGetLastError() << ";" << std::endl;

        WSACleanup();
        exit(EXIT_FAILURE);
    }

    std::cout << "[INFO]: socket was successfully created;\n";
    std::cout << "[INFO]: trying to bind socket...\n";

    sockaddr_in reciever_address;
    reciever_address.sin_family = AF_INET;
    reciever_address.sin_addr.s_addr = INADDR_ANY;
    reciever_address.sin_port = htons(port);

    operation_result = bind(
        reciever_socket, (struct sockaddr*) &reciever_address,
        sizeof(reciever_address)
    );

    if (operation_result != 0)
    {
        std::cout << "[ERROR]: binding was failed ";
        std::cout << "with code: " << WSAGetLastError() << ";" << std::endl;

        closesocket(reciever_socket);
        WSACleanup();

        exit(EXIT_FAILURE);
    }

    std::cout << "[INFO]: socket was successfully binded;\n";
    std::cout << "[INFO]: make necessary preparations;\n";

    DWORD timeout = 1000;
    operation_result = setsockopt(
        reciever_socket, SOL_SOCKET, SO_RCVTIMEO, (const char *) &timeout, sizeof(timeout)
    );

    if (operation_result == SOCKET_ERROR)
    {
        std::cout << "[ERROR]: setting socket options was failed ";
        std::cout << "with code: " << WSAGetLastError() << ";" << std::endl;

        closesocket(reciever_socket);
        WSACleanup();
        
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];

    sockaddr_in client_address;
    int size_of_client_address = sizeof(client_address);

    std::cout << "[INFO]: start to recieve data;\n";

    while (should_receive_data)
    {
        memset(buffer, '\0', BUFFER_SIZE);

        operation_result = recvfrom(
            reciever_socket, buffer, BUFFER_SIZE, 0,
            (struct sockaddr*) &client_address, &size_of_client_address
        );

        if (operation_result == SOCKET_ERROR)
        {
            operation_result = WSAGetLastError();

            if (operation_result != WSAETIMEDOUT)
            {
                std::cout << "[ERROR]: data receiving was failed ";
                std::cout << "with code: " << operation_result << ";" << std::endl;

                closesocket(reciever_socket);
                WSACleanup();

                exit(EXIT_FAILURE);
            }

            std::cout << "[INFO]: timeout exceeded;\n";
        }
    }

    std::cout << "[INFO]: stop data receiving;\n";
    std::cout << "[INFO]: stop server thread;" << std::endl;

    closesocket(reciever_socket);
    WSACleanup();
}