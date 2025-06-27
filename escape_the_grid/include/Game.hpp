#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "TriangularGrid.hpp" 
#include <memory>

// Forward declarations
class GameStateBase;

enum class GameState {
    MainMenu,
    LoadState,
    Playing
};



class Game {
public:
  Game();
  ~Game(); // Declarar pero NO definir aquí
  void run();

  // Métodos públicos para que los estados puedan cambiar el estado del juego
  void changeState(GameState newState);
  void closeWindow();


private:
  void processEvents();
  void update();
  void render();


  void initializeStates();



private:
  sf::RenderWindow mWindow;
  sf::Clock mClock; // Reloj para controlar el tiempo entre frames



  /*// Estado del juego
  GameMode mGameMode;
  std::pair<int, int> mPlayerPosition;
  std::vector<std::pair<int, int>> mAdjacentCells;
  int mTurnCounter;
  bool mGameSolved;


  // Para animación
  sf::Clock mAnimationClock;
  float mPulseIntensity;

  // Grid triangular
  TriangularGrid mGrid;*/

  // Para el menú
  sf::Font mFont;
  std::vector<sf::Text> mMenuOptions;
  int mSelectedMenuOption;

  // Estado actual del juego
  GameState mCurrentState;
  std::unique_ptr<GameStateBase> mMainMenuState;
  std::unique_ptr<GameStateBase> mPlayingState;
  std::unique_ptr<GameStateBase> mLoadState;

  GameStateBase* getCurrentStatePtr();

};
