// main.cpp
#include "Client.h"
#include "Server.h"
#include <cstdlib>
#include <thread>
#include <chrono>
#include <iostream>

using namespace std;

int main() {
    Server server;
    Client client;

    // Start the server in a separate thread
    thread serverThread(&Server::start, &server);
    serverThread.detach();

    cout << "Enter number of tasks: ";
    int numTasks;
    cin >> numTasks;

    for (int i = 0; i < numTasks; ++i) {
        string task;
        int priority;
        cout << "Enter task description: ";
        cin >> task;
        cout << "Enter task priority (0 = highest priority): ";
        cin >> priority;
        client.submitTask(task, priority);
    }

    // Allow some time for the server to process tasks before the program exits
    this_thread::sleep_for(chrono::seconds(20));

    return 0;
}
