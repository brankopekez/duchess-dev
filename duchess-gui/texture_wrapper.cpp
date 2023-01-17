#include "texture_wrapper.h"

#include <cassert>
#include <stdexcept>

void TextureWrapper::Load(int id, const std::string& filename,
                          const sf::IntRect& area) noexcept(false) {
  std::unique_ptr<sf::Texture> texture{new sf::Texture};
  if (!texture->loadFromFile(filename, area)) {
    throw std::runtime_error("Loading failed.");
  }
  auto inserted = textures_.insert(std::make_pair(id, std::move(texture)));
  assert(inserted.second);
}

sf::Texture& TextureWrapper::Get(int id) noexcept(false) {
  if (auto search = textures_.find(id); search != textures_.end()) {
    return *search->second;
  } else {
    throw std::runtime_error("Get failed.");
  }
}

const sf::Texture& TextureWrapper::Get(int id) const
    noexcept(false) {
  if (auto search = textures_.find(id); search != textures_.end()) {
    return *search->second;
  } else {
    throw std::runtime_error("Get failed.");
  }
}

void TextureWrapper::SetSmooth(bool smooth) noexcept(true) {
  for (auto const& t : textures_) {
    t.second->setSmooth(smooth);
  }
}
