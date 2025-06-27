#include "MainMenuState.hpp"
#include "Game.hpp"
#include "EmbeddedResources.hpp"
#include <iostream>

constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;

MainMenuState::MainMenuState() : mSelectedOption(0) {
    if (!mFont.openFromMemory(embedded_font_data, embedded_font_data_size)) {
        std::cout << "Error cargando fuente en MainMenu" << std::endl;
    }
    setupMenu();
}

void MainMenuState::setupMenu() {
    mMenuOptions.clear();
    //        "Configuraciones", 
    //    "Creditos",
    std::vector<std::string> menuTexts = {
        "Jugar",

        "Salir"
    };
    
    for (size_t i = 0; i < menuTexts.size(); i++) {
        sf::Text text(mFont, menuTexts[i], 36);
        text.setFillColor(i == mSelectedOption ? sf::Color::Yellow : sf::Color::White);
        
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setPosition(sf::Vector2f(
            (WINDOW_WIDTH - textBounds.size.x) / 2.0f,
            300 + i * 60
        ));
        
        mMenuOptions.push_back(text);
    }
}

void MainMenuState::handleEvent(const sf::Event& event, Game* game) {
    if (event.is<sf::Event::KeyPressed>()) {
        const sf::Event::KeyPressed* keyEvent = event.getIf<sf::Event::KeyPressed>();
        
        switch (keyEvent->code) {
            case sf::Keyboard::Key::Up:
                mSelectedOption = (mSelectedOption - 1 + mMenuOptions.size()) % mMenuOptions.size();
                updateSelection();
                break;
                
            case sf::Keyboard::Key::Down:
                mSelectedOption = (mSelectedOption + 1) % mMenuOptions.size();
                updateSelection();
                break;
                
            case sf::Keyboard::Key::Enter:
                switch (mSelectedOption) {
                    case 0: // Jugar
                        game->changeState(GameState::LoadState);
                        break;
                    case 1: // Configuraciones
                        //game->changeState(GameState::Config);
                        game->closeWindow();
                        break;
                    case 2: // Créditos
                        //game->changeState(GameState::Credits);
                        break;
                    case 3: // Salir
                        //game->closeWindow();
                        break;
                }
                break;
        }
    }
}

void MainMenuState::update(float deltaTime, Game* game) {
    // Animaciones del menú aquí
}

void MainMenuState::render(sf::RenderWindow& window, Game* game) {
        window.clear(sf::Color::Black);   
    // Título del juego
    sf::Text title(mFont, "Escape The Grid", 72);
    title.setFillColor(sf::Color::Cyan);
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setPosition({(WINDOW_WIDTH - titleBounds.size.x) / 2.0f, 100});
    window.draw(title);
    
    // Opciones del menú
    for (const auto& option : mMenuOptions) {
        window.draw(option);
    }
}

void MainMenuState::onEnter(Game* game) {
    mSelectedOption = 0;
    setupMenu();
}

void MainMenuState::updateSelection() {
    for (size_t i = 0; i < mMenuOptions.size(); i++) {
        mMenuOptions[i].setFillColor(i == mSelectedOption ? sf::Color::Yellow : sf::Color::White);
    }
}