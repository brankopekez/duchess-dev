#pragma once

#include <map>
#include <memory>
#include <string>

#include <SFML/Graphics/Texture.hpp>

namespace Textures {
enum ID {
    kWhiteKing      =   0x0000'0001,
    kBlackKing      =   0x0000'0010,
    kWhiteQueen     =   0x0001'0001,
    kBlackQueen     =   0x0001'0010,
    kWhiteBishop    =   0x0010'0001,
    kBlackBishop    =   0x0010'0010,
    kWhiteKnight    =   0x0011'0001,
    kBlackKnight    =   0x0011'0010,
    kWhiteRook      =   0x0100'0001,
    kBlackRook      =   0x0100'0010,
    kWhitePawn      =   0x0101'0001,
    kBlackPawn      =   0x0101'0010
};
}

class TextureWrapper {
 public:
  void Load(int id, const std::string& filename,
            const sf::IntRect& area = sf::IntRect()) noexcept(false);
  sf::Texture& Get(int id) noexcept(false);
  const sf::Texture& Get(int id) const noexcept(false);
  void SetSmooth(bool smooth) noexcept(true);

 private:
  std::map<int, std::unique_ptr<sf::Texture>> textures_;
};
