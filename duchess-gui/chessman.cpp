#include "chessman.h"

Chessman::Chessman(const std::string& id, const TextureWrapper& textures)
    : sprite_{textures.Get(id)} {}

void Chessman::SetTexture(const sf::Texture& texture, bool resetRect) {
  sprite_.setTexture(texture, resetRect);
}

void Chessman::SetSize(float size) {
  float scale = size / sprite_.getTexture()->getSize().x;
  sprite_.setScale({scale, scale});
}

void Chessman::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();
  target.draw(sprite_, states);
}
