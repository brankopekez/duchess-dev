#pragma once

#include <array>
#include <cmath>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

#include "chessman.h"
#include "entity.h"
#include "scene_node.h"

class Chessboard : public SceneNode {
 public:
  Chessboard(const float size);

  void ResetSelectedSquare();
  bool MoveIsLegal(size_t file, size_t rank, size_t end_file, size_t end_rank) const;
  bool Move(const sf::Vector2f& position);
  void UnmarkAll();

  void AssignPiece(size_t file, size_t rank, Chessman* piece);
  Chessman* PieceAt(const sf::Vector2f& position) const;
  std::pair<size_t, size_t> GetCoordinatesOfPiece(const Chessman* piece) const;
  void PickUpPiece(const Chessman* piece);

 protected:
  class Square : public SceneNode {
   public:
    Square(float side);
    const sf::Color& GetColor() const;
    void SetColor(const sf::Color& color);
    void SetTexture(const sf::Texture& texture, bool resetRect = false);
    void SetSize(float side);
    float GetSize() const;

    Chessman* const& Piece() const;
    Chessman*& Piece();

    bool Contains(const sf::Vector2f& position) const;
    void SetPiecePosition(const sf::Vector2f& position);
    Chessman* MovePiece();
    void Mark();
    void Unmark();
    void Threaten();
    void Unthreaten();

   protected:
    virtual void DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

   private:
    sf::RectangleShape shape_;
    Chessman* piece_;
    bool marked_;
    bool threatened_;
  };

  virtual void DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

 private:
  Square* const& SquareAt(size_t file, size_t rank) const;
  Square*& SquareAt(size_t file, size_t rank);
  Chessman* const& PieceAt(size_t file, size_t rank) const;
  Chessman*& PieceAt(size_t file, size_t rank);

  Square* SquareAt(const sf::Vector2f& position) const;
  std::pair<size_t, size_t> GetCoordinatesOfSquare(const Square* square) const;

  bool IsPathClear(size_t file, size_t rank, size_t end_file, size_t end_rank) const;

  static const int kSquaresNo = 64;
  static const int kSideSquaresNo = 8;

  float size_;
  std::array<Square*, kSquaresNo> squares_;
  sf::RectangleShape board_shape_;
  Square* selected_square_;
};
