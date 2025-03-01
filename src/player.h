#pragma once
#include "hbox_entity.h"
#include "animation.h"

static sf::Font font{"../res//fonts/main.ttf"};
static sf::Texture nothing{};

struct PlayerSprites {
    sf::Sprite* body;
    sf::Sprite* hand;
    sf::Sprite* feet;
};

// very boilerplate rn
class Player : public HitboxEntity {
public: 
    Player(std::string const& display_name = "", sf::Vector2f const& spawn = {0.f, 0.f}) : display_name(display_name), displayNameText(font), anim(0.1, this), feet(nothing), hand(nothing), body(nothing) {
        setPosition(spawn);
        setHitBox(sf::FloatRect({0, 0}, {32, 32}));
        setScale({0.5, 0.5});
        
        displayNameText.setPosition({0, -15.f});
        displayNameText.setStyle(sf::Text::Style::Regular);
        displayNameText.setCharacterSize(10);
        displayNameText.setString(display_name);
    }

    virtual ~Player() {};

    void update(sf::Time dt) {
        update_derived(dt);

        move(velo);
        if (moving) {
            anim.update(1, dt, false);
        } else {
            anim.update(0, dt, false);
        }
    };

    std::string const& getDisplayname() const { return display_name; }

    virtual void update_derived(sf::Time dt) = 0;
    virtual void update_per_tick() = 0;

    PlayerSprites getSprites() { return {&body, &hand, &feet}; }

    bool isMoving() {
        return moving;
    }
protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        states.transform *= getTransform();

        target.draw(feet, states);
        target.draw(body, states);
        target.draw(hand, states);

        target.draw(displayNameText, states);
    }
protected: // online properties:
    std::string display_name;
    bool moving;
    sf::Vector2f velo;
private:
    sf::Sprite body, hand, feet;
    bool inverted{false};
    PlayerAnimation anim;
protected:
    sf::Text displayNameText;
    sf::Font displayNameFont;
};

class RemotePlayer : public Player {
public:
    RemotePlayer(std::string const& name, sf::Vector2f const& spawn) : Player(name, spawn) {}

    void update_derived(sf::Time dt) override {}
    void update_per_tick() override;
private:
    sf::Vector2f old_pos;
};

class ControllingPlayer : public Player {
public:
    ControllingPlayer(std::string const& disp) : Player(disp) {
        displayNameText.setFillColor(sf::Color::Blue);
    }
    sf::View playerView;
    void update_derived(sf::Time delta) override;
    void update_per_tick() override {}
};