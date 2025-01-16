#include <iostream>
#include <unistd.h>
#include <sstream>
#include <curl/curl.h>
#include <curl/curl.h>
#include <winsock2.h>

#include "ServerCommune.hpp"

using namespace std;

#pragma comment(lib, "Ws2_32.lib")

// Function to handle the response from the server
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Function to send data using sockets
std::string sendData(const std::string& data) {
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo* result = NULL, * ptr = NULL, hints;
    std::string response;

    // Initialize Winsock
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cerr << "WSAStartup failed: " << iResult << std::endl;
        return "";
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo("127.0.0.1", "5000", &hints, &result);
    if (iResult != 0) {
        std::cerr << "getaddrinfo failed: " << iResult << std::endl;
        WSACleanup();
        return "";
    }

    // Attempt to connect to the first address returned by the call to getaddrinfo
    ptr = result;
    ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    if (ConnectSocket == INVALID_SOCKET) {
        std::cerr << "Error at socket(): " << WSAGetLastError() << std::endl;
        freeaddrinfo(result);
        WSACleanup();
        return "";
    }

    // Connect to server
    iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        closesocket(ConnectSocket);
        ConnectSocket = INVALID_SOCKET;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        std::cerr << "Unable to connect to server!" << std::endl;
        WSACleanup();
        return "";
    }

    // Send the data
    iResult = send(ConnectSocket, data.c_str(), (int)strlen(data.c_str()), 0);
    if (iResult == SOCKET_ERROR) {
        std::cerr << "send failed: " << WSAGetLastError() << std::endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return "";
    }

    // Receive the response
    char recvbuf[512];
    iResult = recv(ConnectSocket, recvbuf, 512, 0);
    if (iResult > 0) {
        response = std::string(recvbuf, iResult);
    } else if (iResult == 0) {
        std::cout << "Connection closed" << std::endl;
    } else {
        std::cerr << "recv failed: " << WSAGetLastError() << std::endl;
    }

    // Cleanup
    closesocket(ConnectSocket);
    WSACleanup();

    return response;
}