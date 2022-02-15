//-----------------------------------------------------------------------------
// File Name:    SquareMesh.h
// Author(s):    Kalby Jang
// Course:       GAM 200
// Copyright     "Copyright Symbol"  2020 DigiPen (USA) Corporation.
//-----------------------------------------------------------------------------
#pragma once

#include "Mesh.h"
#include "Affine.h"

class SquareMesh : public Mesh
{
public:
	~SquareMesh(void) {}
	int vertexCount(void) const;
	const glm::vec4* vertexArray(void) const;
	glm::vec4 dimensions(void) const;
	glm::vec4 center(void) const;
	int faceCount(void) const;
	const Face* faceArray(void) const;
	int edgeCount(void) const;
	const Edge* edgeArray(void) const;
private:
  static const glm::vec4 vertices[4];
  static const Face faces[2];
  static const Edge edges[4];

//  GLuint _meshHandle;

};