#include "chessboard.h"

#include <cassert>
#include <cmath>
#include <iostream>

#include "circle_node.h"

Chessboard::Chessboard(const float size) : size_{size}, picked_square_{nullptr} {
  // Initialize the different layers
  for (std::size_t i = 0; i < kLayerCount; ++i) {
    std::unique_ptr<SceneNode> layer(new SceneNode());
    scene_layers_[i] = layer.get();

    AttachChild(std::move(layer));
  }

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
      scene_layers_[kSquares]->AttachChild(std::move(square));
    }
  }
}

void Chessboard::NewPiece(Chessman::Color color, Chessman::Type type, size_t file, size_t rank,
                          const TextureWrapper& textures) {
  std::unique_ptr<Chessman> piece{new Chessman(color, type, textures)};
  PieceAt(file, rank) = piece.get();
  SquareAt(file, rank)->AttachChild(std::move(piece));
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

bool Chessboard::Pick(const sf::Vector2f& position) {
  Square* new_pick = SquareAt(position);

  if (new_pick) {
    if (picked_square_) {
      if (new_pick == picked_square_) {
        Unpick();
      } else {
        if (new_pick->Piece() &&
            new_pick->Piece()->IsWhite() == picked_square_->Piece()->IsWhite()) {
          Unpick();
          picked_square_ = new_pick;
          auto coordinates = GetCoordinatesOfSquare(picked_square_);

          // Find all legal moves
          for (auto& square : squares_) {
            auto end_coordinates = GetCoordinatesOfSquare(square);
            if (MoveIsLegal(coordinates.first, coordinates.second, end_coordinates.first,
                            end_coordinates.second)) {
              if (square->Piece() &&
                  square->Piece()->IsWhite() != picked_square_->Piece()->IsWhite()) {
                square->AttackFlag() = true;
              } else {
                square->LegalMoveFlag() = true;
              }
            }
          }
        } else if (new_pick->LegalMoveFlag() || new_pick->AttackFlag()) {
          if (new_pick->Piece()) {
            new_pick->DetachChild(*new_pick->Piece());
          }
          new_pick->Piece() = picked_square_->Piece();
          new_pick->AttachChild(std::move(picked_square_->DetachChild(*picked_square_->Piece())));
          picked_square_->Piece() = nullptr;
          Unpick();
        } else {
          Unpick();
        }
      }
    } else if (new_pick->Piece()) {
      picked_square_ = new_pick;
      auto coordinates = GetCoordinatesOfSquare(picked_square_);

      // Find all legal moves
      for (auto& square : squares_) {
        auto end_coordinates = GetCoordinatesOfSquare(square);
        if (MoveIsLegal(coordinates.first, coordinates.second, end_coordinates.first,
                        end_coordinates.second)) {
          if (square->Piece() && square->Piece()->IsWhite() != picked_square_->Piece()->IsWhite()) {
            square->AttackFlag() = true;
          } else {
            square->LegalMoveFlag() = true;
          }
        }
      }
    }
  } else {
    Unpick();
  }

  //if (!picked_square_ && new_pick && new_pick->Piece()) {
  //  picked_square_ = new_pick;
  //  auto coordinates = GetCoordinatesOfSquare(picked_square_);

  //  // Find all legal moves
  //  for (auto& square : squares_) {
  //    auto end_coordinates = GetCoordinatesOfSquare(square);
  //    if (MoveIsLegal(coordinates.first, coordinates.second, end_coordinates.first,
  //                    end_coordinates.second)) {
  //      square->LegalMoveFlag() = true;
  //    }
  //  }
  //  return true;
  //}

  //if (picked_square_) {
  //  if (new_pick) {
  //    auto coordinates = GetCoordinatesOfSquare(picked_square_);
  //    auto end_coordinates = GetCoordinatesOfSquare(new_pick);
  //    if (MoveIsLegal(coordinates.first, coordinates.second, end_coordinates.first,
  //                    end_coordinates.second)) {
  //      if (new_pick->Piece()) {
  //        new_pick->DetachChild(*new_pick->Piece());
  //      }
  //      new_pick->Piece() = picked_square_->Piece();
  //      new_pick->AttachChild(std::move(picked_square_->DetachChild(*picked_square_->Piece())));
  //      picked_square_->Piece() = nullptr;
  //      Unpick();
  //    } else {
  //      Unpick();
  //    }
  //    return true;
  //  } else {
  //    Unpick();
  //  }
  //  return false;
  //}

  return false;
}

void Chessboard::Unpick() {
  picked_square_ = nullptr;

  // Remove flags
  for (auto& square : squares_) {
    square->LegalMoveFlag() = false;
    square->AttackFlag() = false;
  }
}

//--- Square class --- //

Chessboard::Square::Square(float side) : legal_move_flag_{false} {
  SetSize(side);
}

const sf::Color& Chessboard::Square::GetColor() const {
  return shape_.getFillColor();
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
  sf::FloatRect r{GetWorldPosition(), shape_.getSize()};
  if (r.contains(position)) {
    return true;
  } else {
    return false;
  }
}

void Chessboard::Square::SetPiecePosition(const sf::Vector2f& position) {
  if (piece_) {
    piece_->setPosition(position);
  }
}

const bool& Chessboard::Square::LegalMoveFlag() const {
  return legal_move_flag_;
}

bool& Chessboard::Square::LegalMoveFlag() {
  return legal_move_flag_;
}

const bool& Chessboard::Square::AttackFlag() const {
  return attack_flag_;
}

bool& Chessboard::Square::AttackFlag() {
  return attack_flag_;
}

void Chessboard::Square::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(shape_, states);
  if (legal_move_flag_) {
    float r = GetSize() / 8;
    const size_t point_count = 90;
    sf::CircleShape circle_mark{r, point_count};
    circle_mark.setFillColor(sf::Color(15, 15, 15, 30));
    circle_mark.setOrigin({r, r});
    circle_mark.move({GetSize() / 2, GetSize() / 2});
    target.draw(circle_mark, states);
  } else if (attack_flag_) {
    sf::RectangleShape square_mark{shape_};
    square_mark.setFillColor(sf::Color(255, 153, 153, 120));
    target.draw(square_mark, states);
  }
}

//--- Square class end --- //

//void Chessboard::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
//  target.draw(scene_graph_, states);
//}

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

Chessboard::Square* Chessboard::SquareAt(const sf::Vector2f& position) const {
  for (const auto& square : squares_) {
    if (square->Contains(position)) {
      return square;
    }
  }
  return nullptr;
}

Chessman* Chessboard::PieceAt(const sf::Vector2f& position) const {
  if (SquareAt(position)) {
    return SquareAt(position)->Piece();
  } else {
    return nullptr;
  }
}

std::pair<size_t, size_t> Chessboard::GetCoordinatesOfSquare(const Square* square) const {
  Square* found = nullptr;
  size_t i = 0;
  for (; i < squares_.size(); i++) {
    if (squares_.at(i) == square) {
      found = squares_.at(i);
      break;
    }
  }
  assert(found);
  return std::pair<size_t, size_t>(i % kSideSquaresNo, kSideSquaresNo - 1 - (i / kSideSquaresNo));
}

std::pair<size_t, size_t> Chessboard::GetCoordinatesOfPiece(const Chessman* piece) const {
  Chessman* found = nullptr;
  size_t i = 0;
  for (; i < squares_.size(); i++) {
    if (squares_.at(i)->Piece() == piece) {
      found = squares_.at(i)->Piece();
      break;
    }
  }
  assert(found);
  return std::pair<size_t, size_t>(i % kSideSquaresNo, kSideSquaresNo - 1 - (i / kSideSquaresNo));
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
