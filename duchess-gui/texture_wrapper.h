#pragma once

#include <map>
#include <memory>
#include <string>

#include <SFML/Graphics/Texture.hpp>

class TextureWrapper {
 public:
  void Load(const std::string& id, const std::string& filename,
            const sf::IntRect& area = sf::IntRect()) noexcept(false);
  sf::Texture& Get(const std::string& id) noexcept(false);
  const sf::Texture& Get(const std::string& id) const noexcept(false);
  void SetSmooth(bool smooth) noexcept(true);

 private:
  std::map<std::string, std::unique_ptr<sf::Texture>> textures_;
};
