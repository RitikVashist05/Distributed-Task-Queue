#include "Client.h"
#include "Server.h"
#include "Worker.h"
#include <cstdlib>

int main() {
    // Initialize server and client
    Server server;
    Client client;

    // Start the server in a separate thread
    thread serverThread(&Server::start, &server);
    serverThread.detach();

    // Input number of tasks to submit
    cout << "Enter number of tasks: ";
    int numTasks;
    cin >> numTasks;

    // Submit multiple tasks with priorities
    for (int i = 0; i < numTasks; ++i) {
        string task;
        int priority;
        cout << "Enter task description: ";
        cin >> task;
        cout << "Enter task priority (0 = highest priority): ";
        cin >> priority;
        client.submitTask(task, priority);
    }

    // Give time for tasks to process
    this_thread::sleep_for(chrono::seconds(30));

    return 0;
}
