#pragma once

// std
#include <cmath>

// sfml
#include <SFML/Graphics.hpp>

// arbalesto
#include "chunk.h"

struct ChunkPos {
    bool operator==(ChunkPos const& other) const {
        return x == other.x && y == other.y;
    }
    
    ChunkPos(int x, int y) : x(x), y(y) {}
    ChunkPos(sf::Vector2i const& v) : x(v.x), y(v.y) {} 

    int x, y;
};

struct hash_fn {
    std::size_t operator()(ChunkPos const& node) const {
        std::size_t h1 = std::hash<int>()(node.x);
        std::size_t h2 = std::hash<int>()(node.y);

        return h1 ^ h2;
    }
};

class World : public sf::Drawable, public sf::Transformable {
public:
    World() : isometric_texture("../res/world/iso.png") {}

    void start();
    void update(sf::Time dt); 
    void loadChunkFromData(sf::Vector2i const& origin, std::array<int, 256> const& data);
protected:
    void draw(sf::RenderTarget&, sf::RenderStates) const override;
private:
    sf::Texture isometric_texture;
    WorldManipulator manip{};
    std::unordered_map<ChunkPos,Chunk,hash_fn> loaded_chunks{};
    int selected = 0;
};