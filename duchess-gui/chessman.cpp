#include "chessman.h"
#include "texture_wrapper.h"

Chessman::Chessman(Color color, Type type, const TextureWrapper& textures)
    : color_{color}, type_{type} {
  const sf::Texture& texture{textures.Get(Textures::kChessPieces)};
  sf::Vector2u texture_size{texture.getSize()};
  int d = texture_size.x / Type::kTypeCount;

  int row = 0, column = 0;

  switch (color) {
    case Chessman::kWhite:
      row = 0;
      break;
    case Chessman::kBlack:
      row = 1;
      break;
    default:
      break;
  }

  switch (type) {
    case Chessman::kKing:
      column = 0;
      break;
    case Chessman::kQueen:
      column = 1;
      break;
    case Chessman::kBishop:
      column = 2;
      break;
    case Chessman::kKnight:
      column = 3;
      break;
    case Chessman::kRook:
      column = 4;
      break;
    case Chessman::kPawn:
      column = 5;
      break;
    default:
      break;
  }

  sprite_.setTexture(texture);
  sprite_.setTextureRect(sf::IntRect{{column * d, row * d}, {d, d}});
}

Chessman::Chessman(const Chessman& other)
    : color_{other.color_}, type_{other.type_}, sprite_{other.sprite_} {}

void Chessman::SetTexture(const sf::Texture& texture, bool resetRect) {
  sprite_.setTexture(texture, resetRect);
}

float Chessman::GetSize() const {
  return sprite_.getTextureRect().width;
}

void Chessman::SetSize(float size) {
  float scale = size / GetSize();
  sprite_.setScale({scale, scale});
}

bool Chessman::IsWhite() const {
  return color_ == kWhite;
}

Chessman::Type Chessman::GetType() const {
  return type_;
}

void Chessman::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(sprite_, states);
}
