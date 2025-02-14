#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class AnimatedTexture {
public:
    AnimatedTexture(std::string const& path, sf::Vector2u image_count, float switch_time);

    void update(int row, float dtSeconds);
public:
    sf::Texture& getTexture() { return texture; }
    sf::Vector2u getImageCount() const { return image_count; }
    sf::Vector2u getCurrentImage() const { return current_image; }
    sf::IntRect getAnimRect() { return anim_rect; }

    void setCurrentImage(sf::Vector2u const& at) { current_image = at; }
    void setCurrentImageX(unsigned int x) { current_image.x = x; }
    void setCurrentImageY(unsigned int y) { current_image.y = y; }

private:
    sf::Texture texture{};
    sf::Vector2u image_count, current_image;
    sf::IntRect anim_rect;

    float total_time, switch_time;
};

class Player;
class PlayerAnimation {
public:
    PlayerAnimation(float switch_time, Player* player_ptr);
    void update(int row, sf::Time dt, bool inverted);

    // from arbalesto::Serializable (yet to be implemented)
    // void fromPacket(sf::Packet)
    // sf::Packet toPacket()
private:
    AnimatedTexture hand, body, feet;
    Player* player_ptr;
};