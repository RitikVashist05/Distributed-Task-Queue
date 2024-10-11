#include "Client.h"

void Client::submitTask(const std::string& task, int priority) {
    WSADATA wsaData;
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cerr << "WSAStartup failed: " << iResult << std::endl;
        return;
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket creation error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return;
    }

    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);  // Server port

    // Convert IP address from text to binary
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address" << std::endl;
        closesocket(sock);
        WSACleanup();
        return;
    }

    // Connect to the server
    if (connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection failed: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return;
    }

    // Send task to the server with the task priority
    std::string message = std::to_string(priority) + ":" + task;
    int sendResult = send(sock, message.c_str(), static_cast<int>(message.length()), 0);
    if (sendResult == SOCKET_ERROR) {
        std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
    } else {
        std::cout << "Submitting task: " << task << " with priority: " << priority << std::endl;
    }

    closesocket(sock);
    WSACleanup();
}
