#include "world.h"
#include "game.h"

void World::start() {

}

void World::loadChunkFromData(sf::Vector2i const& origin, std::array<int, 256> const& data) {
    if (loaded_chunks.count(ChunkPos(origin)) != 0) {
        logger::log("world").warn("Chunk already exists");
        return;
    }

    loaded_chunks.insert({ChunkPos(origin.x, origin.y), Chunk(&isometric_texture, origin.x, origin.y)});

    const ChunkPos pos {origin};

    loaded_chunks[pos].load(data);
    loaded_chunks[pos].update();
}

void World::update(sf::Time dt) {
    for (auto& [p, x]: loaded_chunks) {
       //x.update();
       int mouse_tile = manip.getTileIndexFromMouse(&Game::getInstance()->getWindow(), &x);
    }
}

void World::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();

    for (auto& [p, x] : loaded_chunks) {
        target.draw(x, states);
    }

    manip.drawSelection(target, states);
}