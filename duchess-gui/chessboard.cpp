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
      squares_[i][j].SetPosition(GetPosition() + sf::Vector2f{square_size * j, square_size * i});
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
      squares_[i][j].SetPosition(position + sf::Vector2f{square_size * j, square_size * i});
    }
  }
}

void Chessboard::NewPiece(Chessman::Color color, Chessman::Type type, size_t file, size_t rank,
                          const TextureWrapper& textures) {
  squares_[rank][file].SetPiece(std::unique_ptr<Chessman>(new Chessman(color, type, textures)));
}

Chessboard::Square::Square(float side) {
  SetSize(side);
}

void Chessboard::Square::SetColor(const sf::Color& color) {
  shape_.setFillColor(color);
}

void Chessboard::Square::SetTexture(const sf::Texture& texture, bool resetRect) {
  shape_.setTexture(&texture, resetRect);
}

void Chessboard::Square::SetSize(float side) {
  shape_.setSize({side, side});
}

const sf::Vector2f& Chessboard::Square::GetPosition() const {
  return shape_.getPosition();
}

void Chessboard::Square::SetPosition(const sf::Vector2f& position) {
  shape_.setPosition(position);
  if (piece_) {
    piece_->setPosition(position);
  }
}

void Chessboard::Square::SetPiece(std::unique_ptr<Chessman> piece) {
  piece_ = std::move(piece);
  piece_->setPosition(GetPosition());
}

void Chessboard::Square::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  //states.transform *= getTransform();
  target.draw(shape_, states);
  if (piece_) {
    target.draw(*piece_);
  }
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
