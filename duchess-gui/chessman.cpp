#include "chessman.h"

Chessman::Chessman(Color color, Type type, const TextureWrapper& textures)
    : color_{color}, type_{type}, sprite_{textures.Get(color | type)} {}

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
