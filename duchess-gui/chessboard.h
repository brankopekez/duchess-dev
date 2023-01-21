#pragma once

#include <cmath>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

#include "chessman.h"
#include "entity.h"

class Chessboard : public sf::Drawable {
 public:
  Chessboard(const float size);
  const sf::Vector2f& GetPosition() const;
  void SetPosition(const sf::Vector2f& position);
  void NewPiece(Chessman::Color color, Chessman::Type type, size_t file, size_t rank,
                const TextureWrapper& textures);
  void SelectSquareAt(const sf::Vector2f& position);
  void MoveSelectedPiece(const sf::Vector2f& position);
  void ResetSelectedSquare();
  bool MoveIsLegal(size_t file, size_t rank, size_t end_file, size_t end_rank) const;
  bool Move(const sf::Vector2f& position);

 protected:
  class Square : public sf::Drawable {
   public:
    Square(float side = 0);
    void SetColor(const sf::Color& color);
    void SetTexture(const sf::Texture& texture, bool resetRect = false);
    void SetSize(float side);
    float GetSize() const;
    const sf::Vector2f& GetPosition() const;
    void SetPosition(const sf::Vector2f& position);
    void SetPiece(std::unique_ptr<Chessman> piece);
    bool Contains(const sf::Vector2f& position) const;
    void SetPiecePosition(const sf::Vector2f& position);
    Chessman* GetPiece() const;
    std::unique_ptr<Chessman> MovePiece();
    
   protected:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

   private:
    sf::RectangleShape shape_;
    std::unique_ptr<Chessman> piece_;
  };

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 private:
  bool IsPathClear(size_t file, size_t rank, size_t end_file, size_t end_rank) const;

  static const int kSquaresNo = 64;
  static const int kSideSquaresNo = 8;
  float size_;
  Square squares_[kSideSquaresNo][kSideSquaresNo];
  sf::RectangleShape board_shape_;
  Square* selected_square_;
};
