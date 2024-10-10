#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <queue>
#include <map>
#include <thread>
#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <mutex>

using namespace std;

class Server {
private:
    queue<string> taskQueue;
    map<int, bool> workers;
    mutex queueMutex;
    mutex workerMutex;

public:
    Server() {
        for (int i = 0; i < 5; ++i) {
            workers[i] = true;
        }
    }

    void start() {
        int server_fd, new_socket;
        struct sockaddr_in address;
        int addrlen = sizeof(address);

        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
            cout << "Socket creation failed" << endl;
            return;
        }

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(8080);

        if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
            cout << "Bind failed" << endl;
            return;
        }

        if (listen(server_fd, 3) < 0) {
            cout << "Listen failed" << endl;
            return;
        }

        cout << "Server started, waiting for tasks..." << endl;

        thread taskHandler(&Server::distributeTasks, this);
        taskHandler.detach();

        while (true) {
            if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
                cout << "Accept failed" << endl;
                continue;
            }

            char buffer[1024] = {0};
            read(new_socket, buffer, 1024);
            string task(buffer);

            {
                lock_guard<mutex> lock(queueMutex);
                taskQueue.push(task);
            }

            cout << "Task received: " << task << endl;
            close(new_socket);
        }
    }

    void distributeTasks() {
        while (true) {
            lock_guard<mutex> lock(queueMutex);
            if (!taskQueue.empty()) {
                for (auto& worker : workers) {
                    if (worker.second == true) {
                        string task = taskQueue.front();
                        taskQueue.pop();
                        thread workerThread(&Server::assignTaskToWorker, this, worker.first, task);
                        workerThread.detach();
                        break;
                    }
                }
            }
        }
    }

    void assignTaskToWorker(int workerId, const string& task) {
        {
            lock_guard<mutex> lock(workerMutex);
            workers[workerId] = false;
        }

        cout << "Worker " << workerId << " processing task: " << task << endl;
        this_thread::sleep_for(chrono::seconds(5));

        {
            lock_guard<mutex> lock(workerMutex);
            workers[workerId] = true;
        }

        cout << "Worker " << workerId << " completed task: " << task << endl;
    }
};

#endif
