#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable {
 public:
  void AttachChild(std::unique_ptr<SceneNode> child);

 protected:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
  virtual void DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
  virtual void DrawChildren(sf::RenderTarget& target, sf::RenderStates states) const;

 private:
  SceneNode* parent_;
  std::vector<std::unique_ptr<SceneNode>> children_;
};
