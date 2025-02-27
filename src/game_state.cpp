#include "game_state.h"
#include "game.h"

GameState::GameState() : State("game", States::GameStateType), player(Game::getInstance()->getName()) {
    logger::log("game").info("Loaded game state!");
    world.start();
}

GameState::~GameState() {
    for (auto &[name, p] : players) {
        delete p;
    }

    players.clear();
}

void GameState::start() {
    players.insert({player.getDisplayname(), &player});
    view.setSize(Game::getInstance()->getWindow().getDefaultView().getSize());
    view.zoom(0.3);
}

void GameState::spawn(std::string const& name, sf::Vector2f pos) {
    ctrace("Inserted new player called " + name + " at [x:" + std::to_string(pos.x) + ", y:" + std::to_string(pos.y) + "]");
    players.insert({name, new RemotePlayer(name, pos)});
}

bool GameState::exists(std::string const& name) {
    return players.count(name) != 0;
}

void GameState::update(sf::Time dt, ClientNetwork& client, sf::Clock& tick) {
    view.setCenter(player.getPosition());
    world.update(dt);
    
    for (auto &[name, p] : players) {
        p->update(dt);
    }

    // send update pos packet x times a second:
    constexpr int tickrate = 100;
    //constexpr int spt = 1000/100;
    
    if (tick.getElapsedTime().asMilliseconds() >= 1000/tickrate) {
        if (player.isMoving()) {
            auto pos = player.getPosition();

            sf::Packet updatePos;
            updatePos << net::Packet::ClientMovementPacket << pos.x << pos.y;
            client.send(updatePos);
        }

        for (auto &[name, p] : players) {
            p->update_per_tick();
        }

        tick.restart();
    }
}

void GameState::draw(sf::RenderTarget& targ, sf::RenderStates states) const {
    states.transform *= getTransform();
    
    targ.draw(world, states);

    for (auto &[name, p] : players) {
        targ.draw(*p, states);
    }
} 