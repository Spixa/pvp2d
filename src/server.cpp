#include "server.h"
#include <sstream>
#include <cmath>
#include <thread>

Server::Server(unsigned short port) : listen_port(port) {
    sinfo("Starting server on port " + std::to_string(port));

    if (listener.listen(listen_port) != sf::Socket::Status::Done) {
        sinfo("Cannot listen on port " + std::to_string(listen_port));
    }
}

Server::~Server() {
    sinfo("Stopping server...");

    for (auto x : clients) {
        x->socket()->disconnect();

        delete x;
    }

    clients.clear();
}

void Server::connectClients(std::vector<server::Client*> *clients) {
    while (true) {
        sf::TcpSocket* new_client = new sf::TcpSocket();

        if (listener.accept(*new_client) == sf::Socket::Status::Done) {
            // new client joined
            new_client->setBlocking(false);
            clients->push_back(new server::Client(new_client));

            std::stringstream log;
            log << "Added client " << new_client->getRemoteAddress().value() << ":" << new_client->getRemotePort() << " on slot " << clients->size();
            sinfo(log.str());
        } else {
            serror("Server has run into a listener error, restart the server");
            delete new_client;

            break;
        }
    }
}

void Server::disconnectClient(server::Client* client_ptr, size_t pos) {
    std::stringstream log;
    log << "Client " << client_ptr->socket()->getRemoteAddress().value() << ":" << client_ptr->socket()->getRemotePort() << " disconnected";
    sinfo(log.str());

    std::string name = client_ptr->getName().value();

    client_ptr->socket()->disconnect();
    delete client_ptr;

    clients.erase(clients.begin() + pos);

    {
        sf::Packet leave;
        leave << net::Packet::ClientLeftPacket << name;
        broadcastPacket(leave);
    }
}

void Server::broadcastPacket(sf::Packet& packet) {
    for (size_t iter = 0; iter < clients.size(); iter++) {
        sf::TcpSocket* sock = clients[iter]->socket();

        if (sock->send(packet) != sf::Socket::Status::Done) {
            serror("Could not send packet on broadcast");
        }
    }
}

void Server::receivePackets(server::Client* client, size_t iter) {
    sf::Packet packet;
    sf::TcpSocket* sock = client->socket();

    if (sock->receive(packet) == sf::Socket::Status::Disconnected) {
        disconnectClient(client, iter);
    }

    if (packet.getDataSize() > 0) {
        std::string name = client->getName().value_or("iter=" + std::to_string(iter));
        net::Packet type;
        packet >> type;

        using namespace net;
        switch (type) {
            case Packet::PingPacket: {
                sinfo("Received ping from client \"" +  name + "\"");

                sf::Packet reply;
                reply << Packet::PingPacket;
                
                client->send(reply);
                
                srand(5);
                for (int j = -1; j < 1; j++) {
                    for (int k = -1; k < 1; k++) {
                        sf::Packet chunk;
                        chunk << Packet::ChunkDataPacket;
                        chunk << int(j) << int(k);

                        for (int i = 0; i < 256; i++) {
                            int tree = rand() % 20;
                            int dead = rand() % 5;
                            int f = 0;

                            if (tree == 0) {
                                if (dead == 0) {
                                    f = 3;
                                } else {
                                    f = 2;
                                }
                            } else {
                                f = 1;
                            }

                            chunk << int(f);
                        }

                        client->send(chunk);
                    }
                }

                break;
            }
            case Packet::NickPacket: {
                sinfo("Attempting to nick person on iter=" + std::to_string(iter));

                std::string name;
                packet >> name;

                if (!std::regex_match(name, alphanum)) {
                    // ADD KICK REASON LATER
                    disconnectClient(client, iter);
                    return;
                }

                for (auto x : clients) {
                    if (x->getName().value_or("") == name) {
                        // ADD KICK REASON LATER
                        disconnectClient(client, iter);
                        return;
                    }
                }

                if (!client->setName(name)) {
                    sinfo("Client was already nicked and tried nicking!");
                }

                {
                    sf::Packet list;
                    list << net::Packet::UpdatePlayerListPacket;

                    client->setPosition({0.f, 0.f});

                    list << int(clients.size());

                    for (auto x : clients) {
                        list << x->getName().value() << x->getPosition().x << x->getPosition().y;
                    }

                    broadcastPacket(list);
                }

                break;
            }
            case Packet::ClientMovementPacket: {
                float newX, newY;
                float oldX, oldY;
                float dX, dY;

                oldX = client->getPosition().x;
                oldY = client->getPosition().y;

                packet >> newX >> newY;

                dX = newX - oldX;
                dY = newY - oldY;


                sf::Vector2f displacement(dX, dY);
                if (displacement != sf::Vector2f()) {
                    if (displacement.length() > 14.2f) {
                        swarn(client->getName().value() + " might be cheating!");
                    }
                }
                client->setPosition({newX, newY});

                strace(name + " moved to (x:" + std::to_string(newX) + ", y:" + std::to_string(newY) + ") [dx: " + std::to_string(dX) + ", dy:" + std::to_string(dY) + "]");

                sf::Packet updatePos;
                updatePos << net::Packet::UpdatePositionPacket << client->getName().value() << client->getPosition().x << client->getPosition().y;

                broadcastPacket(updatePos);

                break;
            }
            default: {
                sinfo("Received an illegal packet from a client");
            }
        }
    }
}

void Server::managePackets() {
    while (true) {
        for (size_t iter = 0; iter < clients.size(); iter++) {
            receivePackets(clients[iter], iter);
        }

        // world.update();
        
        // 1000Hz server
        std::this_thread::sleep_for((std::chrono::milliseconds)1);
    }
}

void Server::run() {
    sinfo("(connection thread) thread launched");
    std::thread connection_thr{&Server::connectClients, this, &clients};

    sinfo("(reception thread) thread launched");
    managePackets();
}