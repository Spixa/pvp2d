#include "chunk.h"

#include "game.h"

int WorldManipulator::getTileIndexFromMouse(sf::RenderWindow* window, Chunk* chunk) {
    auto mouse = window->mapPixelToCoords(sf::Mouse::getPosition(*window), Game::getInstance()->getStateManager().getCurrentView());
    
    sf::Vector2i cell{floor_div(mouse.x, chunk->get_tile_size().x), floor_div(mouse.y, chunk->get_tile_size().y)};
    sf::Vector2u offset{floor_mod(mouse.x, chunk->get_tile_size().x), floor_mod(mouse.y, chunk->get_tile_size().y)};
    sf::Color color{};

    if (offset.x >= 0 && offset.y >= 0) { color = cheat.getPixel(offset);  }

    sf::Vector2i selected =
    {
        (cell.y - chunk->get_origin().y) + (cell.x - chunk->get_origin().x),
        (cell.y - chunk->get_origin().y) - (cell.x - chunk->get_origin().x)  
    };

    if (color == sf::Color::Red) selected += {-1, +0};
    if (color == sf::Color::Blue) selected += {+0, -1};
    if (color == sf::Color::Green) selected += {+0, +1};
    if (color == sf::Color::Yellow) selected += {+1, +0};

    int idx = selected.y * chunk->get_chunk_size().x + selected.x;

    auto to_screen = [&](int x, int y) -> sf::Vector2i
    {
      return sf::Vector2i
      {
        (chunk->get_origin().x * chunk->get_tile_size().x) + int( floor( ((x - y) * (chunk->get_tile_size().x)) / 2)),
        (chunk->get_origin().y * chunk->get_tile_size().y) + int( floor( ((x + y) * (chunk->get_tile_size().y)) / 2))
      };
    };

    if (selected.x < chunk->get_chunk_size().x && selected.y < chunk->get_chunk_size().y && selected.x >=0 && selected.y >= 0) {
        // within bounds
        selection.setPosition(sf::Vector2f{to_screen(selected.x, selected.y)});
        return idx;
    }

    return -1;
}

int WorldManipulator::getTileStandingOn(Chunk* chunk, sf::Vector2f const& position) {    
    sf::Vector2i cell{floor_div(position.x, chunk->get_tile_size().x), floor_div(position.y, chunk->get_tile_size().y)};
    sf::Vector2u offset{floor_mod(position.x, chunk->get_tile_size().x), floor_mod(position.y, chunk->get_tile_size().y)};
    sf::Color color{};

    if (offset.x >= 0 && offset.y >= 0) { color = cheat.getPixel(offset);  }

    sf::Vector2i selected =
    {
        (cell.y - chunk->get_origin().y) + (cell.x - chunk->get_origin().x),
        (cell.y - chunk->get_origin().y) - (cell.x - chunk->get_origin().x)  
    };

    if (color == sf::Color::Red) selected += {-1, +0};
    if (color == sf::Color::Blue) selected += {+0, -1};
    if (color == sf::Color::Green) selected += {+0, +1};
    if (color == sf::Color::Yellow) selected += {+1, +0};

    int idx = selected.y * chunk->get_chunk_size().x + selected.x;

    auto to_screen = [&](int x, int y) -> sf::Vector2i
    {
      return sf::Vector2i
      {
        (chunk->get_origin().x * chunk->get_tile_size().x) + int( floor( ((x - y) * (chunk->get_tile_size().x)) / 2)),
        (chunk->get_origin().y * chunk->get_tile_size().y) + int( floor( ((x + y) * (chunk->get_tile_size().y)) / 2))
      };
    };

    if (selected.x < chunk->get_chunk_size().x && selected.y < chunk->get_chunk_size().y && selected.x >=0 && selected.y >= 0) {
        // within bounds
        return idx;
    }

    return -1;
}

void WorldManipulator::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();

    target.draw(selection_sprite, states);
    target.draw(selection, states);
}
