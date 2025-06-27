#pragma once

#include "GameStateBase.hpp"
#include <vector>

class MainMenuState : public GameStateBase {
private:
    sf::Font mFont;
    std::vector<sf::Text> mMenuOptions;
    int mSelectedOption;
    
public:
    MainMenuState();
    
    void handleEvent(const sf::Event& event, Game* game) override;
    void update(float deltaTime, Game* game) override;
    void render(sf::RenderWindow& window, Game* game) override;
    void onEnter(Game* game) override;
    
private:
    void setupMenu();
    void updateSelection();
};