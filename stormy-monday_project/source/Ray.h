/*!****************************************************************************
\file   Ray.h
\author Kalby Jang
\brief  Data class for rays
//
// Copyright 2020 DigiPen (USA) Corporation.
//
 *****************************************************************************/

#pragma once
#include <glm/glm.hpp>

class Ray
{
public:

  Ray() :
  _origin( glm::vec2(0.0f) ), _direction( glm::vec2(1.0f, 0.0f) )
  { }
  Ray(const glm::vec2& origin , const glm::vec2& direction ) :
    _origin( origin ), _direction( glm::normalize( direction ) )
  { }

  glm::vec2 _origin    = glm::vec2(0.0);
  glm::vec2 _direction = glm::vec2(0.0);
};
