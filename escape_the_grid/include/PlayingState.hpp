#pragma once

#include "GameStateBase.hpp"
#include "TriangularGrid.hpp"
#include <vector>

enum class GameMode {
    Manual,
    Auto
};

class PlayingState : public GameStateBase {
    
public:
    PlayingState();
    
    void handleEvent(const sf::Event& event, Game* game) override;
    void update(float deltaTime, Game* game) override;
    void render(sf::RenderWindow& window, Game* game) override;
    void onEnter(Game* game) override;
    
private:
    void initializeGame();

private:
   /* sf::Font mFont;
    sf::Clock mAnimationClock;
    float mPulseIntensity;
    
    // Variables del juego
    std::pair<int, int> mPlayerPosition;
    std::vector<std::pair<int, int>> mAdjacentCells;
    int mTurnCounter;
    bool mGameSolved;
    TriangularGrid mGrid;*/

       // Botones del reproductor
    sf::RectangleShape mPrevButton;
    sf::RectangleShape mPlayPauseButton;
    sf::RectangleShape mNextButton;
    
    // Estados de los botones
    bool mIsPlaying;
    bool mPrevHover;
    bool mPlayPauseHover;
    bool mNextHover;
    

    sf::Font mFont;
    
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