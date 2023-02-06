#pragma once

#include <array>
#include <cmath>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

#include "chessman.h"
#include "scene_node.h"

class Chessboard : public SceneNode {
 public:
  Chessboard(const float size);
  void NewPiece(Chessman::Color color, Chessman::Type type, size_t file, size_t rank,
                const TextureWrapper& textures);
  bool MoveIsLegal(size_t file, size_t rank, size_t end_file, size_t end_rank) const;
  bool IsPiecePicked() const;
  void Pick(const sf::Vector2f& position);
  void Drag(const sf::Vector2f& position);
  void Release(const sf::Vector2f& position);
  void PickCancel();

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

    const bool& LegalMoveFlag() const;
    bool& LegalMoveFlag();

    const bool& AttackFlag() const;
    bool& AttackFlag();

    const bool& MoveFlag() const;
    bool& MoveFlag();

   protected:
    virtual void DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

   private:
    sf::RectangleShape shape_;
    Chessman* piece_;
    bool legal_move_flag_;
    bool attack_flag_;
    bool move_flag_;
  };

  //virtual void DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

 private:
  Square* const& SquareAt(size_t file, size_t rank) const;
  Square*& SquareAt(size_t file, size_t rank);
  Chessman* const& PieceAt(size_t file, size_t rank) const;
  Chessman*& PieceAt(size_t file, size_t rank);

  Square* SquareAt(const sf::Vector2f& position) const;
  Chessman* PieceAt(const sf::Vector2f& position) const;

  std::pair<size_t, size_t> GetCoordinatesOfSquare(const Square* square) const;
  std::pair<size_t, size_t> GetCoordinatesOfPiece(const Chessman* piece) const;

  bool IsPathClear(size_t file, size_t rank, size_t end_file, size_t end_rank) const;

  void MarkSquares();
  void Move(const sf::Vector2f& position);
  void Unpick();
  void Undrag();

  static const int kSquaresNo = 64;
  static const int kSideSquaresNo = 8;

  float size_;
  std::array<Square*, kSquaresNo> squares_;

  enum Layer { kSquares, kPieces, kAir, kLayerCount };
  std::array<SceneNode*, Layer::kLayerCount> scene_layers_;
  SceneNode scene_graph_;

  Square* picked_square_;
  Chessman* dragged_piece_;
};
