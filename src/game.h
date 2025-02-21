#pragma once

// arbalesto
#include "logger.h"
#include "util.h"
#include "client.h"
#include "state_man.h"

// sfml
#include <SFML/Graphics.hpp>


class Game : private NonCopyable {
public:
    void run(std::string const& name, std::string const& ip, unsigned short port);
    static Game* getInstance();
    StateManager& getStateManager() { return state_manager; }

    int failedPackets{0};
    sf::RenderWindow& getWindow() { return window_; }
    std::string const& getName() const { return name; }
public:
    sf::Font general_font;
private:
    Game();
    ~Game();
private:
    void processEvents(sf::Time elapsed);
    void update(sf::Time elapsed);
    void render();
private:
    // updates
    void update_ping(sf::Time);
private:
    sf::Clock updatePing{};
private:
    static Game* instance_;
    sf::RenderWindow window_;
    bool focused = false;
private:
    // networking
    ClientNetwork client;
    std::string name;
private:
    // drawables
    StateManager state_manager;
    sf::View ui_view;
    sf::Text server_stats;

};