#include "server.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

Server::Server(int port) : port(port)
{
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "Failed to create socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        std::cerr << "Failed to bind socket" << std::endl;
        exit(EXIT_FAILURE);
    }
}

Server::~Server()
{
    close(sockfd);
}

void Server::sendGameState(int clientSock, struct sockaddr_in *clientAddr, socklen_t addrLen)
{
    std::string gameState = game.serialize();
    sendto(clientSock, gameState.c_str(), gameState.length(), 0, (struct sockaddr *)clientAddr, addrLen);
}

void Server::handleClient(int clientSock, struct sockaddr_in *clientAddr, socklen_t addrLen)
{
    char buffer[1024];
    int len = recvfrom(clientSock, buffer, sizeof(buffer), 0, (struct sockaddr *)clientAddr, &addrLen);
    if (len > 0)
    {
        buffer[len] = '\0';
        std::string command(buffer);

        if (command == "GET_GAME_STATE")
        {
            sendGameState(clientSock, clientAddr, addrLen);
        }
        else
        {
            std::cerr << "Unknown command: " << command << std::endl;
        }
    }
}

void Server::run()
{
    std::cout << "Server running on port " << port << std::endl;

    while (true)
    {
        struct sockaddr_in clientAddr;
        socklen_t addrLen = sizeof(clientAddr);
        handleClient(sockfd, &clientAddr, addrLen);
    }
}
