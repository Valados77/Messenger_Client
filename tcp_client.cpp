#include "tcp_client.h"


tcp_client::tcp_client(boost::asio::io_context &context)
    : context_(context), resolver_(context)
{
    start_connect();
}

void tcp_client::start_connect() {
    tcp_connection::pointer connection = tcp_connection::create(context_);
    tcp::resolver::results_type endpoints =
        resolver_.resolve("localhost", "daytime");
    boost::asio::async_connect(connection->socket(), endpoints,
        [this, connection](const boost::system::error_code& err, const tcp::endpoint&) {
            handle_connect(connection, err);
        });
}

void tcp_client::handle_connect(const tcp_connection::pointer& new_connection,
    const boost::system::error_code &err) {
    if (!err)
        new_connection->start();
}
