//-----------------------------------------------------------------------------
// File Name:    Camera.cpp
// Author(s):    Kalby Jang
// Course:       GAM 200
// Copyright     "Copyright Symbol"  2020 DigiPen (USA) Corporation.
//-----------------------------------------------------------------------------
#include "Camera.h"
#include "GameObject.h"
#include "Affine.h"
#include "Renderer.h"


using namespace Affine;

Camera::Camera() : _center(Point(0, 0)), _rightVec(Vector(1, 0)), _upVec(Vector(0, 1)), _rect_width(2), _rect_height(2), _aspectRatio( vec2(4.0, 3.0) ), _zoomLevel( 1.0f ),  
                   Component( cCamera )
{ }

Camera::Camera( const vec2& center, float W, float H, vec2 aspectRatio) : Component( cCamera )
{
  // _center      = center;
  // transform->Position(center);
  _rightVec    = Vector(1.0, 0);
  _upVec       = Vector(0, 1.0);
  _aspectRatio = aspectRatio;
  _windowDimensions = vec2(W, H);
  _rect_width  = H * (_aspectRatio.x / _aspectRatio.y);
  _rect_height = H;
  _zoomLevel   = H / 2.0f;

}

Camera::Camera( float zoom, vec2 aspectRatio ) : Component( cCamera )
{
  // Renderer::mainCamera = this;
  Renderer::AddCamera( this );
  
  // _center      = center;
  // transform->Position(position);
  _rightVec    = Vector(1.0, 0);
  _upVec       = Vector(0, 1.0);
  _aspectRatio = aspectRatio;
  _rect_width  = zoom * (_aspectRatio.x / _aspectRatio.y);
  _rect_height = zoom;
  _zoomLevel   = zoom;

  
}

Camera::Camera( const Camera& other ) : Component( other )
{
  *this = other;
}

Camera::~Camera()
{
  Renderer::RemoveCamera( this );
}

ComponentPtr Camera::Clone() const
{
  return new Camera(*this);
}

void Camera::Start()
{
  // transform = Parent()->transform;
}

void Camera::Move( const vec2& units )
{
  // _center = glm::mat4(_rightVec, _upVec, glm::vec4(0), _center) * Point(units.x, units.y);
  Parent()->transform->Position( vec2( glm::mat3(_rightVec, _upVec, Affine::Point2D( Parent()->transform->Position().x, Parent()->transform->Position().y ) )  * Point2D(units.x, units.y) ) );
  // transform->Position( glm::mat3(_rightVec, _upVec, Affine::Point2D( transform->Position().x, transform->Position().y ) ) * Point2D(units.x, units.y) );

  // return *this;
}

void Camera::Position( const vec2& pos )
{
  // _center = glm::mat4(_rightVec, _upVec, glm::vec4(0), Point(0.0, 0.0)) * Point(pos.x, pos.y);
  Parent()->transform->Position( vec2( glm::mat3(_rightVec, _upVec, Point2D(0.0, 0.0)) * Point2D(pos.x, pos.y) ));

  // return *this;
}

void Camera::Rotate( float degrees )
{
  float t = radians( degrees );
  // mat4 newRotate = mat4(Vector(cos(t), sin(t)), Vector(-sin(t), cos(t)), vec4(0), Point(0, 0));
  mat3 newRotate = Rotate2D( t );

  _rightVec = newRotate * _rightVec;
  _upVec    = newRotate * _upVec;

  // return *this;
}

void Camera::Zoom( float zoom )
{
  _zoomLevel = zoom;

  _rect_width  = _zoomLevel * (_aspectRatio.x / _aspectRatio.y);
  _rect_height = _zoomLevel;

  // return *this;
}

const mat4 Camera::WorldToCamera()
{
  return Affine::inverse2D(CameraToWorld());
}

const mat4 Camera::CameraToNDC()
{
  // return mat4(Vector(2 / _rect_width, 0), Vector(0, 2 / _rect_height), vec4(0), Point(0, 0));
  return mat4(Vector(2 / _rect_width, 0), Vector(0, 2 / _rect_height), vec4(0), Point(0, 0));
  // return mat3(Vector(2 / _rect_width, 0), Vector(0, 2 / _rect_height), Point2D(0, 0));
}

const mat4 Camera::NDCToCamera()
{
  // return mat4(Vector(_rect_width / 2, 0), Vector(0, _rect_height / 2), vec4(0), Point(0, 0));
  return mat4(Vector(_rect_width / 2, 0), Vector(0, _rect_height / 2), vec4(0), Point(0, 0));
  // return mat3(Vector(_rect_width / 2, 0), Vector(0, _rect_height / 2), Point2D(0, 0));
}

const mat4 Camera::CameraToWorld()
{
  // return mat4( _rightVec, _upVec, vec4(0), _center );
  return mat4( vec4(_rightVec, 0), vec4(_upVec, 0), vec4(0), Point( Parent()->transform->Position().x, Parent()->transform->Position().y ) );
  // return mat3( _rightVec, _upVec, Point2D( transform->Position().x, transform->Position().y ) );
}

// const vec3 Camera::ScreenToWorld( const vec3& )
// {
//   glm::vec4 viewport = glm::vec4 (0.0f, 0.0f, _windowDimensions);
//   glm::mat4 tempView(1.0f);
//   glm::mat4 tempProj = glm::perspective();
//
//
//   return ;
// }

void Camera::ResizeWindow(int w, int h)
{
  _windowDimensions = vec2(w, h);
  _aspectRatio = vec2(float(w), float(h));
  // _rect_width = (float)h * (_aspectRatio.x / _aspectRatio.y);
  // _rect_height = (float)h;
  
  Zoom(_zoomLevel);
}
