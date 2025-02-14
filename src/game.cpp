#include "game.h"
#include <SFML/Graphics.hpp>

Game* Game::instance_ = nullptr;

Game* Game::getInstance() {
    if (!instance_) {
        logger::log("game").info("Game::getInstance() called when game wasn't initalized, creating game");
        instance_ = new Game();
    }
    return instance_;
}

Game::Game()
:   window_(sf::VideoMode({1280, 720}), "Arbalesto v1", sf::Style::Close | sf::Style::Resize), client(), general_font("../res/fonts/main.ttf"), server_stats(general_font), state_manager() {
    server_stats.setCharacterSize(15);
}

void Game::run(std::string const& nickname, std::string const& ip, unsigned short port) {
    sf::Clock clock{};
    logger::log("game").trace("name=" + nickname + " ip=" + ip);
    name = nickname;
    client.connect(ip, port);
    client.run(); // Note: function creates new thread (client_reception)

    state_manager.init();

    //window_.setFramerateLimit(256);
    {
        // block reserved for server join activites
        sf::Packet nick;
        nick << net::Packet::NickPacket << nickname;

        client.send(nick);
    }

    try {
        while (window_.isOpen()) {
            sf::Time elapsedTime = clock.restart();

            processEvents(elapsedTime);
            update(elapsedTime);
            render();
        }
    } catch (std::runtime_error const& e) {
        logger::log("game").error("Arbalesto ran into an error: " + std::string(e.what()));
    }
}

void Game::processEvents(sf::Time elasped) {
    while (const std::optional event = window_.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            logger::log("game").info("Goodbye!");
            window_.close();
            exit(0);
        }
    }
}

void Game::update_ping(sf::Time elapsed) {
    if (updatePing.getElapsedTime().asSeconds() >= 5) {
        std::lock_guard<std::mutex> guard(client.pingTimeMutex);
        client.pingTime.restart();

        sf::Packet pack;
        pack << net::Packet::PingPacket;
        client.send(pack);

        updatePing.restart();
    }
    
    server_stats.setString("Connected: " + std::to_string(client.last_ping) + "ms\n" + std::to_string( int(1/elapsed.asSeconds())) + " fps\n" + "Failed packets: " + std::to_string(failedPackets));
}

void Game::update(sf::Time elapsed) {
    update_ping(elapsed);

    state_manager.update(elapsed, client);
}

void Game::render() {
    window_.clear();

    window_.draw(server_stats);
    state_manager.render(window_);

    window_.display();
}