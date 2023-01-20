#include "chessboard.h"

#include <cmath>

Chessboard::Chessboard(const float size) : size_{size}, selected_square_{nullptr} {
  board_shape_.setSize({size, size});
  board_shape_.setOutlineColor(sf::Color::Black);
  board_shape_.setOutlineThickness(1);
  float square_size = size_ / kSideSquaresNo;
  for (size_t i = 0; i < kSideSquaresNo; i++) {
    for (size_t j = 0; j < kSideSquaresNo; j++) {
      squares_[i][j].SetSize(square_size);
      squares_[i][j].SetPosition(
          GetPosition() + sf::Vector2f{square_size * j, square_size * (kSideSquaresNo - i - 1)});
      if ((j + i) % 2 == 0) {
        squares_[i][j].SetColor(sf::Color{209, 139, 71});   // dark
      } else {
        squares_[i][j].SetColor(sf::Color{255, 206, 158});  // light
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
      squares_[kSideSquaresNo - i - 1][j].SetPosition(
          position + sf::Vector2f{square_size * j, square_size * i});
    }
  }
}

void Chessboard::NewPiece(Chessman::Color color, Chessman::Type type, size_t file, size_t rank,
                          const TextureWrapper& textures) {
  squares_[rank][file].SetPiece(std::unique_ptr<Chessman>(new Chessman(color, type, textures)));
}

void Chessboard::SelectSquareAt(const sf::Vector2f& position) {
  for (size_t i = 0; i < kSideSquaresNo; i++) {
    for (size_t j = 0; j < kSideSquaresNo; j++) {
      if (squares_[i][j].Contains(position)) {
        selected_square_ = &squares_[i][j];
        return;
      }
    }
  }
}

void Chessboard::MoveSelectedPiece(const sf::Vector2f& position) {
  if (selected_square_) {
    selected_square_->SetPiecePosition(
        position - sf::Vector2f{selected_square_->GetSize() / 2, selected_square_->GetSize() / 2});
  }
}

void Chessboard::ResetSelectedSquare() {
  selected_square_->SetPiecePosition(selected_square_->GetPosition());
}

bool Chessboard::MoveIsLegal(size_t file, size_t rank, size_t end_file, size_t end_rank) const {
  // Check if the destination square is within the bounds of the board
  if (file < 0 || file >= kSideSquaresNo || rank < 0 || rank >= kSideSquaresNo || end_file < 0 ||
      end_file >= kSideSquaresNo || end_rank < 0 || end_rank >= kSideSquaresNo) {
    return false;
  }

  Chessman* piece = squares_[rank][file].GetPiece();

  // Check if the destination square is occupied by a piece of the same color
  Chessman* end_piece = squares_[end_rank][end_file].GetPiece();
  if (end_piece != nullptr && end_piece->IsWhite() == piece->IsWhite()) {
    return false;
  }

  // Check if the move is valid for the specific type of piece
  switch (piece->GetType()) {
    case Chessman::Type::kPawn:
      // Pawns can only move forward and capture diagonally
      if (piece->IsWhite()) {
        if (end_file == file) {
          if (rank == 1 && end_rank == 3 && squares_[2][file].GetPiece() == nullptr) {
            return true;
          }
          if (end_rank - rank == 1 && squares_[end_rank][file].GetPiece() == nullptr) {
            return true;
          }
          return false;
        }
        if (std::abs((int)(end_file - file)) == 1 && end_rank - rank == 1 && end_piece != nullptr) {
          // Pawns can only capture diagonally
          return true;
        }
        return false;
      } else {
        if (end_file == file) {
          if (rank == 6 && end_rank == 4 && squares_[5][file].GetPiece() == nullptr) {
            return true;
          }
          if (rank - end_rank == 1 && squares_[end_rank][file].GetPiece() == nullptr) {
            return true;
          }
          return false;
        }
        if (std::abs((int)(file - end_file)) == 1 && rank - end_rank == 1 && end_piece != nullptr) {
          // Pawns can only capture diagonally
          return true;
        }
        return false;
      }
      break;
    case Chessman::Type::kKnight:
      // knights can move in an "L" shape in any direction
      if ((std::abs((int)(end_rank - rank)) != 2 || std::abs((int)(end_file - file)) != 1) &&
          (std::abs((int)(end_rank - rank)) != 1 || std::abs((int)(end_file - file)) != 2)) {
        return false;
      }
      break;
    case Chessman::Type::kBishop:
      // Bishops can move diagonally in any direction
      if (std::abs((int)(end_rank - rank)) != std::abs((int)(end_file - file))) {
        return false;
      }
      break;
    case Chessman::Type::kRook:
      // Rooks can move horizontally or vertically in any direction
      if (end_rank != rank && end_file != file) {
        return false;
      }
      break;
    case Chessman::Type::kQueen:
      // Queens can move like bishops or rooks
      if (std::abs((int)(end_rank - rank)) != std::abs((int)(end_file - file)) &&
          end_rank != rank && end_file != file) {
        return false;
      }
      break;
    case Chessman::Type::kKing:
      // Kings can move one square in any direction
      if (std::abs((int)(end_rank - rank)) > 1 || std::abs((int)(end_file - file)) > 1) {
        return false;
      }
      break;
  }

  // Check if the path to the destination square is clear
  //if (!isPathClear(end_rank, end_file, board)) {
  //  return false;
  //}

  // If all checks pass, the move is valid
  return true;
}

bool Chessboard::Move(const sf::Vector2f& position) {
  size_t file, rank;
  for (size_t i = 0; i < kSideSquaresNo; i++) {
    for (size_t j = 0; j < kSideSquaresNo; j++) {
      if (squares_[i][j].Contains(selected_square_->GetPosition())) {
        file = j;
        rank = i;
        break;
      }
    }
  }

  for (size_t i = 0; i < kSideSquaresNo; i++) {
    for (size_t j = 0; j < kSideSquaresNo; j++) {
      if (squares_[i][j].Contains(position)) {
        if (MoveIsLegal(file, rank, j, i)) {
          squares_[i][j].SetPiece(std::move(squares_[rank][file].MovePiece()));
          return true;
        }
      }
    }
  }
  return false;
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

float Chessboard::Square::GetSize() const {
  return shape_.getSize().x;
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

bool Chessboard::Square::Contains(const sf::Vector2f& position) const {
  sf::FloatRect r{shape_.getPosition(), shape_.getSize()};
  if (r.contains(static_cast<sf::Vector2f>(position))) {
    return true;
  } else {
    return false;
  }
}

void Chessboard::Square::SetPiecePosition(const sf::Vector2f& position) {
  if (piece_) {
    piece_->setPosition(static_cast<sf::Vector2f>(position));
  }
}

Chessman* Chessboard::Square::GetPiece() const {
  return piece_.get();
}

std::unique_ptr<Chessman> Chessboard::Square::MovePiece() {
  return std::move(piece_);
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
  if (selected_square_) {
    target.draw(*selected_square_, states);
  }
}
