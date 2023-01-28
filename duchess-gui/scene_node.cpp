#include "scene_node.h"

void SceneNode::AttachChild(std::unique_ptr<SceneNode> child) {
  child->parent_ = this;
  children_.push_back(std::move(child));
}

sf::Vector2f SceneNode::getWorldPosition() const {
  return getWorldTransform() * sf::Vector2f();
}

sf::Transform SceneNode::getWorldTransform() const {
  sf::Transform transform = sf::Transform::Identity;

  for (const SceneNode* node = this; node != nullptr; node = node->parent_)
    transform = node->getTransform() * transform;

  return transform;
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();
  DrawCurrent(target, states);
  DrawChildren(target, states);
}

void SceneNode::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {}

void SceneNode::DrawChildren(sf::RenderTarget& target, sf::RenderStates states) const {
  for (const auto& child : children_) {
    target.draw(*child, states);
  }
}
