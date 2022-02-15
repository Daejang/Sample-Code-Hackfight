//-----------------------------------------------------------------------------
// File Name:    SpriteRenderer.h
// Author(s):    Kalby Jang
// Course:       GAM 200
// Copyright     "Copyright Symbol"  2020 DigiPen (USA) Corporation.
//-----------------------------------------------------------------------------
#pragma once

#include <gl/glew.h>
#include "Component.h"

#include <string>

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------
//typedef class Sprite Sprite, *SpritePtr;
typedef class Texture Texture, *TexturePtr;
typedef class Mesh Mesh, *MeshPtr;
typedef class Transform Transform, *TransformPtr;
typedef class SpriteFont SpriteFont, *SpriteFontPtr;
typedef class SpriteSource SpriteSource, *SpriteSourcePtr;



typedef enum RenderLayer
{
	layerBackground,
	layerParticlesBehind,
	layerEnemies,
	layerPlayer,
	layerParticlesFront,
	layerForeground,
	layerUI,
	layerDebug,

	layerCount,
} RenderLayer;

//------------------------------------------------------------------------------
// SpriteRenderer Class
//------------------------------------------------------------------------------
class SpriteRenderer : public Component
{
public:

  SpriteRenderer();
  SpriteRenderer( const TexturePtr texture );

  SpriteRenderer( const SpriteRenderer& other );

  ~SpriteRenderer();

  // ~SpriteRenderer();

  // Component Methods
  ComponentPtr Clone() const override;

  // Public Methods -----------------------------------------------------------
  void Start() override;
  // void Draw() const override;

  void TextureToNull();




  // Getters ------------------------------------------------------------------
  const GLuint     GetVAO();
  const TransformPtr GetTransform();
  const TexturePtr Texture() const;
  const MeshPtr    Mesh() const;
  const SpriteFontPtr Text() const;
  const SpriteSourcePtr GetSpriteSource() const;
  unsigned int GetFrameIndex() const;

  RenderLayer GetRenderLayer() const;

  float Alpha();

  // Setters ------------------------------------------------------------------
  void Texture( TexturePtr texture );
  void Mesh(const MeshPtr mesh);
  void Text(std::string ctext);
  void Alpha(float value);
  void SetSpriteSource(const SpriteSourcePtr cspriteSource);
  void SetFrameIndex(unsigned cframeIndex);

  void SetRenderLayer(RenderLayer layer);

//   void Sprite(Sprite& sprite);


private:

  GLuint _spriteVAO;
//  Transform parentTransform;

  TexturePtr _texture;
  MeshPtr    _mesh;
  float      _alpha;

  SpriteFontPtr text;
  SpriteSourcePtr spriteSource;
  unsigned int frameIndex;

  RenderLayer _renderLayer;
};
