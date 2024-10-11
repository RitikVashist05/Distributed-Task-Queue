#include "Client.h"
#include "Server.h"
#include "Worker.h"
#include <cstdlib>

int main() {
    Server server;
    Client client;

    thread serverThread(&Server::start, &server);
    serverThread.detach();

    cout << "Enter number of tasks: ";
    int numTasks;
    cin >> numTasks;

    for (int i = 0; i < numTasks; ++i) {
        string task;
        cout << "Enter task description: ";
        cin >> task;
        client.submitTask(task);
    }

    this_thread::sleep_for(chrono::seconds(20));

    return 0;
}
