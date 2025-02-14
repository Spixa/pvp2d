#include "state_man.h"

StateManager::StateManager() {

}

void StateManager::init() {
    State::Ptr game = std::make_shared<GameState>();
    states.push_back(std::move(game));
    
    for (auto x : states) {
        x->start();
    }
}

void StateManager::update(sf::Time dt, ClientNetwork& client) {
    if (states[selected]) {
        states[selected]->update(dt, client, tick);
    }
}

void StateManager::render(sf::RenderWindow& window) {
    if (states[selected]) {
        window.draw(*states[selected].get());
    }
}
