#include "TCPClinet.h"

void TCPClient::connect() {
    boost::asio::ip::tcp::resolver::query query(host_, port_);
    boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver_.resolve(query);
    boost::asio::connect(socket_, endpoint_iterator);

    std::cout << "Connected to server" << std::endl;
}

void TCPClient::start_receiving() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        receive_position_data();

        for (const auto& pos : positions_) {
            std::cout << "CAM:" << pos.CAM << ", status:" << pos.success << ", Position - X: " << pos.x << ", Y: " << pos.y << ", R: " << pos.r << std::endl;
        }
    }
}

void TCPClient::receive_position_data() {
    std::array<char, 1024> buf;
    boost::system::error_code error;
    size_t bytes_transferred = socket_.read_some(boost::asio::buffer(buf), error);

    if (!error) {
        size_t num_positions = bytes_transferred / sizeof(Position);
        positions_.clear();
        const Position* data = reinterpret_cast<const Position*>(buf.data());
        positions_.insert(positions_.end(), data, data + num_positions);
    }
    else {
        std::cerr << "Error receiving position data: " << error.message() << std::endl;
    }
}

int main() {
    try {
        TCPClient client("192.168.1.20", "6000");

        client.connect();
        client.start_receiving();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
