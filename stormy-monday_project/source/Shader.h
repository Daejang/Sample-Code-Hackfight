//-----------------------------------------------------------------------------
// File Name:    Shader.h
// Author(s):    Kalby Jang
// Course:       GAM 200
// Copyright     "Copyright Symbol"  2020 DigiPen (USA) Corporation.
//-----------------------------------------------------------------------------
#pragma once
#include <string>

class Shader
{
public:
	Shader();
  //Shader(const std::string& vertex, const std::string& fragment);
	Shader(const char * vertexPath, const char * fragPath);

  void Use();
  void UnUse();

  void Unload();

  void SetBool (const std::string &variable, bool value) const;
  void SetInt  (const std::string &variable, int value) const;
  void SetFloat(const std::string &variable, float value) const;
  void SetVec2 (const std::string &variable, glm::vec2 value) const;
  void SetVec3 (const std::string &variable, glm::vec3 value) const;
  void SetVec4 (const std::string &variable, glm::vec4 value) const;
  void SetMat3 (const std::string &variable, glm::mat3 mat) const;
  void SetMat4 (const std::string &variable, glm::mat4 mat) const;

  void SetUniform(const GLchar * uniformName, GLint value);
  void SetUniform(const GLchar * uniformName, const glm::mat4 &matrix);

private:
  GLuint programID;

  void VertexRead(const char * vertexPath);
  void FragmentRead(const char * fragPath);
  void CompileAndLink();
  GLuint CompileVertex();
  GLuint CompileFrag();
  std::string vertexCode;
  std::string fragCode;
};
