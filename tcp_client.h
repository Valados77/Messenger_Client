//
// Created by vladislav on 06.12.2025.
//

#ifndef MESSENGER_CLIENT_TCP_CLIENT_H
#define MESSENGER_CLIENT_TCP_CLIENT_H
#include "tcp_connection.h"
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class tcp_client {
public:
    explicit tcp_client(boost::asio::io_context& context);

private:
    boost::asio::io_context& context_;
    tcp::resolver resolver_;

    void start_connect();
    void handle_connect(const tcp_connection::pointer& new_connection, const boost::system::error_code& err);
};


#endif //MESSENGER_CLIENT_TCP_CLIENT_H