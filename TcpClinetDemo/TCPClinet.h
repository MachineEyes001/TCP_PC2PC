#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include <boost/asio.hpp>

struct Position {
    int CAM;
    int success;
    int x;
    int y;
    int r;
};

class TCPClient {
public:
    TCPClient(const std::string& host, const std::string& port)
        : io_service_(), resolver_(io_service_), socket_(io_service_), host_(host), port_(port) {}

    void connect();
    void start_receiving();

private:
    void receive_position_data();

    boost::asio::io_service io_service_;
    boost::asio::ip::tcp::resolver resolver_;
    boost::asio::ip::tcp::socket socket_;
    std::string host_;
    std::string port_;
    std::vector<Position> positions_;
};


