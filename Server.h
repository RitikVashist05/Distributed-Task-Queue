#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <queue>
#include <map>
#include <thread>
#include <mutex>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <chrono>
#include "Worker.h"


// Task structure to store task description and priority
struct Task {
    int priority;
    std::string description;

    bool operator<(const Task& other) const {
        return priority > other.priority;  // Lower number = higher priority
    }
};

class Server {
private:
    std::priority_queue<Task> taskQueue;
    std::map<int, bool> workers;
    std::mutex queueMutex;
    std::mutex workerMutex;

public:
    Server();
    void start();
    void distributeTasks();
    void assignTaskToWorker(int workerId, const Task& task);
};

#endif // SERVER_H
