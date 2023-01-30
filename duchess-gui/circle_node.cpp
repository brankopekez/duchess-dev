#include "circle_node.h"

CircleNode::CircleNode(float radius, std::size_t pointCount) : shape_{radius, pointCount} {}

void CircleNode::SetFillColor(const sf::Color& color) {
  shape_.setFillColor(color);
}

void CircleNode::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(shape_, states);
}
