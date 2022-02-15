//-----------------------------------------------------------------------------
// File Name:    Sprite.h
// Author(s):    Kalby Jang
// Course:       GAM 200
// Copyright     "Copyright Symbol"  2020 DigiPen (USA) Corporation.
//-----------------------------------------------------------------------------
#pragma once
#include "Component.h"

//-----------------------------------------------------------------------------
//Forward Referencing
//-----------------------------------------------------------------------------
typedef class Texture Texture, *TexturePtr;

typedef class Sprite Sprite, *SpritePtr;


class Sprite : public Component
{
public:
	Sprite();
  Sprite(TexturePtr texture);
	Sprite(const Sprite& other);
	~Sprite();

  // Component Methods;
  ComponentPtr Clone() const override;

  void Update(float dt) override;
  // void Draw() const override;

  // Getters ------------------------------------------------------------------
  const TexturePtr Texture() const;

  // Setters ------------------------------------------------------------------
  void Texture(const TexturePtr texture);

private:
	TexturePtr _tex;
};