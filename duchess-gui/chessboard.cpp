#include "chessboard.h"

#include <cmath>

Chessboard::Chessboard(const float size) : size_{size} {
  board_shape_.setSize({size, size});
  board_shape_.setOutlineColor(sf::Color::Black);
  board_shape_.setOutlineThickness(1);
  float square_size = size_ / kSideSquaresNo;
  for (size_t i = 0; i < kSideSquaresNo; i++) {
    for (size_t j = 0; j < kSideSquaresNo; j++) {
      squares_[i][j].SetSize(square_size);
      squares_[i][j].setPosition(
          GetPosition() + sf::Vector2f{square_size * j, square_size * i});
      if ((j + i) % 2 != 0) {
        squares_[i][j].SetColor(sf::Color{209, 139, 71});
      } else {
        squares_[i][j].SetColor(sf::Color{255, 206, 158});
      }
    }
  }
}

const sf::Vector2f& Chessboard::GetPosition() const {
  return board_shape_.getPosition();
}

void Chessboard::SetPosition(const sf::Vector2f& position) {
  board_shape_.setPosition(position);
  float square_size = size_ / kSideSquaresNo;
  for (size_t i = 0; i < kSideSquaresNo; i++) {
    for (size_t j = 0; j < kSideSquaresNo; j++) {
      squares_[i][j].setPosition(
          position + sf::Vector2f{square_size * j, square_size * i});
    }
  }
}

Chessboard::Square::Square(float side) {
  SetSize(side);
}

void Chessboard::Square::SetColor(const sf::Color& color) {
  shape_.setFillColor(color);
}

void Chessboard::Square::SetTexture(const sf::Texture& texture,
                                    bool resetRect) {
  shape_.setTexture(&texture, resetRect);
}

void Chessboard::Square::SetSize(float side) {
  shape_.setSize({side, side});
}

void Chessboard::Square::draw(sf::RenderTarget& target,
                              sf::RenderStates states) const {
  states.transform *= getTransform();
  target.draw(shape_, states);
}

void Chessboard::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  //states.transform *= getTransform();
  target.draw(board_shape_, states);
  for (size_t i = 0; i < kSideSquaresNo; i++) {
    for (size_t j = 0; j < kSideSquaresNo; j++) {
      target.draw(squares_[i][j], states);
    }
  }
}
