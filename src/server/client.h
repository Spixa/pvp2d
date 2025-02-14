#pragma once

#include <SFML/Network.hpp>
#include <memory>

#include "logger.h"

namespace server {

class Client {
private:
    sf::TcpSocket* sock;
    std::string uname;
    bool is_nicked = false;

    sf::Vector2f world_pos;
public:
    Client(sf::TcpSocket* socket) : sock(socket) {

    }

    sf::Vector2f const& getPosition() const {
        return world_pos;
    }

    void setPosition(sf::Vector2f const& pos) {
        world_pos = pos;
    }

    ~Client() { delete sock; }

    sf::TcpSocket* socket() {
        return sock;
    };

    bool setName(std::string const& name) {
        if (!is_nicked) {
            this->uname = name;

            is_nicked = true;
            return true;
        }

        return false;
    }

    void send(sf::Packet& packet) {
        if (sock->send(packet) != sf::Socket::Status::Done) {
            logger::log("client").error("failed to send to client");
        }
    }

    std::optional<std::string> getName() const {
        if (is_nicked)
            return std::make_optional(uname);
        else
            return std::nullopt;
    }
};

}