#ifndef CLIENT_H
#define CLIENT_H

#include "game.h"
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Client
{
public:
    Client(const std::string &serverIp, int serverPort);
    ~Client();
    void run();

private:
    int sockfd;
    struct sockaddr_in serverAddr;
    Game game;
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;

    void sendRequest(const std::string &request);
    std::string receiveResponse();
    void drawBoard();
    void drawText(int x, int y, const std::string &text, SDL_Color color);
    void initSDL();
    void cleanupSDL();
};

#endif // CLIENT_H
