#include <iostream>
#include "Game.hpp" // Incluimos la definición de nuestra clase Game
#include "DrawMap.hpp"  // Incluimos la función para dibujar el mapa
constexpr unsigned short HEIGHT = 720;
constexpr unsigned short WIDTH = 1280;
#include <cmath>
#include "EmbeddedResources.hpp"
#include "MainMenuState.hpp" // Agrega esta línea para declarar MainMenuState
#include "PlayingState.hpp"
#include "LoadState.hpp"

// Constructor: Se llama automáticamente cuando se crea un objeto Game.
// Aquí inicializamos los miembros de la clase.
Game::Game() :
    mWindow(sf::VideoMode({WIDTH, HEIGHT}), "Escape The Grid - Team 11 - Boveda Genetica", sf::Style::Close),
    mCurrentState(GameState::LoadState) // Empezar en el menú
    //mGameMode(GameMode::Manual),
    //mPlayerPosition({1, 2}), // Posición inicial
    //mTurnCounter(0),
    //mGameSolved(false),
    //mPulseIntensity(0.0f)
    
{
    // Inicializar las celdas adyacentes para la posición inicial
    //mAdjacentCells = mGrid.getAdjacentCells(mPlayerPosition.first, mPlayerPosition.second);

    // Cargar fuente para el menú
    //if (!mFont.openFromMemory(embedded_font_data, embedded_font_data_size)) {
        // Si no tienes una fuente, SFML usará la fuente por defecto
    //    std::cout << "No se pudo cargar la fuente, usando fuente por defecto" << std::endl;
    //}
    
    initializeStates();
    getCurrentStatePtr()->onEnter(this);

}

// Definir el destructor aquí, donde GameStateBase está completamente definido
Game::~Game() = default;

void Game::initializeStates() {
    mMainMenuState = std::make_unique<MainMenuState>();
    mPlayingState = std::make_unique<PlayingState>();
    mLoadState = std::make_unique<LoadState>();
    // mConfigState = std::make_unique<ConfigState>();
    // mCreditsState = std::make_unique<CreditsState>();
}

// El método principal que contiene el bucle del juego.
void Game::run()
{
    while (mWindow.isOpen()) // Mientras la ventana esté abierta...
    {
        float deltaTime = mClock.restart().asSeconds();

        processEvents(); // Procesamos los eventos de entrada (teclado, ratón, cerrar ventana, etc.).
        update();        // Actualizamos la lógica del juego (movimiento de objetos, colisiones, etc.).
        render();        // Dibujamos los elementos en la ventana.
    }
}

// Procesa los eventos que ocurren en la ventana.
void Game::processEvents()
{
    // Iteramos sobre todos los eventos pendientes en la cola.
    while (const std::optional event = mWindow.pollEvent())
    {
        // Si el evento es de tipo sf::Event::Closed (el usuario hace clic en la 'X' para cerrar la ventana).
        if (event->is<sf::Event::Closed>())
        {
            mWindow.close(); // Cerramos la ventana.
        }

        // Evento de mouse presionado
        /*if (event->is<sf::Event::MouseButtonPressed>()) {
            const sf::Event::MouseButtonPressed* mouseButtonEvent = event->getIf<sf::Event::MouseButtonPressed>();
            if (mouseButtonEvent->button == sf::Mouse::Button::Left && mGameMode == GameMode::Manual && !mGameSolved)
            {
                std::cout << "Clic izquierdo en: (" << mouseButtonEvent->position.x 
                         << ", " << mouseButtonEvent->position.y << ")" << std::endl;
                std::pair<int, int> clickedCell = mGrid.getClickedCell(mouseButtonEvent->position.x, mouseButtonEvent->position.y);
                
                if (clickedCell.first != -1 && clickedCell.second != -1)
                {
                    if (mGrid.isAdjacent(mPlayerPosition, clickedCell))
                    {
                        // Mover el jugador
                        mPlayerPosition = clickedCell;
                        mTurnCounter++;
                        
                        // Actualizar las celdas adyacentes para la nueva posición
                        mAdjacentCells = mGrid.getAdjacentCells(mPlayerPosition.first, mPlayerPosition.second);
                        
                        std::cout << "Jugador movido a: (" << clickedCell.first 
                                 << ", " << clickedCell.second << ") - Turno: " << mTurnCounter << std::endl;
                    }
                    else
                    {
                        std::cout << "Movimiento inválido: la celda no es adyacente" << std::endl;
                        std::cout << "Celda clickeada: (" << clickedCell.first << ", " << clickedCell.second << ")" << std::endl;
                        std::cout << "Posición actual: (" << mPlayerPosition.first << ", " << mPlayerPosition.second << ")" << std::endl;
                    }
                }
                else
                {
                    std::cout << "Click fuera del grid" << std::endl;
                }
            }
            else if (mouseButtonEvent->button == sf::Mouse::Button::Right)
            {
                std::cout << "Clic derecho en: (" << mouseButtonEvent->position.x 
                         << ", " << mouseButtonEvent->position.y << ")" << std::endl;
            }
        }*/
        /********************************NEW */
        
        if (GameStateBase* currentState = getCurrentStatePtr()) {
            currentState->handleEvent(*event, this);
        }
        // Aquí podrías añadir más lógica para otros tipos de eventos, como:
        // if (event->is<sf::Event::KeyPressed>()) {
        //     // Lógica para cuando se presiona una tecla.
        // }
    }
}

// Actualiza la lógica del juego. En este ejemplo simple, no hay mucho que actualizar,
// pero aquí iría el código para mover el jugador, actualizar puntuaciones, etc.
void Game::update()
{
    // Actualizar animación de pulso para celdas adyacentes
    //float time = mAnimationClock.getElapsedTime().asSeconds();
    //mPulseIntensity = (std::sin(time * 3.0f) + 1.0f) * 0.5f; // Valor entre 0 y 1, velocidad ajustable
    if (GameStateBase* currentState = getCurrentStatePtr()) {
        float deltaTime = mClock.getElapsedTime().asSeconds();
        currentState->update(deltaTime, this);
    }
}

// Dibuja los elementos en la ventana.
void Game::render()
{
    ///mWindow.clear(sf::Color::White);       // Limpia la ventana con el color por defecto (negro).
    // Usar la versión con efectos visuales
    //draw_map(mWindow, mPlayerPosition, mAdjacentCells, mPulseIntensity);
    if (GameStateBase* currentState = getCurrentStatePtr()) {
        currentState->render(mWindow, this);
    }
    mWindow.display();     // Muestra en pantalla todo lo que se ha dibujado.
}

void Game::changeState(GameState newState) {
    if (GameStateBase* currentState = getCurrentStatePtr()) {
        currentState->onExit(this);
    }
    
    mCurrentState = newState;
    
    if (GameStateBase* newStatePtr = getCurrentStatePtr()) {
        newStatePtr->onEnter(this);
    }
}
void Game::closeWindow() {
    mWindow.close();
}

GameStateBase* Game::getCurrentStatePtr() {
    switch (mCurrentState) {
        case GameState::MainMenu: return mMainMenuState.get();
        case GameState::Playing: return mPlayingState.get();
        case GameState::LoadState: return mLoadState.get();
        default: return nullptr;
    }
}

