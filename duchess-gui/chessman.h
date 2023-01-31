#pragma once

#include "scene_node.h"
#include "texture_wrapper.h"

class Chessman : public SceneNode {
 public:
  enum Color { kWhite, kBlack, kColorCount };
  enum Type { kKing, kQueen, kBishop, kKnight, kRook, kPawn, kTypeCount };

  Chessman(Color color, Type type, const TextureWrapper& textures);
  void SetTexture(const sf::Texture& texture, bool resetRect = false);
  float GetSize() const;
  void SetSize(float size);
  bool IsWhite() const;
  Type GetType() const;

 protected:
  virtual void DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

 private:
  Color color_;
  Type type_;
  sf::Sprite sprite_;
};
