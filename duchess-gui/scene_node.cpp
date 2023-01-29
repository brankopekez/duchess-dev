#include "scene_node.h"

#include <cassert>

void SceneNode::AttachChild(std::unique_ptr<SceneNode> child) {
  child->parent_ = this;
  children_.push_back(std::move(child));
}

std::unique_ptr<SceneNode> SceneNode::DetachChild(const SceneNode& node) {
  auto found = std::find_if(children_.begin(), children_.end(),
                            [&](std::unique_ptr<SceneNode>& p) { return p.get() == &node; });
  assert(found != children_.end());

  std::unique_ptr<SceneNode> result = std::move(*found);
  result->parent_ = nullptr;
  children_.erase(found);
  return result;
}

sf::Vector2f SceneNode::GetWorldPosition() const {
  return GetWorldTransform() * sf::Vector2f();
}

sf::Transform SceneNode::GetWorldTransform() const {
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
