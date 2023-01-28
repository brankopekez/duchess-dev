#pragma once

#include <map>
#include <memory>
#include <string>

#include <SFML/Graphics/Texture.hpp>

namespace Textures {
enum ID {
  kChessPieces,
  kWhiteKing,
  kBlackKing,
  kWhiteQueen,
  kBlackQueen,
  kWhiteBishop,
  kBlackBishop,
  kWhiteKnight,
  kBlackKnight,
  kWhiteRook,
  kBlackRook,
  kWhitePawn,
  kBlackPawn
};
}

class TextureWrapper {
 public:
  void Load(Textures::ID id, const std::string& filename,
            const sf::IntRect& area = sf::IntRect()) noexcept(false);
  sf::Texture& Get(Textures::ID id) noexcept(false);
  const sf::Texture& Get(Textures::ID id) const noexcept(false);
  void Smooth(bool smooth) noexcept(true);

 private:
  std::map<Textures::ID, std::unique_ptr<sf::Texture>> textures_;
};
