#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define DEFAULT_PORT 8888

int main()
{
    WSADATA wsaData;
    SOCKET listenSocket, clientSocket;
    sockaddr_in serverAddr, clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    char recvBuffer[1024];

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "WSAStartup failed." << std::endl;
        return 1;
    }

    listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET)
    {
        std::cerr << "Failed to create socket." << std::endl;
        WSACleanup();
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(DEFAULT_PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        std::cerr << "Failed to bind socket." << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        std::cerr << "Failed to listen on socket." << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server listening on port " << DEFAULT_PORT << std::endl;

    while (true)
    {
        clientSocket = accept(listenSocket, (sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSocket == INVALID_SOCKET)
        {
            std::cerr << "Failed to accept connection." << std::endl;
            closesocket(listenSocket);
            WSACleanup();
            return 1;
        }

        int bytesReceived = recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
        if (bytesReceived == SOCKET_ERROR)
        {
            std::cerr << "Failed to receive data." << std::endl;
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }

        std::string message(recvBuffer, bytesReceived);
        std::cout << "Received message from client: " << message << std::endl;

        // 3초 뒤에 "hello world!" 문자열을 전송
        Sleep(3000);

        std::string response = "hello world!";
        send(clientSocket, response.c_str(), response.length(), 0);

        closesocket(clientSocket);
    }

    closesocket(listenSocket);
    WSACleanup();

    return 0;
}