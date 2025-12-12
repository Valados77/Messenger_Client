#ifndef MESSENGER_CLIENT_TCP_CONNECTION_H
#define MESSENGER_CLIENT_TCP_CONNECTION_H

#include <boost/asio.hpp>
#include <deque>

using boost::asio::ip::tcp;

class tcp_client;

class tcp_connection
    : public std::enable_shared_from_this<tcp_connection>
{
public:
    explicit tcp_connection(tcp::socket socket, std::shared_ptr<tcp_client> observer);
    void start();
    void close();

    void deliver(const std::string& msg);
private:
    tcp::socket socket_;
    std::shared_ptr<tcp_client> observer_;
    boost::asio::streambuf buffer_;
    std::deque<std::string> message_;

    void do_read();
    void handle_read(const boost::system::error_code& error, size_t bytes_transferred);
    void do_write();
    void handle_write(const boost::system::error_code& error);
};


#endif //MESSENGER_CLIENT_TCP_CONNECTION_H