#ifndef MESSENGER_CLIENT_TCP_CLIENT_H
#define MESSENGER_CLIENT_TCP_CLIENT_H
#include <boost/asio.hpp>

#include "tcp_connection.h"

using boost::asio::ip::tcp;

class tcp_client : public std::enable_shared_from_this<tcp_client>
{
public:
    explicit tcp_client(boost::asio::io_context& io_context, std::string& host, std::string& port);
    void start_connect();

    void send_loop();
    void on_message_received(std::shared_ptr<tcp_connection> connection, const std::string& message);
    void on_disconnected(std::shared_ptr<tcp_connection> connection);
private:
    boost::asio::io_context& io_context_;
    tcp::resolver::results_type endpoints_;
    std::shared_ptr<tcp_connection> connection_;

    void do_connect();
};


#endif  // MESSENGER_CLIENT_TCP_CLIENT_H
