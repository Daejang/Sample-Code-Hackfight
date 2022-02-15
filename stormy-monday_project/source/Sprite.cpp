//-----------------------------------------------------------------------------
// File Name:    Sprite.cpp
// Author(s):    Kalby Jang
// Course:       GAM 200
// Copyright     "Copyright Symbol"  2020 DigiPen (USA) Corporation.
//-----------------------------------------------------------------------------
#include "Sprite.h"

Sprite::Sprite() : Component(ComponentType::cSprite)
{
  _tex = nullptr;
}

Sprite::Sprite( TexturePtr texture ) : Component(ComponentType::cSprite)
{
  _tex = texture;
}

Sprite::Sprite(const Sprite& other) : Component(other)
{
  *this = other;
}

Sprite::~Sprite()
{
  
}


ComponentPtr Sprite::Clone() const
{
  return new Sprite(*this);
}

void Sprite::Update( float dt )
{
}

// void Sprite::Draw() const
// {
// }

const TexturePtr Sprite::Texture() const
{
  return _tex;
}

void Sprite::Texture( const TexturePtr texture )
{
  _tex = texture;
}


