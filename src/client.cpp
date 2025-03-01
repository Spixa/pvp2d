#include "client.h"
#include "game.h"

ClientNetwork::ClientNetwork() {
    cinfo("Intialized game client");
}

void ClientNetwork::connect(std::string const& ip, unsigned short port) {
    if (socket.connect(sf::IpAddress::resolve(ip).value(), port) != sf::Socket::Status::Done) {
        cerror("Could not connect to the server");
        cerror("Bad server");

        // bad design rn
        exit(0);
    } else {
        is_connected = true;
        cinfo("Connected to " + ip + ":" + std::to_string(port));
    }
}

void ClientNetwork::receivePackets(sf::TcpSocket* sock) {
    while (true) {
        if (sock->receive(last_packet) == sf::Socket::Status::Done) {
            net::Packet type;

            last_packet >> type;

            if (type == net::Packet::PingPacket) {
                std::lock_guard<std::mutex> guard(pingTimeMutex);
                last_ping = pingTime.getElapsedTime().asMilliseconds();
            } else if (type == net::Packet::UpdatePlayerListPacket) {
                int c;
                last_packet >> c;

                ctrace("Player count prior to join: " + std::to_string(c));

                for (int i = 0; i < c; i++) {
                    std::string name;
                    float x, y;

                    last_packet >> name >> x >> y;

                    if (!Game::getInstance()->getStateManager().getGameState()->exists(name))
                        Game::getInstance()->getStateManager().getGameState()->spawn(name, sf::Vector2f(x, y));
                }
                last_packet.clear();
            } else if (type == net::Packet::UpdatePositionPacket) {
                std::string name;
                float newX, newY;
                last_packet >> name >> newX >> newY;

                if (Game::getInstance()->getName() != name) {
                    if (Game::getInstance()->getStateManager().getGameState()->exists(name)) 
                        Game::getInstance()->getStateManager().getGameState()->players[name]->setPosition({newX, newY});
                }
            } else if (type == net::Packet::ClientLeftPacket) {
                std::string name;
                last_packet >> name;

                if (Game::getInstance()->getStateManager().getGameState()->exists(name)) {
                    Game::getInstance()->getStateManager().getGameState()->players.erase(name);
                }
            } else if (type == net::Packet::ChunkDataPacket) {
                int ox, oy;
                last_packet >> ox >> oy;
                
                std::array<int,256> data{0};
                for (int i = 0; i < 256; i++) {
                    last_packet >> data[i];
                }

                Game::getInstance()->getStateManager().getGameState()->world.loadChunkFromData({ox, oy}, data);
            }

            // 1000Hz
            std::this_thread::sleep_for((std::chrono::milliseconds)1);
        }
    }
}

void ClientNetwork::send(sf::Packet& packet) {
    if (packet.getDataSize() > 0 && socket.send(packet) != sf::Socket::Status::Done) {
        cinfo("Failed to send packet");
        Game::getInstance()->failedPackets++;
    }
}

void ClientNetwork::run() {
    reception_thr = std::thread{&ClientNetwork::receivePackets, this, &socket};
}