#include "tcp_connection.h"

#include <iostream>

#include "tcp_client.h"

tcp_connection::tcp_connection(tcp::socket socket, std::shared_ptr<tcp_client> observer)
    : socket_(std::move(socket)), observer_(std::move(observer)) { }


void tcp_connection::start() {
    do_read();
}

void tcp_connection::close() {
    if (socket_.is_open()) {
        boost::system::error_code ec;
        socket_.shutdown(tcp::socket::shutdown_both, ec);
        socket_.close(ec);
    }
}

void tcp_connection::deliver(const std::string& msg) {
    message_.push_back(msg + "\n");

    if (message_.size() > 1) {
        return;
    }
    do_write();
}

void tcp_connection::do_read() {
    boost::asio::async_read_until(
            socket_, buffer_, '\n',
            [self = shared_from_this()](const boost::system::error_code& error, size_t bytes_transferred) {
                self->handle_read(error, bytes_transferred);
            });
}

void tcp_connection::handle_read(const boost::system::error_code& error, size_t bytes_transferred) {
    if (!error) {
        std::istream is(&buffer_);
        std::string line;
        std::getline(is, line);

        observer_->on_message_received(shared_from_this(), line);

        do_read();

    } else if (error == boost::asio::error::eof || error == boost::asio::error::connection_reset) {
        std::cout << "[Client] Disconnected gracefully.\n";
        observer_->on_disconnected(shared_from_this());
    } else {
        std::cerr << "[Client] Read error: " << error.message() << '\n';
        observer_->on_disconnected(shared_from_this());
    }
}

void tcp_connection::do_write() {
    boost::asio::async_write(socket_, boost::asio::buffer(message_.front()),
                             [self = shared_from_this()](const boost::system::error_code& error,
                                                         size_t /*bytes_transferred*/) { self->handle_write(error); });
}

void tcp_connection::handle_write(const boost::system::error_code& error) {
    if (!error) {
        message_.pop_front();
        if (!message_.empty()) {
            do_write();
        }
    } else {
        std::cerr << "[Client] Write error: " << error.message() << '\n';
        observer_->on_disconnected(shared_from_this());
    }
}