#include "Client.h"
#include "Server.h"
#include "Worker.h"

int main() {
    Server server;
    Client client;

    thread serverThread(&Server::start, &server);
    serverThread.detach();

    client.submitTask("Task 1");
    client.submitTask("Task 2");
    client.submitTask("Task 3");

    this_thread::sleep_for(chrono::seconds(20));

    return 0;
}
