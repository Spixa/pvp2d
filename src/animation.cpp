#include "animation.h"
#include "player.h"

AnimatedTexture::AnimatedTexture(std::string const& path, sf::Vector2u image_count, float switch_time) : image_count(image_count), switch_time(switch_time) {
    bool yes_discard = texture.loadFromFile(path);
    total_time = 0.f;

    anim_rect.size.x = texture.getSize().x / float(image_count.x);
    anim_rect.size.y = texture.getSize().y / float(image_count.y);
}

void AnimatedTexture::update(int row, float dtSeconds) { 
    current_image.y = row;
    total_time += dtSeconds;

    if (total_time >= switch_time) {
        total_time -= switch_time;
        current_image.x++;

        if (current_image.x >= image_count.x)
            current_image.x = 0;
    }

    anim_rect.position.x = current_image.x * anim_rect.size.x;
    anim_rect.position.y = current_image.y * anim_rect.size.y;
}

PlayerAnimation::PlayerAnimation(float switch_time, Player* player_ptr) :
    body("../res/player/body.png", {6, 8}, switch_time),
    hand("../res/player/hand.png", {6, 8}, switch_time),
    feet("../res/player/feet.png", {6, 8}, switch_time),
    player_ptr(player_ptr)
{
    body.setCurrentImageX(0);
    hand.setCurrentImageX(0);
    
    player_ptr->getSprites().body->setTexture(body.getTexture());
    player_ptr->getSprites().hand->setTexture(hand.getTexture());
    player_ptr->getSprites().feet->setTexture(feet.getTexture());
}

void PlayerAnimation::update(int row, sf::Time deltaTime, bool inverted) {
    float dt = deltaTime.asSeconds();

    hand.update(row, dt);
    body.update(row, dt);
    feet.update(row, dt);

    {
        auto b = body.getAnimRect();
        auto h = hand.getAnimRect();
        auto f = feet.getAnimRect();

        player_ptr->getSprites().body->setTextureRect(b);
        player_ptr->getSprites().hand->setTextureRect(h);
        player_ptr->getSprites().feet->setTextureRect(f);
    }
}