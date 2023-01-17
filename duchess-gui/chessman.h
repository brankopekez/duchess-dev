#pragma once

#include "entity.h"

class Chessman : public sf::Transformable, public sf::Drawable {
 public:
  Chessman() = default;
  Chessman(const std::string& id, const TextureWrapper& textures);
  void SetTexture(const sf::Texture& texture, bool resetRect = false);
  void SetSize(float size);

 protected:
  virtual void draw(sf::RenderTarget& target,
                    sf::RenderStates states) const override;

 private:
  sf::Sprite sprite_;
};
