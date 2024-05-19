#ifndef SERVER_H
#define SERVER_H

#include "game.h"
#include <string>

class Server
{
public:
    Server(int port);
    ~Server();
    void run();

private:
    int sockfd;
    int port;
    struct sockaddr_in serverAddr;
    Game game;

    void sendGameState(int clientSock, struct sockaddr_in *clientAddr, socklen_t addrLen);
    void handleClient(int clientSock, struct sockaddr_in *clientAddr, socklen_t addrLen);
};

#endif // SERVER_H
