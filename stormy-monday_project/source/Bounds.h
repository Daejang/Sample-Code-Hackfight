/*!****************************************************************************
\file   Bounds.h
\author Kalby Jang
\brief  Bounding boxs AABB 
//
// Copyright 2020 DigiPen (USA) Corporation.
//
 *****************************************************************************/
#pragma once
#include "glm/glm.hpp"

using namespace glm;

class Bounds2D
{
  public:

    Bounds2D(vec2 center, vec2 size);
    
    void Encapsulate(vec2 point);
    bool Contains(vec2 point);

  private:

  vec2 _center;
  vec2 _extexts;
  vec2 _max;
  vec2 _min;
  vec2 _size;
};


