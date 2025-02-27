#include "world.h"
#include "game.h"

void World::start() {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            loaded_chunks.push_back(Chunk(&isometric_texture, x, y, ChunkBuilderMode::GrassLand));
        }
    }

    for (auto& x: loaded_chunks) {
        x.generate(ChunkBuilderMode::GrassLand);
        x.update();
    }
}

void World::update(sf::Time dt) {
    for (auto& x: loaded_chunks) {
       //x.update();
       int mouse_tile = manip.getTileIndexFromMouse(&Game::getInstance()->getWindow(), &x);
    }
}

void World::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();

    for (auto& x : loaded_chunks) {
        target.draw(x, states);
    }

    manip.drawSelection(target, states);
}