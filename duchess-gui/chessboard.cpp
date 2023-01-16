#include "chessboard.h"

#include <cmath>

Chessboard::Chessboard(const float size, bool white_down)
    : white_down_{white_down}, size_{size} {
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
  if (white_down_) {
    return squares_[0][0].getPosition();
  } else {
    return squares_[kSideSquaresNo - 1][kSideSquaresNo - 1].getPosition();
  }
}

void Chessboard::SetPosition(const sf::Vector2f& position) {
  float square_size = size_ / kSideSquaresNo;
  for (size_t i = 0; i < kSideSquaresNo; i++) {
    for (size_t j = 0; j < kSideSquaresNo; j++) {
      squares_[i][j].setPosition(
          position + sf::Vector2f{square_size * j, square_size * i});
    }
  }
}

void Chessboard::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  //states.transform *= getTransform();
  for (size_t i = 0; i < kSideSquaresNo; i++) {
    for (size_t j = 0; j < kSideSquaresNo; j++) {
      target.draw(squares_[i][j], states);
    }
  }
}
