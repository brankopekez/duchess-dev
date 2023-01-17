#pragma once

#include <cmath>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

#include "entity.h"
#include "chessman.h"

class Chessboard : public sf::Drawable {
 public:
  Chessboard(const float size);
  const sf::Vector2f& GetPosition() const;
  void SetPosition(const sf::Vector2f& position);

 protected:
  class Square : public sf::Transformable, public sf::Drawable {
   public:
    Square(float side = 0);
    void SetColor(const sf::Color& color);
    void SetTexture(const sf::Texture& texture, bool resetRect = false);
    void SetSize(float side);

   protected:
    virtual void draw(sf::RenderTarget& target,
                      sf::RenderStates states) const override;

   private:
    sf::RectangleShape shape_;
    std::unique_ptr<Chessman> piece_;
  };

  virtual void draw(sf::RenderTarget& target,
                    sf::RenderStates states) const override;

 private:
  static const int kSquaresNo = 64;
  static const int kSideSquaresNo = 8;
  float size_;
  Square squares_[kSideSquaresNo][kSideSquaresNo];
  sf::RectangleShape board_shape_;
};
