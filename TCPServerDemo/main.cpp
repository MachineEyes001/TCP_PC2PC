#include <iostream>
#include <array>
#include <vector>
#include <boost/asio.hpp>

struct Position {
    int x;
    int y;
    int r;
};

void send_position_data(boost::asio::ip::tcp::socket& socket) {
    std::vector<Position> positions = {
        {10, 20, 30}, {40, 50, 60}, {70, 80, 90}, {100, 110, 120},
        {130, 140, 150}, {160, 170, 180}, {190, 200, 210}, {220, 230, 240}
    };

    boost::system::error_code ignored_error;
    boost::asio::write(socket, boost::asio::buffer(positions), ignored_error);
}

int main() {
    try {
        boost::asio::io_service io_service;
        boost::asio::ip::tcp::acceptor acceptor(io_service,
            boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 6000));

        std::cout << "Server is running, waiting for connection..." << std::endl;

        boost::asio::ip::tcp::socket socket(io_service);
        acceptor.accept(socket);

        std::cout << "Client connected" << std::endl;

        std::array<char, 128> buf;
        while (true) {
            boost::system::error_code error;
            size_t bytes_transferred = socket.read_some(boost::asio::buffer(buf), error);

            if (!error) {
                std::string message(buf.data(), bytes_transferred);
                std::cout << "Received message: " << message << std::endl;

                if (message == "START") {
                    send_position_data(socket);
                }
            }
            else {
                std::cerr << "Error: " << error.message() << std::endl;
                break;
            }
        }
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
