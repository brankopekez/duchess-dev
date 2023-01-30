#pragma once

#include <SFML/Graphics/CircleShape.hpp>

#include "scene_node.h"

class CircleNode : public SceneNode {
 public:
  explicit CircleNode(float radius = 0, std::size_t pointCount = 30);
  void SetFillColor(const sf::Color& color);

 protected:
  virtual void DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

 private:
  sf::CircleShape shape_;
};
