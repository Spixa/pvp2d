#pragma once

// std
#include <cmath>

// sfml
#include <SFML/Graphics.hpp>

// arbalesto
#include "chunk.h"

class World : public sf::Drawable, public sf::Transformable {
public:
    World() : isometric_texture("../res/world/iso.png") {}

    void start();
    void update(sf::Time dt); 
protected:
    void draw(sf::RenderTarget&, sf::RenderStates) const override;
private:
    sf::Texture isometric_texture;
    WorldManipulator manip{};
    std::vector<Chunk> loaded_chunks{};
    int selected = 0;
};