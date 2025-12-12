#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "tcp_client.h"


using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: client <host> <port>" << std::endl;
        return 1;
    }
    std::string host = argv[1];
    std::string port = argv[2];

    try
    {
        auto io_context = std::make_shared<boost::asio::io_context>();

        std::cout << "[Main] Client started.\n";

        auto client = std::make_shared<tcp_client>(*io_context, host, port);
        client->start_connect();

        io_context->run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
