#include "square.h"

int Square::StringToSquare(std::string str) {
  if (str.length() >= 2) {
    return str.at(0) - 'a' + str.at(1) * 8;
  }
}

Square::Square(float side) {
  SetSize(side);
}

void Square::SetColor(const sf::Color& color) {
  shape_.setFillColor(color);
}

void Square::SetTexture(const sf::Texture& texture, bool resetRect) {
  shape_.setTexture(&texture, resetRect);
}

void Square::SetSize(float side) {
  shape_.setSize({side, side});
}

void Square::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();
  target.draw(shape_, states);
}
