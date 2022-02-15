
//-------------------
// File Name:    Camera.h
// Author(s):    Kalby Jang
// Course:       GAM 200
// Copyright     "Copyright Symbol"  2020 DigiPen (USA) Corporation.
//-----------------------------------------------------------------------------

#pragma once

#include <glm/glm.hpp>
#include "Component.h"
#include "Transform.h"

using namespace glm;

typedef class Camera Camera, *CameraPtr;

class Camera : public Component
{
public:

  Camera(); 
  Camera(const vec2& center, float W, float H, vec2 aspectRatio);
  Camera(float zoom, vec2 aspectRatio);

  Camera(const Camera& other);

  ~Camera();
  ComponentPtr Clone() const override;

  // Inherited
  void Start() override;


  // public Methods
  void Move( const vec2& units );
  void Position( const vec2& pos );
  void Rotate( float degrees );
  void Zoom( float zoom );

	// Getters ------------------------------------------------------------------
	// const vec4& Center() const { return _center; }
	// const vec3& Center() const { return Parent()->transform; }
	const vec3& Right() const { return _rightVec; }
	const vec3& Up() const { return _upVec; }
	const float Width() const { return _rect_width; }
	const float Height() const { return _rect_height; }
  const float Zoom() const { return _zoomLevel; }

  const mat4 WorldToCamera();
  const mat4 CameraToNDC();
  const mat4 NDCToCamera();
  const mat4 CameraToWorld();

  // const vec3 ScreenToWorld(const vec3&);

	// Setters ------------------------------------------------------------------
	void ResizeWindow(int w, int h);

  // Static -------------------------------------------------------------------
  static Camera& main;

  // Variables ----------------------------------------------------------------
  // TransformPtr transform;

private:

  vec3 _center;
  vec3 _rightVec, _upVec;
  vec2 _aspectRatio;
  vec2 _windowDimensions;
  float _rect_width, _rect_height;
  float _zoomLevel;
  

};
