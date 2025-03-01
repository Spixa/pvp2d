#pragma once

// std
#include <cmath>
#include <string>
#include <cstdlib>

// sfml
#include <SFML/Graphics.hpp>

// arbalesto
#include "logger.h"

enum class ChunkBuilderMode {
    GrassLand,
    None
};

class Chunk: public sf::Drawable, public sf::Transformable {
    public:
        Chunk() {} // risky, but needed for the unordered map

        Chunk(sf::Texture* text, int x, int y, ChunkBuilderMode const& mode = ChunkBuilderMode::None) : texture(text) {
            data = new int[size.x*size.y]{0};
    
            sf::Vector2i screen;
            screen.x = int(floor( ((x - y)) * 16/2));
            screen.y = int(floor( ((x + y)) * 16/2));
    
            std::stringstream pos;
            pos << "[" << screen.x << ", " << screen.y << "]";
            logger::log("world").trace("Created chunk at " + pos.str());
    
            origin.x = screen.x;
            origin.y = screen.y;
            
            for (int i = 0; i < 256; i++) {
                switch (mode) {
                    case ChunkBuilderMode::GrassLand: {
                        data[i] = 1;
                    } break;
                    case ChunkBuilderMode::None: {
                        data[i] = 0;
                    } break;
                }
                isometric_tiles[i] = new sf::Sprite(*text);   
                //isometric_tiles[i]->setTexture(*text);
                //isometric_tiles[i].setSize({float(get_tile_size().x), float(get_tile_size().y)});   
            }
        }

        void load(std::array<int, 256> const& data) {
            int c = 0;
            for (auto a : data) {
                this->data[c] = a;
                c++;
            }
        }
    
        ~Chunk() {
            // delete data;
        }
    
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
            states.transform *= getTransform();

            for (auto&x : isometric_tiles) {
                target.draw(*x, states);
            }
        }
    
        bool setTile(int at, int with) {
            if (at >= 0 && at < 256) {
                data[at] = with; 
                return true;
            }
            return false;
        }
    
        int getTileAt(int at) {
            if (at >= 0 && at < 256) {
                return data[at];
            }
            return -1;
        }
    
        void update() {
            auto to_screen = [&](int x, int y) -> sf::Vector2i
            {
                return sf::Vector2i
                {
                    (origin.x * tile_size.x) + int( floor( ((x - y) * (tile_size.x)) / 2)),
                    (origin.y * tile_size.y) + int( floor( ((x + y) * (tile_size.y)) / 2))
                };
            };
    
            for (int y = 0; y < size.y; y++) {
                for (int x = 0; x < size.x; x++) {
                    int idx = y * size.x + x;
                    
                    auto world_coords = to_screen(x, y);
    
                    // Can be modified within the switch statement:
                    sf::IntRect selection{};
                    int layer = 0;
    
                    switch (data[idx]) {
                        case 0: { // void
                            selection = sf::IntRect({1*tile_size.x, 0}, tile_size);
                        } break;
                        case 1: { // grass
                            selection = sf::IntRect({2*tile_size.x, 0}, tile_size);
                        } break;
                        case 2: { // tree
                            selection = sf::IntRect({0, tile_size.y}, {tile_size.x, 2*tile_size.y}); 
                            layer = 1;
                        } break;
                        case 3: { // dead tree
                            selection = sf::IntRect({1 * tile_size.x, tile_size.y}, {tile_size.x, 2*tile_size.y});
                            layer = 1;
                        } break;
                        case 4: { // sand
                            selection = sf::IntRect({2 * tile_size.x, tile_size.y}, {tile_size.x, tile_size.y * 2});
                            layer = 1;
                        } break;
                        case 5: { // water
                            selection = sf::IntRect({3 * tile_size.x, tile_size.y}, {tile_size.x, tile_size.y * 2});
                            layer = 1;
                        } break;
                        case 6: { // funky tree
                            selection = sf::IntRect({5 * tile_size.x, 0}, {tile_size.x, tile_size.y * 4});
                            layer = 3;
                        } break;
                    }
    
                    isometric_tiles[idx]->setTextureRect(selection);
                    isometric_tiles[idx]->setPosition(sf::Vector2f{float(world_coords.x), float(world_coords.y - layer * tile_size.y)});
                }
            }
        }
    
        sf::Vector2i const& get_origin() const {
            return origin;
        }
    
        sf::Vector2i const& get_tile_size() const {
            return tile_size;
        }
    
        sf::Vector2i const& get_chunk_size() const {
            return size;
        }
    
        sf::Sprite** get_tiles() {
            return isometric_tiles;
        }
    private:
        sf::Texture* texture;
        int* data;
        sf::Vector2i origin;
        sf::Vector2i size{16, 16};
        sf::Vector2i tile_size{40, 20};
        bool first_shot = true;
    
    private:
        sf::Sprite* isometric_tiles[256];
    };
    
    // https://stackoverflow.com/questions/62916620/c-equivalent-of-javas-math-floormod
    inline unsigned int floor_mod(int x, int y) {
        x %= y;
      
        if (x < 0) {
            x += y;
        }
      
        return x;
      }
      
      // https://stackoverflow.com/questions/2622441/c-integer-floor-function
      inline int floor_div (int num, int den)
      {
        if (0 < (num^den))
          return num/den;
        else
          {
            ldiv_t res = ldiv(num,den);
            return (res.rem)? res.quot-1 
                            : res.quot;
          }
      }
    
    class WorldManipulator : public sf::Drawable, public sf::Transformable  {
    public:
        WorldManipulator() : cheat("../res/world/cheat.png"), selection_img("../res/world/highlight.png"), selection_sprite(selection_img) {
            selection.setFillColor(sf::Color::Red);
            selection.setSize({40, 20});
            selection.setTexture(&selection_img);
        }
    
        int getTileIndexFromMouse(sf::RenderWindow* window, Chunk* chunk);
        int getTileStandingOn(Chunk* chunk, sf::Vector2f const& position);

        sf::Vector2i to_screen(Chunk* chunk, int x, int y) {
          return sf::Vector2i
          {
            (chunk->get_origin().x * chunk->get_tile_size().x) + int( floor( ((x - y) * (chunk->get_tile_size().x)) / 2)),
            (chunk->get_origin().y * chunk->get_tile_size().y) + int( floor( ((x + y) * (chunk->get_tile_size().y)) / 2))
          };
        }
    
        auto chunk_world_pos(Chunk* chunk) {
          return chunk->get_tiles()[0]->getPosition();
        }

        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    private:
        sf::Image cheat;
    private:
        sf::Sprite selection_sprite;
    public:
        sf::RectangleShape selection;
    private:
        sf::Texture selection_img;
    
        bool too_far;
    };