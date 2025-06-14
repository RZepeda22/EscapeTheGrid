#ifndef _GAME_HPP_
#define _GAME_HPP_

#include <SFML/Graphics.hpp>

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
  sf::CircleShape mPlayer;
};

#endif // _GAME_HPP_