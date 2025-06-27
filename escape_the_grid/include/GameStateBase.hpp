#pragma once

#include <SFML/Graphics.hpp>

class Game; // Forward declaration

class GameStateBase {
public:
    virtual ~GameStateBase() = default;
    
    virtual void handleEvent(const sf::Event& event, Game* game) = 0;
    virtual void update(float deltaTime, Game* game) = 0;
    virtual void render(sf::RenderWindow& window, Game* game) = 0;
    virtual void onEnter(Game* game) {}
    virtual void onExit(Game* game) {}
};
