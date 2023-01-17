#pragma once

#include "entity.h"

class Chessman : public sf::Transformable, public sf::Drawable {
 public:
  enum Color {
    kWhite     =   0x0000'0001, 
    kBlack     =   0x0000'0010 
  };
  enum Type {
    kKing      =   0x0000'0000,
    kQueen     =   0x0001'0000,
    kBishop    =   0x0010'0000,
    kKnight    =   0x0011'0000,
    kRook      =   0x0100'0000,
    kPawn      =   0x0101'0000
  };

  Chessman(Color color, Type type, const TextureWrapper& textures);
  void SetTexture(const sf::Texture& texture, bool resetRect = false);
  void SetSize(float size);

 protected:
  virtual void draw(sf::RenderTarget& target,
                    sf::RenderStates states) const override;

 private:
  Color color_;
  Type type_;
  sf::Sprite sprite_;
};
