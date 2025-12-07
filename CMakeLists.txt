cmake_minimum_required(VERSION 4.0)
project(Messenger_Client)

set(CMAKE_CXX_STANDARD 20)

add_executable(Messenger_Client main.cpp
        tcp_client.cpp
        tcp_client.h
        tcp_connection.cpp
        tcp_connection.h)

find_package(Boost REQUIRED COMPONENTS system thread)
target_link_libraries(Messenger_Client PRIVATE Boost::system Boost::thread)
