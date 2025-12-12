#include "tcp_client.h"

#include <iostream>


tcp_client::tcp_client(boost::asio::io_context& io_context, std::string& host, std::string& port)
    : io_context_(io_context)
{
    tcp::resolver resolver(io_context);
    endpoints_ = resolver.resolve(host, port);
    std::cout << "[Client] Connector started on adress: " << port << '\n';
}

void tcp_client::start_connect() { do_connect(); }

void tcp_client::do_connect()
{
    auto socket = std::make_shared<tcp::socket>(io_context_);
    boost::asio::async_connect(
        *socket, endpoints_,
        [self = shared_from_this(), socket](const boost::system::error_code& error, const tcp::endpoint& ep)
        {
            if (!error)
            {
                self->connection_ = std::make_shared<tcp_connection>(std::move(*socket), self);
                self->connection_->start();
                std::cout << "[Client] Connected. You can type messages.\n";
                self->send_loop();
            }
        });
}

void tcp_client::send_loop()
{
    auto self = shared_from_this();
    std::thread([self]() {
        std::string msg;
        while (std::getline(std::cin, msg)) {
            if (!msg.empty()) {
                // Используем async post
                boost::asio::post(self->io_context_, [self, msg]() {
                    if (self->connection_) {
                        self->connection_->deliver(msg);
                    }
                });
            }
        }
    }).detach();
}

void tcp_client::on_message_received(std::shared_ptr<tcp_connection> session, const std::string& message)
{
    std::cout << message << std::endl;
}

void tcp_client::on_disconnected(std::shared_ptr<tcp_connection> session)
{
    session->close();
    std::cout << "[Client] Session disconected";
}
