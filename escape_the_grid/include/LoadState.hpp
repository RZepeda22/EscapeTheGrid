#pragma once

#include "GameStateBase.hpp"
#include <vector>

class LoadState : public GameStateBase {
private:
    sf::Font mFont;
    //std::vector<sf::Text> mMenuOptions;
    //int mSelectedOption;
        // Variables para file chooser (sin métodos adicionales)
    std::string mSelectedMapPath;
    bool mMapValid;
    sf::RectangleShape mSelectMapButton;
    sf::RectangleShape mLoadMapButton;
    sf::RectangleShape mBackButton;
    
public:
    LoadState();
    
    void handleEvent(const sf::Event& event, Game* game) override;
    void update(float deltaTime, Game* game) override;
    void render(sf::RenderWindow& window, Game* game) override;
    void onEnter(Game* game) override;


};