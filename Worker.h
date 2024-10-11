#ifndef WORKER_H
#define WORKER_H

#include <iostream>
#include <thread>

using namespace std;

class Worker {
public:
    // Simulate task processing
    void processTask(const string& task) {
        cout << "Worker processing task: " << task << endl;
        this_thread::sleep_for(chrono::seconds(3));  // Simulate processing time
        cout << "Worker completed task: " << task << endl;
    }
};

#endif // WORKER_H

