#pragma once

#include <cmath>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

#include "entity.h"
#include "square.h"

class Chessboard : public sf::Drawable {
 public:
  Chessboard(const float size, bool white_down = true);
  const sf::Vector2f& GetPosition() const;
  void SetPosition(const sf::Vector2f& position);

 protected:
  virtual void draw(sf::RenderTarget& target,
                    sf::RenderStates states) const override;

 private:
  static const int kSquaresNo = 64;
  static const int kSideSquaresNo = 8;
  bool white_down_;
  float size_;
  Square squares_[kSideSquaresNo][kSideSquaresNo];
};
