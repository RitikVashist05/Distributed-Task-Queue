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

// Task structure to store task description and priority
struct Task {
    int priority;
    string description;

    // Define operator for priority comparison (lower number = higher priority)
    bool operator<(const Task& other) const {
        return priority > other.priority;
    }
};

class Server {
private:
    // Priority queue to store tasks with priority-based scheduling
    priority_queue<Task> taskQueue;
    map<int, bool> workers;  // Worker availability status
    mutex queueMutex;        // Mutex for thread-safe task queue access
    mutex workerMutex;       // Mutex for thread-safe worker management

public:
    Server() {
        // Initialize worker statuses (all workers are available initially)
        for (int i = 0; i < 5; ++i) {
            workers[i] = true;
        }
    }

    // Start the server to accept incoming task submissions
    void start() {
        int server_fd, new_socket;
        struct sockaddr_in address;
        int addrlen = sizeof(address);

        // Create server socket
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
            cout << "Socket creation failed" << endl;
            return;
        }

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(8080);

        // Bind the socket to the server address and port
        if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
            cout << "Bind failed" << endl;
            return;
        }

        // Listen for incoming connections
        if (listen(server_fd, 3) < 0) {
            cout << "Listen failed" << endl;
            return;
        }

        cout << "Server started, waiting for tasks..." << endl;

        // Start a thread to distribute tasks to workers
        thread taskHandler(&Server::distributeTasks, this);
        taskHandler.detach();

        // Accept and handle incoming client task submissions
        while (true) {
            if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
                cout << "Accept failed" << endl;
                continue;
            }

            // Receive task from the client
            char buffer[1024] = {0};
            read(new_socket, buffer, 1024);
            string message(buffer);

            // Extract priority and task description
            size_t separator = message.find(":");
            int priority = stoi(message.substr(0, separator));
            string task = message.substr(separator + 1);

            // Add task to the priority queue
            {
                lock_guard<mutex> lock(queueMutex);
                taskQueue.push({priority, task});
                cout << "Task received: " << task << " with priority " << priority << endl;
            }

            // Debugging output
            cout << "Task added to queue. Queue size is now: " << taskQueue.size() << endl;

            close(new_socket);
        }
    }

    // Distribute tasks to workers based on availability and priority
    void distributeTasks() {
        while (true) {
            lock_guard<mutex> lock(queueMutex);
            if (!taskQueue.empty()) {
                for (auto& worker : workers) {
                    if (worker.second == true) {
                        Task task = taskQueue.top();
                        taskQueue.pop();
                        thread workerThread(&Server::assignTaskToWorker, this, worker.first, task);
                        workerThread.detach();
                        break;
                    }
                }
            }
        }
    }

    // Assign task to an available worker
    void assignTaskToWorker(int workerId, const Task& task) {
        {
            lock_guard<mutex> lock(workerMutex);
            workers[workerId] = false;  // Mark worker as busy
        }

        cout << "Worker " << workerId << " processing task: " << task.description << endl;
        this_thread::sleep_for(chrono::seconds(5));  // Simulate task processing time

        {
            lock_guard<mutex> lock(workerMutex);
            workers[workerId] = true;  // Mark worker as available again
        }

        cout << "Worker " << workerId << " completed task: " << task.description << endl;
    }
};

#endif
