#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include "chessboard.h"
#include "chessman.h"
#include "texture_wrapper.h"

class Game {
 public:
  Game();
  void Run();

 private:
  static const sf::Time kTimePerFrame;

  void LoadTextures();
  void ProcessInput();
  void Update(sf::Time elapsedTime);
  void Render();

  sf::RenderWindow window_;
  TextureWrapper textures_;
  Chessboard board_;

  bool press_;
  bool drag_;
};
