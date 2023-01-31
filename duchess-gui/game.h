#pragma once

#include <array>

#include <SFML/Graphics.hpp>

#include "chessboard.h"
#include "chessman.h"
#include "scene_node.h"
#include "texture_wrapper.h"

class Game {
 public:
  Game();
  void Run();

 private:
  enum Layer { kBoard, kLayerCount };

  static const sf::Time kTimePerFrame;

  void LoadTextures(bool smooth = true);
  void BuildScene();
  void ProcessInput();
  void Update(sf::Time elapsedTime);
  void Render();

  sf::RenderWindow window_;
  TextureWrapper textures_;

  SceneNode scene_graph_;
  std::array<SceneNode*, kLayerCount> scene_layers_;
  Chessboard* board_;

  bool press_;
  bool drag_;
  bool pick_;
};
