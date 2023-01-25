#include "scene_node.h"

void SceneNode::AttachChild(std::unique_ptr<SceneNode> child) {
  child->parent_ = this;
  children_.push_back(std::move(child));
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();
  DrawCurrent(target, states);
  DrawChildren(target, states);
}

void SceneNode::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {}

void SceneNode::DrawChildren(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();
  for (const auto& child : children_) {
    target.draw(*child, states);
  }
}
