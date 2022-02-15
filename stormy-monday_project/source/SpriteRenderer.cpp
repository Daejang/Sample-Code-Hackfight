//-----------------------------------------------------------------------------
// File Name:    SpriteRenderer.cpp
// Author(s):    Kalby Jang
// Course:       GAM 200
// Copyright     "Copyright Symbol"  2020 DigiPen (USA) Corporation.
//-----------------------------------------------------------------------------

#include <GL/glew.h>

#include "SpriteRenderer.h"

#include "GameObject.h"     // Parent()
#include "Renderer.h"
#include "SpriteFont.h"
#include "SpriteSource.h"

//------------------------------------------------------------------------------
// Class Constructors and Destructors
//------------------------------------------------------------------------------
SpriteRenderer::SpriteRenderer() : SpriteRenderer(nullptr)
{
  
}

SpriteRenderer::SpriteRenderer( const TexturePtr texture ) : _alpha( 1.0f ), 
                                                             text(NULL),
															 spriteSource(NULL),
															 frameIndex(0),
                                                             Component(ComponentType::cSpriteRenderer)
{
  _texture = texture;

  Renderer::Inst()->AddSpriteRenderer(this);
}

SpriteRenderer::SpriteRenderer( const SpriteRenderer& other ) : Component(other) 
{
  *this = other;
}

SpriteRenderer::~SpriteRenderer()
{
	if (text != NULL)
		delete text;

  if (_texture)
  {
    delete _texture;
    _texture = nullptr;
  }
    
  Renderer::Inst()->RemoveSpriteRenderer( this );
}


ComponentPtr SpriteRenderer::Clone() const
{
  return new SpriteRenderer(*this);
}



//-----------------------------------------------------------------------------
// Public Methods
//-----------------------------------------------------------------------------
void SpriteRenderer::Start()
{
//  parentTransform = Parent()->GetComponent<Transform>(ComponentType::cTransform);

}

void SpriteRenderer::TextureToNull()
{
  _texture = nullptr;
}

// void SpriteRenderer::Draw() const
// {



//  TransformPtr transform = Parent()->GetComponent<Transform>(ComponentType::cTransform);

//  Renderer::Inst()->Draw( transform->Matrix() );
// }

// Getters ------------------------------------------------------------------
const GLuint SpriteRenderer::GetVAO()
{
  return _spriteVAO;
}

//const TransformPtr SpriteRenderer::GetTransform()
//{
//  return parentTransform;
//}

const TexturePtr SpriteRenderer::Texture() const
{
  return _texture;
}

const MeshPtr SpriteRenderer::Mesh() const
{
  return _mesh;
}

float SpriteRenderer::Alpha()
{
  return _alpha;
}

const SpriteFontPtr SpriteRenderer::Text() const
{
	return text;
}

const SpriteSourcePtr SpriteRenderer::GetSpriteSource() const
{
	return spriteSource;
}

unsigned int SpriteRenderer::GetFrameIndex() const
{
	return frameIndex;
}

RenderLayer SpriteRenderer::GetRenderLayer() const
{
	return _renderLayer;
}


// Setters --------------------------------------------------------------------
void SpriteRenderer::Texture( TexturePtr texture )
{
  if (!texture) return;

  _texture = texture;
}


void SpriteRenderer::Mesh( const MeshPtr mesh )
{
  if (!mesh) return;

  _mesh = mesh;
}

void SpriteRenderer::Alpha( float value )
{
  _alpha = value;
}


void SpriteRenderer::Text(std::string ctext)
{
	delete text;

	text = new SpriteFont(ctext);

	text->AddToSprite(this);
}

void SpriteRenderer::SetSpriteSource(const SpriteSourcePtr cspriteSource)
{
	spriteSource = cspriteSource;
}

void SpriteRenderer::SetFrameIndex(unsigned cframeIndex)
{
	frameIndex = cframeIndex;
}


void SpriteRenderer::SetRenderLayer(RenderLayer layer)
{
	_renderLayer = layer;
}


