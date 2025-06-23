#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "TriangularGrid.hpp" 
enum class GameMode {
    Manual,
    Auto
};

class Game {
public:
  Game();
  void run();

private:
  void processEvents();
  void update();
  void render();

private:
  sf::RenderWindow mWindow;

  // Estado del juego
  GameMode mGameMode;
  std::pair<int, int> mPlayerPosition;
  std::vector<std::pair<int, int>> mAdjacentCells;
  int mTurnCounter;
  bool mGameSolved;


  // Para animación
  sf::Clock mAnimationClock;
  float mPulseIntensity;

  // Grid triangular
  TriangularGrid mGrid;

};
