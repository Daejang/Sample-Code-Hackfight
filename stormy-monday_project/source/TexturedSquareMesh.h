//-----------------------------------------------------------------------------
// File Name:    TexturedSquareMesh.h
// Author(s):    Kalby Jang
// Course:       GAM 200
// Copyright     "Copyright Symbol"  2020 DigiPen (USA) Corporation.
//-----------------------------------------------------------------------------
#pragma once
#include "SquareMesh.h"

class TexturedSquareMesh : public SquareMesh
{
public:

  TexturedSquareMesh();

  const glm::vec4* texcoordArray() const;
  void SetTextureCoordinates(float start_u, float start_v, float u, float v);












private:

  glm::vec4 texcoords[4];
};

