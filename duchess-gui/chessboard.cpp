#include "chessboard.h"

#include <cmath>

Chessboard::Chessboard(const float size) : size_{size}, selected_square_{nullptr} {
  // Prepare the border (board outline)
  board_shape_.setSize({size, size});
  board_shape_.setOutlineColor(sf::Color::Black);
  board_shape_.setOutlineThickness(1);

  // Prepare the squares
  squares_.fill(nullptr);
  float square_size = size_ / kSideSquaresNo;
  for (size_t i = 0; i < kSideSquaresNo; i++) {
    for (size_t j = 0; j < kSideSquaresNo; j++) {
      std::unique_ptr<Square> square{new Square(square_size)};
      square->SetSize(square_size);
      square->move(sf::Vector2f{square_size * i, square_size * (kSideSquaresNo - j - 1)});
      if ((j + i) % 2 == 0) {
        square->SetColor(sf::Color{209, 139, 71});  // dark
      } else {
        square->SetColor(sf::Color{255, 206, 158});  // light
      }
      SquareAt(i, j) = square.get();
      AttachChild(std::move(square));
    }
  }
}

void Chessboard::NewPiece(Chessman::Color color, Chessman::Type type, size_t file, size_t rank,
                          const TextureWrapper& textures) {
  std::unique_ptr<Chessman> piece{new Chessman(color, type, textures)};
  PieceAt(file, rank) = piece.get();
  SquareAt(file, rank)->AttachChild(std::move(piece));
}

bool Chessboard::SelectPieceAt(const sf::Vector2f& position) {
  for (size_t i = 0; i < kSideSquaresNo; i++) {
    for (size_t j = 0; j < kSideSquaresNo; j++) {
      if (SquareAt(i, j)->Contains(position) && PieceAt(i, j) != nullptr) {
        selected_square_ = &squares_[i][j];
        UnmarkAll();
        for (size_t k = 0; k < kSideSquaresNo; k++) {
          for (size_t l = 0; l < kSideSquaresNo; l++) {
            if (MoveIsLegal(j, i, l, k)) {
              if (PieceAt(l, k) != nullptr) {
                squares_[k][l].Threaten();
              } else {
                squares_[k][l].Mark();
              }
            }
          }
        }
        return true;
      }
    }
  }
  return false;
}

void Chessboard::MoveSelectedPiece(const sf::Vector2f& position) {
  if (selected_square_) {
    selected_square_->SetPiecePosition(
        position - sf::Vector2f{selected_square_->GetSize() / 2, selected_square_->GetSize() / 2});
  }
}

void Chessboard::ResetSelectedPiece() {
  selected_square_->SetPiecePosition(selected_square_->getPosition());
}

bool Chessboard::MoveIsLegal(size_t file, size_t rank, size_t end_file, size_t end_rank) const {
  // Check if the destination square is within the bounds of the board
  if (file < 0 || file >= kSideSquaresNo || rank < 0 || rank >= kSideSquaresNo || end_file < 0 ||
      end_file >= kSideSquaresNo || end_rank < 0 || end_rank >= kSideSquaresNo) {
    return false;
  }

  Chessman* piece = PieceAt(file, rank);

  // Check if the destination square is occupied by a piece of the same color
  Chessman* end_piece = PieceAt(end_file, end_rank);
  if (end_piece != nullptr && end_piece->IsWhite() == piece->IsWhite()) {
    return false;
  }

  // Check if the move is valid for the specific type of piece
  switch (piece->GetType()) {
    case Chessman::Type::kPawn:
      // Pawns can only move forward and capture diagonally
      if (piece->IsWhite()) {
        if (end_file == file) {
          if (rank == 1 && end_rank == 3 && PieceAt(file, 2) == nullptr && end_piece == nullptr) {
            return true;
          }
          if (end_rank - rank == 1 && PieceAt(file, end_rank) == nullptr) {
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
          if (rank == 6 && end_rank == 4 && PieceAt(file, 5) == nullptr && end_piece == nullptr) {
            return true;
          }
          if (rank - end_rank == 1 && PieceAt(file, end_rank) == nullptr) {
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
  if (!IsPathClear(file, rank, end_file, end_rank)) {
    return false;
  }

  // If all checks pass, the move is valid
  return true;
}

bool Chessboard::Move(const sf::Vector2f& position) {
  //size_t file, rank;
  //for (size_t i = 0; i < kSideSquaresNo; i++) {
  //  for (size_t j = 0; j < kSideSquaresNo; j++) {
  //    if (squares_[i][j].Contains(selected_square_->getPosition())) {
  //      file = j;
  //      rank = i;
  //      break;
  //    }
  //  }
  //}

  //for (size_t i = 0; i < kSideSquaresNo; i++) {
  //  for (size_t j = 0; j < kSideSquaresNo; j++) {
  //    if (squares_[i][j].Contains(position)) {
  //      if (MoveIsLegal(file, rank, j, i)) {
  //        SetPiece(i, j, squares_[rank][file].MovePiece());
  //        return true;
  //      }
  //    }
  //  }
  //}
  return false;
}

void Chessboard::UnmarkAll() {
  for (size_t i = 0; i < kSideSquaresNo; i++) {
    for (size_t j = 0; j < kSideSquaresNo; j++) {
      squares_[i][j].Unmark();
      squares_[i][j].Unthreaten();
    }
  }
}

Chessboard::Square::Square(float side) : marked_{false}, threatened_{false} {
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

Chessman* const& Chessboard::Square::Piece() const {
  return piece_;
}

Chessman*& Chessboard::Square::Piece() {
  return piece_;
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

Chessman* Chessboard::Square::MovePiece() {
  return piece_;
}

void Chessboard::Square::Mark() {
  marked_ = true;
}

void Chessboard::Square::Unmark() {
  marked_ = false;
}

void Chessboard::Square::Threaten() {
  threatened_ = true;
}

void Chessboard::Square::Unthreaten() {
  threatened_ = false;
}

void Chessboard::Square::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(shape_, states);
  /*if (piece_) {
    target.draw(*piece_);
  }
  if (marked_) {
    float r = shape_.getSize().x / 2;
    sf::CircleShape c;
    c.setRadius(r / 2);
    c.setFillColor({37, 150, 190, 120});
    c.setOrigin({r / 2, r / 2});
    c.setPosition(GetPosition() + sf::Vector2f{r, r});
    target.draw(c);
  }
  if (threatened_) {
    sf::RectangleShape t{shape_};
    t.setFillColor(sf::Color{255, 93, 82, 120});
    target.draw(t);
  }*/
}

void Chessboard::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(board_shape_, states);
}

Chessboard::Square* const& Chessboard::SquareAt(size_t file, size_t rank) const {
  return squares_.at(file + (kSideSquaresNo - rank - 1) * kSideSquaresNo);
}

Chessboard::Square*& Chessboard::SquareAt(size_t file, size_t rank) {
  return squares_.at(file + (kSideSquaresNo - rank - 1) * kSideSquaresNo);
}

Chessman* const& Chessboard::PieceAt(size_t file, size_t rank) const {
  return SquareAt(file, rank)->Piece();
}

Chessman*& Chessboard::PieceAt(size_t file, size_t rank) {
  return SquareAt(file, rank)->Piece();
}

bool Chessboard::IsPathClear(size_t file, size_t rank, size_t end_file, size_t end_rank) const {
  Chessman* piece = PieceAt(file, rank);
  auto type = piece->GetType();

  // check the path for rook and queen moves
  if (type == Chessman::Type::kRook || type == Chessman::Type::kQueen) {
    if (end_rank == rank) {
      // move is horizontal
      int step = (end_file > file) ? 1 : -1;
      for (int f = file + step; f != end_file; f += step) {
        if (PieceAt(f, rank) != nullptr) {
          return false;
        }
      }
    } else if (end_file == file) {
      // move is vertical
      int step = (end_rank > rank) ? 1 : -1;
      for (int r = rank + step; r != end_rank; r += step) {
        if (PieceAt(file, r) != nullptr) {
          return false;
        }
      }
    }
  }
  // check the path for bishop and queen moves
  if (type == Chessman::Type::kBishop || type == Chessman::Type::kQueen) {
    if (std::abs((int)(end_rank - rank)) == std::abs((int)(end_file - file))) {
      int rank_step = (end_rank > rank) ? 1 : -1;
      int file_step = (end_file > file) ? 1 : -1;
      for (int r = rank + rank_step, f = file + file_step; r != end_rank;
           r += rank_step, f += file_step) {
        if (PieceAt(f, r) != nullptr) {
          return false;
        }
      }
    }
  }
  return true;
}
