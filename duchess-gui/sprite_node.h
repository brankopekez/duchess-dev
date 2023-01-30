#pragma once

#include <SFML/Graphics/Sprite.hpp>

#include "scene_node.h"

class SpriteNode : public SceneNode {
 public:
  explicit SpriteNode(const sf::Texture& texture);
  SpriteNode(const sf::Texture& texture, const sf::IntRect& textureRect);

 private:
  virtual void DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

 private:
  sf::Sprite sprite_;
};
