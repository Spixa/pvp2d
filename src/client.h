#pragma once

// sfml
#include <SFML/Network.hpp>

// std
#include <vector>
#include <string>
#include <memory>
#include <thread>
#include <mutex>

// arbalesto
#include "packets.h"
#include "logger.h"

#define ctrace(x) logger::log("client").trace(x)
#define cinfo(x) logger::log("client").info(x)
#define cwarn(x) logger::log("client").warn(x)
#define cerror(x) logger::log("client").error(x)

class ClientNetwork {
    sf::TcpSocket socket;
    sf::Packet last_packet;

    bool is_connected = false;
    std::thread reception_thr;
public:
    ClientNetwork();
    virtual ~ClientNetwork() {}
private:
    void receivePackets(sf::TcpSocket*);
public: // public functions
    void connect(std::string const& ip, unsigned short port);
    void send(sf::Packet& packet);
public: // public things:
    int last_ping = 7;
    sf::Clock pingTime;
    std::mutex pingTimeMutex;
public:
    void run();
};