#ifndef WORKER_H
#define WORKER_H

#include <iostream>
#include <thread>
#include <chrono>
#include <string>

class Worker {
public:
    void processTask(const std::string& task);
};

#endif // WORKER_H


