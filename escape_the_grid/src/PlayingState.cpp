#include "PlayingState.hpp"
#include "Game.hpp"
#include "DrawMap.hpp"
#include "EmbeddedResources.hpp"
#include <iostream>
#include <cmath>
#include "Config.hpp"

constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;
std::pair<int, int> currentCellD;

PlayingState::PlayingState() 
    : mPulseIntensity(0.0f), mTurnCounter(0), mGameSolved(false) {
    
    if (!mFont.openFromMemory(embedded_font_data, embedded_font_data_size)) {
        std::cout << "Error cargando fuente en PlayingState" << std::endl;
    }
    
    initializeGame();
}

void PlayingState::initializeGame() {
    mPlayerPosition = {0, 0};
    mTurnCounter = 0;
    mGameSolved = false;
    mAdjacentCells = mGrid.getAdjacentCells(mPlayerPosition.first, mPlayerPosition.second);
    std::cout << "Juego inicializado. Posición del jugador: (" 
              << mPlayerPosition.first << ", " << mPlayerPosition.second << ")" << std::endl;
}

void PlayingState::handleEvent(const sf::Event& event, Game* game) {
    if (event.is<sf::Event::KeyPressed>()) {
        const sf::Event::KeyPressed* keyEvent = event.getIf<sf::Event::KeyPressed>();
        if (keyEvent->code == sf::Keyboard::Key::Escape) {
            game->changeState(GameState::MainMenu);
            return;
        }
    }

    if (event.is<sf::Event::MouseButtonPressed>()) {
        const sf::Event::MouseButtonPressed* mouseButtonEvent = event.getIf<sf::Event::MouseButtonPressed>();
        if (mouseButtonEvent->button == sf::Mouse::Button::Left && !mGameSolved) {
            std::pair<int, int> clickedCell = mGrid.getClickedCell(mouseButtonEvent->position.x, mouseButtonEvent->position.y);
            //std::cout << "first: " << clickedCell.first << " Second: " << clickedCell.second << std::endl;
            if (clickedCell.first != -1 && clickedCell.second != -1) {
                if (mGrid.isAdjacent(mPlayerPosition, clickedCell)) {
                    mPlayerPosition = clickedCell;
                    mTurnCounter++;
                    mAdjacentCells = mGrid.getAdjacentCells(mPlayerPosition.first, mPlayerPosition.second);
                    currentCellD = clickedCell;
                    std::cout << "Jugador movido a: (" << clickedCell.first 
                             << ", " << clickedCell.second << ") - Turno: " << mTurnCounter << std::endl;
                }
            }
        }
    }

    // Manejar hover de botones directamente aquí
    if (event.is<sf::Event::MouseMoved>()) {
        const sf::Event::MouseMoved* mouseMoveEvent = event.getIf<sf::Event::MouseMoved>();
        sf::Vector2f mousePosF(static_cast<float>(mouseMoveEvent->position.x), 
                               static_cast<float>(mouseMoveEvent->position.y));
        
        // Actualizar hover states directamente
        mPrevHover = mPrevButton.getGlobalBounds().contains(mousePosF);
        mPlayPauseHover = mPlayPauseButton.getGlobalBounds().contains(mousePosF);
        mNextHover = mNextButton.getGlobalBounds().contains(mousePosF);
    }

    if (event.is<sf::Event::MouseButtonPressed>()) {
        const sf::Event::MouseButtonPressed* mouseButtonEvent = event.getIf<sf::Event::MouseButtonPressed>();
        if (mouseButtonEvent->button == sf::Mouse::Button::Left) {
            sf::Vector2f clickPos(static_cast<float>(mouseButtonEvent->position.x), 
                                 static_cast<float>(mouseButtonEvent->position.y));
            
            // Verificar clics en botones directamente
            if (mPrevButton.getGlobalBounds().contains(clickPos)) {
                std::cout << "Botón Anterior presionado" << std::endl;
                // Tu lógica aquí
                return;
            }
            
            if (mPlayPauseButton.getGlobalBounds().contains(clickPos)) {
                mIsPlaying = !mIsPlaying;
                std::cout << (mIsPlaying ? "Play" : "Pause") << " presionado" << std::endl;
                // Tu lógica aquí
                return;
            }
            
            if (mNextButton.getGlobalBounds().contains(clickPos)) {
                std::cout << "Botón Siguiente presionado" << std::endl;
                // Tu lógica aquí
                return;
            }
            
            // Lógica del juego solo si no se hizo clic en botones
            if (!mGameSolved) {
                std::pair<int, int> clickedCell = mGrid.getClickedCell(mouseButtonEvent->position.x, mouseButtonEvent->position.y);
                
                if (clickedCell.first != -1 && clickedCell.second != -1) {
                    if (mGrid.isAdjacent(mPlayerPosition, clickedCell)) {
                        mPlayerPosition = clickedCell;
                        mTurnCounter++;
                        mAdjacentCells = mGrid.getAdjacentCells(mPlayerPosition.first, mPlayerPosition.second);
                        
                        std::cout << "Jugador movido a: (" << clickedCell.first 
                                 << ", " << clickedCell.second << ") - Turno: " << mTurnCounter << std::endl;
                    }
                }
            }
        }
    }
}

void PlayingState::update(float deltaTime, Game* game) {
    float time = mAnimationClock.getElapsedTime().asSeconds();
    mPulseIntensity = (std::sin(time * 3.0f) + 1.0f) * 0.5f;

    
}

void PlayingState::render(sf::RenderWindow& window, Game* game) {
    window.clear(sf::Color::Black);
    //std::cout << "mPlayerPosition PLAYING:" << mPlayerPosition.first << ", " << mPlayerPosition.second << std::endl;
    draw_map(window, mPlayerPosition, mAdjacentCells, mPulseIntensity);
    
    // Info del juego
    sf::Text turnText(mFont, "Turnos: " + std::to_string(mTurnCounter), 24);
    turnText.setFillColor(sf::Color::White);
    turnText.setPosition({(WINDOW_WIDTH/3)*2+30+30, 200});
    window.draw(turnText);

    // Info de la celda
    sf::Text currentCellText(mFont, "Celda Actual: Fila: " + std::to_string(currentCellD.second) + " Columna: " + std::to_string(currentCellD.first), 18);
    currentCellText.setFillColor(sf::Color::White);
    currentCellText.setPosition({(WINDOW_WIDTH/3)*2+30+30, 200+30});
    window.draw(currentCellText);

    // BOTONES DEL REPRODUCTOR - Creados localmente
    const float buttonSize = 60.0f;
    const float spacing = 20.0f;
    const float startX = (WINDOW_WIDTH/3)*2+30+(((WINDOW_WIDTH/3)/2)-((buttonSize*3+spacing*2)/2)); //WINDOW_WIDTH - (3 * buttonSize + 2 * spacing + 30)
    const float buttonY = WINDOW_HEIGHT-buttonSize-75.6;
    
    // Botón anterior
    mPrevButton.setSize({buttonSize, buttonSize});
    mPrevButton.setPosition({startX, buttonY});
    mPrevButton.setFillColor(sf::Color(70, 70, 70));
    mPrevButton.setOutlineThickness(2);
    mPrevButton.setOutlineColor(sf::Color::White);
    
    // Botón play/pause
    mPlayPauseButton.setSize({buttonSize, buttonSize});
    mPlayPauseButton.setPosition({startX + buttonSize + spacing, buttonY});
    mPlayPauseButton.setFillColor(sf::Color(70, 70, 70));
    mPlayPauseButton.setOutlineThickness(2);
    mPlayPauseButton.setOutlineColor(sf::Color::White);
    
    // Botón siguiente
    mNextButton.setSize({buttonSize, buttonSize});
    mNextButton.setPosition({startX + 2 * (buttonSize + spacing), buttonY});
    mNextButton.setFillColor(sf::Color(70, 70, 70));
    mNextButton.setOutlineThickness(2);
    mNextButton.setOutlineColor(sf::Color::White);
    
    // Dibujar botones
    window.draw(mPrevButton);
    window.draw(mPlayPauseButton);
    window.draw(mNextButton);

        // Crear iconos simples con texto
    // Iconos directamente aquí
    sf::Text prevIcon(mFont, "<<", 18);
    prevIcon.setFillColor(sf::Color::White);
    prevIcon.setPosition({startX + 12, buttonY + 18});
    window.draw(prevIcon);
    
    sf::Text playIcon(mFont, mIsPlaying ? "||" : ">", mIsPlaying ? 18 : 20);
    playIcon.setFillColor(sf::Color::White);
    playIcon.setPosition({startX + buttonSize + spacing + (mIsPlaying ? 18 : 20), buttonY + 18});
    window.draw(playIcon);
    
    sf::Text nextIcon(mFont, ">>", 18);
    nextIcon.setFillColor(sf::Color::White);
    nextIcon.setPosition({startX + 2 * (buttonSize + spacing) + 12, buttonY + 18});
    window.draw(nextIcon);
    
    sf::Text escText(mFont, "Presiona ESC para volver al menu", 20);
    escText.setFillColor(sf::Color::Black);
    escText.setPosition({10, WINDOW_HEIGHT - 30});
    window.draw(escText);
}

void PlayingState::onEnter(Game* game) {
    initializeGame();
        // Inicializar estados de botones si es necesario
    mIsPlaying = false;
    mPrevHover = false;
    mPlayPauseHover = false;
    mNextHover = false;
}