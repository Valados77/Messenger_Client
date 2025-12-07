#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "tcp_client.h"


using boost::asio::ip::tcp;

int main(int argc, char *argv[]) {
    /*if (argc != 2) {
        std::cerr << "Usage: client <host>" << std::endl;
        return 1;
    }*/

    boost::asio::io_context io_context;
    tcp_client client(io_context);
    io_context.run();
    return 0;
}