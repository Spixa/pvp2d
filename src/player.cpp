#include "player.h"

#include "game.h"

void ControllingPlayer::update_derived(sf::Time delta) {
    float dt = delta.asSeconds();
    

    playerView.setSize({float(Game::getInstance()->getWindow().getSize().x), float(Game::getInstance()->getWindow().getSize().y)});
    playerView.setCenter(this->getPosition());

    constexpr float speed = 512.f;

    if (Game::getInstance()->getWindow().hasFocus())
        velo = sf::Vector2f{
            float(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) - sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)),
            float(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) - sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        };

    if (velo != sf::Vector2f()) {
        velo = velo.normalized();
        velo.x = velo.x * dt * speed;
        velo.y = velo.y * dt * speed; 
    }


    if (velo.x != 0 || velo.y != 0) {
        moving = true;
    } else {
        moving = false;
    }

    // Game::getInstance()->getWindow().setView(playerView);
}

void RemotePlayer::update_per_tick() {
    sf::Vector2f new_pos = getPosition();

    if (old_pos == new_pos) {
        moving = false;
    } else {
        moving = true;
    }

    // always 1 tick behind
    old_pos = new_pos;
}
