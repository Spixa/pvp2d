#pragma once
#include "state.h"
#include "player.h"

class GameState : public State {
public:
    GameState();
    virtual ~GameState();
public:
    void start() override;
    void update(sf::Time dt, ClientNetwork& client, sf::Clock& tick) override;
    void spawn(std::string const& name, sf::Vector2f spawn_pos);    
    bool exists(std::string const& name);
protected:
    void draw(sf::RenderTarget&, sf::RenderStates) const override;
private:
    ControllingPlayer player;
public:
    std::unordered_map<std::string, Player*> players{};
};