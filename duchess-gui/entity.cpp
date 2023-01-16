#include "entity.h"

Entity::Entity(const std::string& id, const TextureWrapper& textures)
    : sprite_{textures.Get(id)} {}

void Entity::SetTexture(const sf::Texture& texture, bool resetRect) {
  sprite_.setTexture(texture, resetRect);
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();
  target.draw(sprite_, states);
}
