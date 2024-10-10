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
    void submitTask(const string& task) {
        int sock = 0;
        struct sockaddr_in serv_addr;

        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            cout << "Socket creation error" << endl;
            return;
        }

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(8080);

        if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
            cout << "Invalid address" << endl;
            return;
        }

        if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
            cout << "Connection failed" << endl;
            return;
        }

        send(sock, task.c_str(), task.length(), 0);
        cout << "Task submitted: " << task << endl;

        close(sock);
    }
};

#endif
