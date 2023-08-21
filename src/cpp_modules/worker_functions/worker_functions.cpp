#include "worker_functions.h"


bool should_speak = true;
bool should_listen = true;
bool should_paint = true;
bool should_simulate = true;


void startSpeakerWorker(const std::string& ip, std::uint16_t port)
{
    const char *comand_turn_on = "$TXMIT,ON*\r\n";
    const char *comand_rotate = "$ANTSW,ON*\r\n";
    UDPSocket speaker;

    speaker.setupAddressToSend(ip, port);
    speaker.sendData(comand_turn_on);

    while (should_speak)
        speaker.sendData(comand_rotate);
}


void startListenerWorker(
    std::uint16_t port, std::queue<std::string>& data_queue,
    std::mutex& data_queue_mutex
)
{
    std::stringstream data_message;
    int line_amount = 3600;
    int counter = 0;

    UDPSocket listener;
    listener.bindSocket(port);

    while (should_listen)
    {
        if (!counter)
            data_message << "[";

        data_message << listener.receiveData();

        if (counter == line_amount - 1)
        {
            data_message << "]";

            data_queue_mutex.lock();
            data_queue.push(data_message.str());
            data_queue_mutex.unlock();

            data_message.str("");
        }

        else
            data_message << ", ";

        counter = (counter + 1) % line_amount;
    }
}


void startPainterWorker(
    std::queue<std::string>& data_queue, std::mutex& data_queue_mutex
)
{
    std::string command_string = "python python_modules/visualize.py ";
    std::string path_to_data = "";
    std::string data_message = "";
    std::string command = "";

    size_t image_id = 0;
    int file_id = 0;

    while (should_paint)
    {
        data_queue_mutex.lock();
        if (!data_queue.empty())
        {
            data_message = data_queue.front();
            data_queue.pop();
        }
        data_queue_mutex.unlock();

        if (data_message.size())
        {
            path_to_data = "radar_data" + std::to_string(file_id) + ".json";
            command = command_string + std::to_string(file_id) + " ";
            command += std::to_string(image_id);

            _writeToFile(path_to_data, data_message);
            std::system(command.c_str());

            file_id = (file_id + 1) % 5;
            ++image_id;
            
            data_message = "";
            command = "";
        }
    }
}


void startRadarSimulatorWorker(const std::string& ip, std::uint16_t port)
{
    srand(static_cast<unsigned>(time(0)));

    UDPSocket speaker;
    std::string data_string;

    speaker.setupAddressToSend(ip, port);

    while (should_simulate)
    {
        data_string = "";
        
        for (int i = 0; i < REPORT_AMOUNT; ++i)
            data_string += static_cast<char>(rand() % 26 + 'a');

        speaker.sendData(data_string.c_str());
    }
}


bool _writeToFile(const std::string& path_to_file, const std::string& data)
{
    std::ofstream data_file(path_to_file);

    if (!data_file.is_open())
        return false;

    data_file << data;
    data_file.close();

    return true;
}


void _exit()
{
    should_speak = false;
    should_listen = false;
    should_paint = false;
    should_simulate = false;
}
