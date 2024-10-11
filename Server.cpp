#include "Server.h"

Server::Server() {
    // Initialize 5 workers, all available initially
    for (int i = 0; i < 5; ++i) {
        workers[i] = true;
    }
}

void Server::start() {
    WSADATA wsaData;
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cerr << "WSAStartup failed: " << iResult << std::endl;
        return;
    }

    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return;
    }

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;  // Listen on all interfaces
    address.sin_port = htons(8080);        // Server port

    // Bind the socket to the address and port
    if (bind(server_fd, (sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
        std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return;
    }

    // Listen for incoming connections
    if (listen(server_fd, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed: " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return;
    }

    std::cout << "Server started, waiting for tasks..." << std::endl;

    // Start the task distributor thread
    std::thread taskHandler(&Server::distributeTasks, this);
    taskHandler.detach();

    while (true) {
        SOCKET new_socket = accept(server_fd, nullptr, nullptr);
        if (new_socket == INVALID_SOCKET) {
            std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
            continue;
        }

        // Receive data from the client
        char buffer[1024] = {0};
        int recvResult = recv(new_socket, buffer, sizeof(buffer), 0);
        if (recvResult > 0) {
            std::string message(buffer, recvResult);
            size_t separator = message.find(":");
            if (separator != std::string::npos) {
                try {
                    int priority = std::stoi(message.substr(0, separator));
                    std::string taskDesc = message.substr(separator + 1);

                    {
                        std::lock_guard<std::mutex> lock(queueMutex);
                        taskQueue.push(Task{priority, taskDesc});
                        std::cout << "Task received: " << taskDesc << " with priority " << priority << std::endl;
                    }
                } catch (const std::exception& e) {
                    std::cerr << "Error parsing task: " << e.what() << std::endl;
                }
            } else {
                std::cerr << "Invalid task format received." << std::endl;
            }
        } else if (recvResult == 0) {
            std::cout << "Connection closed by client." << std::endl;
        } else {
            std::cerr << "Recv failed: " << WSAGetLastError() << std::endl;
        }

        closesocket(new_socket);
    }

    // Cleanup (unreachable in current design)
    closesocket(server_fd);
    WSACleanup();
}

void Server::distributeTasks() {
    while (true) {
        std::lock_guard<std::mutex> lock(queueMutex);
        if (!taskQueue.empty()) {
            for (auto& workerPair : workers) {
                if (workerPair.second) {  // If worker is available
                    Task task = taskQueue.top();
                    taskQueue.pop();

                    // Assign task to worker
                    std::thread workerThread(&Server::assignTaskToWorker, this, workerPair.first, task);
                    workerThread.detach();
                    break;  // Assign one task at a time
                }
            }
        }
        // Sleep to prevent busy waiting
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void Server::assignTaskToWorker(int workerId, const Task& task) {
    {
        std::lock_guard<std::mutex> lock(workerMutex);
        workers[workerId] = false;  // Mark worker as busy
    }

    std::cout << "Worker " << workerId << " processing task: " << task.description << std::endl;

    // Simulate task processing
    Worker worker;
    worker.processTask(task.description);

    {
        std::lock_guard<std::mutex> lock(workerMutex);
        workers[workerId] = true;  // Mark worker as available
    }

    std::cout << "Worker " << workerId << " completed task: " << task.description << std::endl;
}
