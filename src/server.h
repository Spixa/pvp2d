#pragma once

// SFML
#include <SFML/Network.hpp>

// std
#include <vector>
#include <string>
#include <memory>
#include <regex>

// utils
#include "logger.h"
#include "util.h"

// server stuff
#include "server/client.h"
#include "packets.h"

// macros
#define strace(x) logger::log("server").trace(x)
#define sinfo(x) logger::log("server").info(x)
#define swarn(x) logger::log("server").warn(x)
#define serror(x) logger::log("server").error(x)

class Server : NonCopyable {
private:
    unsigned short listen_port;
    sf::TcpListener listener;

    std::vector<server::Client*> clients;
    std::regex alphanum{"^[a-zA-Z0-9_]*$"};
public:
    Server(unsigned short port);
    ~Server();

    void run();
private:
    void connectClients(std::vector<server::Client*>*);
    void disconnectClient(server::Client*, size_t);

    void receivePackets(server::Client*, size_t);
    void broadcastPacket(sf::Packet&);
    void managePackets();
};