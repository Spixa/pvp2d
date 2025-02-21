#pragma once

// sfml
#include <SFML/Graphics.hpp>

// std
#include <vector>
#include <memory>
#include <string>

// arbalesto
#include "client.h"

enum class States {
    GameStateType,
    MenuStateType,
    AboutStateType
};

class State : public sf::Drawable, public sf::Transformable {
public:
    State(std::string const& name, States type) : name(name), type(type) {}
    virtual ~State() {}

    using Ptr = std::shared_ptr<State>;
    sf::View view{};
public:
    States getType() const { return type; }
    std::string getName() const { return name; }

public:
    virtual void start() = 0;
    virtual void update(sf::Time dt, ClientNetwork& client, sf::Clock& tick) = 0;
protected:
    virtual void draw(sf::RenderTarget&, sf::RenderStates) const override = 0; // pure virtual
protected:
    std::vector<sf::Drawable> drawables;
private:
    std::string name;
    States type;
};