//-----------------------------------------------------------------------------
// File Name:    Mesh.h
// Author(s):    Kalby Jang
// Course:       GAM 200
// Copyright     "Copyright Symbol"  2020 DigiPen (USA) Corporation.
//-----------------------------------------------------------------------------
#pragma once

#include <GL/glew.h> //!< for access to OpenGL API declarations 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>


typedef class Shader Shader;


typedef class Texture Texture;


class Mesh 
{
public:
	Mesh() {};
	~Mesh() {};
	//Mesh(const Mesh * other) { *this = *other; }



	static Mesh * GetTexturedMesh();
	static Mesh * GetSquareLoopMesh();
	static Mesh * GetCircleLoopMesh();
	static Mesh * GetLineMesh();


	static void Draw(Mesh * pMesh, Shader shader/*, const Color * color*/);


	void RebindVertexBuffer();
	void RebindTexCoords();

	std::vector<glm::vec2> vertices;
	std::vector<glm::vec2> texCoords; //!< (u, v)



	
  struct Face {
    unsigned index1, index2, index3;
    Face(int i = 0, int j = 0, int k = 0)
      : index1(i), index2(j), index3(k) {}
  };

  struct Edge {
    unsigned index1, index2;
    Edge(int i = 0, int j = 0)
      : index1(i), index2(j) {}
  };

  /*
  virtual ~Mesh(void) {}
  //virtual int vertexCount(void) const = 0;
  virtual const glm::vec4* vertexArray(void) const = 0;
  virtual glm::vec4 dimensions(void) const = 0;
  virtual glm::vec4 center(void) const = 0;
  virtual int faceCount(void) const = 0;
  virtual const Face* faceArray(void) const = 0;
  virtual int edgeCount(void) const = 0;
  virtual const Edge* edgeArray(void) const = 0;

  */

private:
	GLuint vao = 0,
		vertex_buffer = 0,
		texcoord_buffer = 0;
	bool isTextured = false;
	
	inline static Mesh * tMesh  = nullptr;
	inline static Mesh * sqMesh = nullptr;
	inline static Mesh * crMesh = nullptr;
	inline static Mesh * lnMesh = nullptr;

	GLenum primitiveType = GL_DONT_CARE;
	GLsizei vertexCount  = 0;

	static Mesh * CreateTexturedQuad(std::vector<glm::vec2> vertices, /*std::vector<Texture> textures,*/
						      std::vector<glm::vec2> texCoords);
	static Mesh * CreateColoredQuad(std::vector<glm::vec2> vertices);

	void SetupMesh();

	void BindVertexArray();
	void UnBindVertexArray();




};
