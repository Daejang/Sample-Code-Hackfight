//-----------------------------------------------------------------------------
// File Name:    Transform.cpp
// Author(s):    Kalby Jang
// Course:       GAM 200
// Copyright     "Copyright Symbol"  2020 DigiPen (USA) Corporation.
//-----------------------------------------------------------------------------

#include "Transform.h"
#include <glm/common.hpp>

#include "GameObject.h"
#include "PhysicsSystem.h"
#include "Time.h"
#include "Affine.h"


// Constructor 
Transform::Transform() : hasChanged( true ), _rotationDegrees( 0.0f ), _rotationRadians(0.0f),
                         Component( ComponentType::cTransform )
{
  transformMatrix = mat4(1.0f);
	_position = vec2(0, 0);
	_scale = vec2(1);
}

Transform::Transform( const Transform& other ) : Component(other)
{
  *this = other;
}
Transform::~Transform()
{

}



ComponentPtr Transform::Clone() const
{
	return new Transform(*this);
}


// Component Methods
void Transform::UpdateTransform( )
{
  // Return if no changes are made
  if (!hasChanged) return;

  // if (Parent()->Name().compare("Rat") == 0)
  //   __debugbreak();

  // Prepping for calculations
  mat4x4 newScale, newRotate, newTranslate;
  vec3 forwordAxis = vec3(0, 0, 1);

  newScale = newRotate = newTranslate = mat4x4(1.0f);
  
  // Applying properties to temp matrix
  newRotate = rotate(newRotate, _rotationRadians, forwordAxis);
  newScale  = scale(newScale, Affine::Vector2D( _scale.x, _scale.y)); 
  newTranslate = translate(newTranslate, Affine::Point2D( _position.x, _position.y ));

  // Merge the matrices
  transformMatrix = newTranslate * newRotate * newScale;

  // Calculation complete
  hasChanged = false;
}

void Transform::Translate( const vec3& translation )
{
  _position += vec2(translation);
  hasChanged = true;
}

const bool Transform::HasChanged() const
{
  return hasChanged;
}

const vec2& Transform::Position2() const
{
  return _position;
}

const vec3& Transform::Position() const
{
  return Affine::Point2D(_position.x, _position.y);
}

const float Transform::Rotation() const
{
  return _rotationDegrees;
}

const vec2& Transform::Scale2() const
{
  return _scale;
}

const vec3& Transform::Scale() const
{
  return Affine::Vector2D( _scale.x, _scale.y );
}

const mat4x4& Transform::Matrix() const
{
  return transformMatrix;
}


void Transform::Position(const vec3& position)
{
	// _position = position;
	// hasChanged = true;

  Position( vec2(position) );
}

void Transform::Position(const vec2& position)
{
	// Position( vec3( position.x, position.y, 1 ) );
  _position = position;
  hasChanged = true;

  Physics::Inst()->UpdateObjectHitboxes(Parent(), Time::GetDeltaTime());

  // vec3 (vec2(324, 42), 1.0);
}

void Transform::Rotation(const float& degrees)
{
	_rotationDegrees = degrees;
	_rotationRadians = radians(degrees);
	hasChanged = true;
}

void Transform::Scale( const vec3& scale )
{
	_scale = scale;
	hasChanged = true;
}

void Transform::Scale( const vec2& scale )
{
  Scale( vec3( scale.x, scale.y, 0 ) );
}




