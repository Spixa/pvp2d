#pragma once
#include "state.h"
#include "game_state.h"

class ClientNetwork;
class StateManager {
public:
    StateManager();

    void init();
    void update(sf::Time dt, ClientNetwork& client);
    void render(sf::RenderWindow& window);

    std::shared_ptr<GameState> getGameState() {
        // downcast to GameState
        return std::dynamic_pointer_cast<GameState>(states[0]);
    }

    sf::View& getCurrentView() {
        return states[selected]->view;
    }
protected:
    sf::Clock tick{};
private:
    std::vector<State::Ptr> states;
    size_t selected{0};
};