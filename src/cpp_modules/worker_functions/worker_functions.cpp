#include "worker_functions.h"


void startSpeakerWorker(const std::string& ip, std::uint16_t port)
{
    const char *comand_turn_on = "$TXMIT,ON*\r\n";
    const char *comand_rotate = "$ANTSW,ON*\r\n";
    UDPSocket speaker;

    speaker.setupAddressToSend(ip, port);
    speaker.sendData(comand_turn_on);

    while (true)
        speaker.sendData(comand_rotate);
}


void startListenerWorker(
    std::uint16_t port, std::queue<std::string>& data_queue,
    std::mutex& data_queue_mutex
)
{
    UDPSocket listener;
    std::stringstream data_message;
    int line_amount = 3600;
    int counter = 0;

    listener.bindSocket(port);

    while (true)
    {
        data_message << listener.receiveData() << "\n";

        if (counter == line_amount - 1)
        {
            data_queue_mutex.lock();
            data_queue.push(data_message.str());
            data_queue_mutex.unlock();

            data_message.clear();
        }

        counter = (counter + 1) % line_amount;
    }
}


void startPainterWorker(
    std::queue<std::string>& data_queue, std::mutex& data_queue_mutex
)
{
    std::string path_to_data = "data.txt";          // заглушка
    std::string data_message = "";
    const char *path_to_script = "script.py";       // заглушка

    while (true)
    {
        data_queue_mutex.lock();
        if (!data_queue.empty())
        {
            data_message = data_queue.front();
            data_queue.pop();
        }
        data_queue_mutex.unlock();

        if (_save_to_file(path_to_data, data_message))
        {
            Py_Initialize();

            PyObject *obj = Py_BuildValue("s", path_to_script);
            FILE *fp = _Py_fopen_obj(obj, "r+");

            PyRun_SimpleFile(fp, path_to_script);
            Py_Finalize();
            fclose(fp);
        }
    }
}

bool _save_to_file(const std::string& path_to_file, const std::string& data)
{
    std::ofstream file_with_data(path_to_file);

    if (file_with_data.is_open())
    {
        file_with_data << data;
        file_with_data.close();
    }

    else
        return false;

    return true;
}
