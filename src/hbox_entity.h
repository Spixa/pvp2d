#pragma once
#include <SFML/Graphics.hpp>

class HitboxEntity : public sf::Drawable, public sf::Transformable {
public:
    void setHitBox(const sf::FloatRect& hbox) { hitbox = hbox; }

    sf::FloatRect getGlobalHbox() const {
        return getTransform().transformRect(hitbox);
    }
protected:
    virtual void draw(sf::RenderTarget&, sf::RenderStates) const override = 0;
private:
    sf::FloatRect hitbox;
};