//-----------------------------------------------------------------------------
// File Name:    Transform.h
// Author(s):    Kalby Jang
// Course:       GAM 200
// Copyright     "Copyright Symbol"  2020 DigiPen (USA) Corporation.
//-----------------------------------------------------------------------------
#pragma once
#include "Component.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/common.hpp>


using namespace glm;

//-----------------------------------------------------------------------------
// Forward References
//-----------------------------------------------------------------------------
typedef class Transform Transform, *TransformPtr;

//-----------------------------------------------------------------------------
// Transform Class
//-----------------------------------------------------------------------------
class Transform : public Component
{
public:

	Transform();
  Transform(const Transform& other);
	~Transform();

  ComponentPtr Clone() const override;
  
  // Class Methods
  void UpdateTransform( );

  // Mehthods
  void Translate(const vec3& translation);

  // Getters
	const bool HasChanged() const;
  const vec2& Position2() const;
	const vec3& Position() const;
  const float Rotation() const;
  const vec2& Scale2() const;
  const vec3& Scale() const;
  const mat4x4& Matrix() const;

  // Setters
	void Position(const vec3& position);
	void Position(const vec2& position);
	// void SetRotation(Quaternion rotation);
	void Rotation(const float& degrees);
	void Scale(const vec3& scale);
	void Scale( const vec2& scale );


private:

	bool hasChanged;

	vec2 _position;
	vec2 _scale;
	float _rotationDegrees;
	float _rotationRadians;

	mat4x4 transformMatrix;

};
