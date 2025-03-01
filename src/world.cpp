#include "world.h"
#include "game.h"

void World::start() {

}

void World::loadChunkFromData(sf::Vector2i const& origin, std::array<int, 256> const& data) {
    if (loaded_chunks.count(ChunkPos(origin)) == 0) {
        loaded_chunks.insert({ChunkPos(origin.x, origin.y), Chunk(&isometric_texture, origin.x, origin.y)});
    } else {
        // remove this for updating already loaded chunks
        return;
    }

    const ChunkPos pos {origin};

    loaded_chunks[pos].load(data);
    loaded_chunks[pos].update();
}

void World::update(sf::Time dt) {
    for (auto& [p, x]: loaded_chunks) {
        //x.update();
        int mouse_tile = x.getTileAt(manip.getTileIndexFromMouse(&Game::getInstance()->getWindow(), &x));
        
        switch (mouse_tile) {
            case 0: Game::getInstance()->lookingAt = "Nothing"; break;
            case 1: Game::getInstance()->lookingAt = "Grass"; break;
            case 2: Game::getInstance()->lookingAt = "Tree"; break;
            case 3: Game::getInstance()->lookingAt = "Dead Tree"; break;
            case 4: Game::getInstance()->lookingAt = "Sand"; break;
            case 5: Game::getInstance()->lookingAt = "Water"; break;
            case 6: Game::getInstance()->lookingAt = "Funky Tree"; break;
        }

        float length = 0;
        {
            auto p1 = Game::getInstance()->getWindow().mapPixelToCoords(sf::Mouse::getPosition(Game::getInstance()->getWindow()), Game::getInstance()->getStateManager().getCurrentView());
            auto p2 = Game::getInstance()->getStateManager().getGameState()->players[
                Game::getInstance()->getName()
            ]->getPosition();
        
            length = (p2 - p1).length();
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) && 
            length <= 60 &&
            x.setTile(manip.getTileIndexFromMouse(&Game::getInstance()->getWindow(), &x), 5)) {
            x.update();
        }
    }
}

void World::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();

    for (auto& [p, x] : loaded_chunks) {
        target.draw(x, states);
    }

    target.draw(manip, states);
}