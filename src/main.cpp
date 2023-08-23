#include <iostream>
#include <vector>
#include <thread>
#include <string>
#include <queue>
#include <mutex>

#include "cpp_modules/worker_functions/worker_functions.h"


BOOL WINAPI handleCtrlC(DWORD signal)
{
    if (signal == CTRL_C_EVENT)
        _exit();

    return TRUE;
}


int main()
{
    SetConsoleCtrlHandler(handleCtrlC, TRUE);

    std::queue<std::string> data_queue;
    std::mutex data_queue_mutex;

    std::thread speaker(startRadarSimulatorWorker, "127.0.0.1", 8000);
    std::thread listener(
        startListenerWorker, 8000,
        std::ref(data_queue), std::ref(data_queue_mutex)
    );
    std::thread painter(
        startPainterWorker, std::ref(data_queue), std::ref(data_queue_mutex)
    );

    speaker.join();
    listener.join();
    painter.join();

    std::system("invoke clean-up");
}
