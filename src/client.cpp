#include "client.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

Client::Client(const std::string &serverIp, int serverPort)
{
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "Failed to create socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    if (inet_pton(AF_INET, serverIp.c_str(), &serverAddr.sin_addr) <= 0)
    {
        std::cerr << "Invalid server IP address" << std::endl;
        exit(EXIT_FAILURE);
    }

    initSDL();
}

Client::~Client()
{
    cleanupSDL();
    close(sockfd);
}

void Client::sendRequest(const std::string &request)
{
    sendto(sockfd, request.c_str(), request.length(), 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
}

std::string Client::receiveResponse()
{
    char buffer[1024];
    int len = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, nullptr, nullptr);
    if (len > 0)
    {
        buffer[len] = '\0';
        return std::string(buffer);
    }
    return "";
}

void Client::drawText(int x, int y, const std::string &text, SDL_Color color)
{
    SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dstRect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void Client::drawBoard()
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (int i = 0; i <= BOARD_SIZE; ++i)
    {
        SDL_RenderDrawLine(renderer, i * 50, 0, i * 50, 450);
        SDL_RenderDrawLine(renderer, 0, i * 50, 450, i * 50);
    }

    SDL_Color color = {0, 0, 0};
    for (int row = 0; row < BOARD_SIZE; ++row)
    {
        for (int col = 0; col < BOARD_SIZE; ++col)
        {
            int value = game.getBoard()[row][col];
            if (value != 0)
            {
                drawText(col * 50 + 20, row * 50 + 10, std::to_string(value), color);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

void Client::initSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    if (TTF_Init() != 0)
    {
        std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    window = SDL_CreateWindow("Sudoku", 100, 100, 450, 450, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr)
    {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    font = TTF_OpenFont("arial.ttf", 24);
    if (font == nullptr)
    {
        std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Client::cleanupSDL()
{
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

void Client::run()
{
    bool quit = false;
    SDL_Event event;

    sendRequest("GET_GAME_STATE");
    std::string gameState = receiveResponse();
    game.deserialize(gameState);

    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        drawBoard();
    }
}
