#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

class Client {
public:
    void submitTask(const std::string& task, int priority);
};

#endif // CLIENT_H


