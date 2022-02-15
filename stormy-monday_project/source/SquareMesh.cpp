//-----------------------------------------------------------------------------
// File Name:    SquareMesh.cpp
// Author(s):    Kalby Jang
// Course:       GAM 200
// Copyright     "Copyright Symbol"  2020 DigiPen (USA) Corporation.
//-----------------------------------------------------------------------------

#include <GL/glew.h>


#include "SquareMesh.h"


// Private static variables initialization ------------------------------------
const glm::vec4 SquareMesh::vertices[4]
= {
    // Affine::Point(-1,-1),  // 0
    // Affine::Point(1,-1),   // 1
    // Affine::Point(1,1),    // 2
    // Affine::Point(-1,1)    // 3
    
    Affine::Point(-0.5,-0.5),  // 0
    Affine::Point(0.5,-0.5),   // 1
    Affine::Point(0.5,0.5),    // 2
    Affine::Point(-0.5,0.5)    // 3
};


const Mesh::Face SquareMesh::faces[2]
= { Face(0,1,2),  Face(0,2,3) };


const Mesh::Edge SquareMesh::edges[4]
= { Edge(0,1),  Edge(1,2),  Edge(2,3),  Edge(3,0) };



// Public Methods -------------------------------------------------------------
int SquareMesh::vertexCount() const
{
  return 4;
}

const glm::vec4* SquareMesh::vertexArray() const
{
  return vertices;
}

glm::vec4 SquareMesh::dimensions() const
{
  return Affine::Vector(2,2);
}

glm::vec4 SquareMesh::center() const
{
  return Affine::Point(0,0);
}

int SquareMesh::faceCount() const
{
  return 2;
}

const Mesh::Face* SquareMesh::faceArray() const
{
  return faces;
}

int SquareMesh::edgeCount() const
{
  return 4;
}

const Mesh::Edge* SquareMesh::edgeArray() const
{
  return edges;
}
