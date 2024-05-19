#include "server.h"
#include "client.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    if (argc != 2 && argc != 4)
    {
        std::cerr << "Usage: " << argv[0] << " <port> (for server)" << std::endl;
        std::cerr << "Usage: " << argv[0] << " <server_ip> <server_port> (for client)" << std::endl;
        return EXIT_FAILURE;
    }

    if (argc == 2)
    {
        int port = std::stoi(argv[1]);
        Server server(port);
        server.run();
    }
    else if (argc == 4)
    {
        std::string serverIp = argv[1];
        int serverPort = std::stoi(argv[2]);
        Client client(serverIp, serverPort);
        client.run();
    }

    return EXIT_SUCCESS;
}
