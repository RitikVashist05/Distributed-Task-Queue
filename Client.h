#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

class Client {
public:
    // Submit task to the server with a description
    void submitTask(const string& task, int priority) {
        int sock = 0;
        struct sockaddr_in serv_addr;

        // Create socket
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            cout << "Socket creation error" << endl;
            return;
        }

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(8080);  // Server port

        // Convert IP address from text to binary
        if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
            cout << "Invalid address" << endl;
            return;
        }

        // Connect to the server
        if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
            cout << "Connection failed" << endl;
            return;
        }

        // Send task to the server with the task priority
        string message = to_string(priority) + ":" + task;
        send(sock, message.c_str(), message.length(), 0);

        // Debugging output
        cout << "Submitting task: " << task << " with priority: " << priority << endl;

        close(sock);
    }
};

#endif // CLIENT_H
