#pragma once
#include "hbox_entity.h"

static sf::Font font{"../res//fonts/main.ttf"};

// very boilerplate rn
class Player : public HitboxEntity {
public: 
    Player(std::string const& display_name = "", sf::Vector2f const& spawn = {500.f, 500.f}) : display_name(display_name), displayNameText(font) {
        setPosition(spawn);
        setHitBox(sf::FloatRect({0, 0}, {32, 32}));
        
        displayNameText.setPosition({5.f, -25.f});
        displayNameText.setCharacterSize(12);
        displayNameText.setString(display_name);
        placeholder.setSize({32, 32});

        placeholder.setFillColor(sf::Color::Red);
        placeholder.setOutlineColor(sf::Color::Black);
        placeholder.setOutlineThickness(1);
    }

    void update(sf::Time dt) {
        update_derived(dt);

        move(velo);
        if (moving) {
            placeholder.setFillColor(sf::Color::Blue);
        } else {
            placeholder.setFillColor(sf::Color::Red);
        }
    };

    std::string const& getDisplayname() const { return display_name; }

    virtual void update_derived(sf::Time dt) = 0;
    virtual void update_per_tick() = 0;

    bool isMoving() {
        return moving;
    }
protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        states.transform *= getTransform();

        target.draw(displayNameText, states);
        target.draw(placeholder, states);
    }
protected: // online properties:
    std::string display_name;
    bool moving;
    sf::Vector2f velo;
protected:
    sf::Text displayNameText;
    sf::Font displayNameFont;
    sf::RectangleShape placeholder{};
};

class RemotePlayer : public Player {
public:
    RemotePlayer(std::string const& name, sf::Vector2f const& spawn) : Player(name, spawn) {
        placeholder.setFillColor(sf::Color::Blue);
    }

    void update_derived(sf::Time dt) override {}
    void update_per_tick() override;
private:
    sf::Vector2f old_pos;
};

class ControllingPlayer : public Player {
public:
    ControllingPlayer(std::string const& disp) : Player(disp) {
        displayNameText.setFillColor(sf::Color::Green);
    }
    sf::View playerView;
    void update_derived(sf::Time delta) override;
    void update_per_tick() override {}
};