//-----------------------------------------------------------------------------
// File Name:    Shader.cpp
// Author(s):    Kalby Jang
// Course:       GAM 200
// Copyright     "Copyright Symbol"  2020 DigiPen (USA) Corporation.
//-----------------------------------------------------------------------------
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>


Shader::Shader()
{
	programID = 0;
	vertexCode = '\0';
	fragCode = '\0';
}

//Shader::Shader( const std::string& vertex, const std::string& fragment )
Shader::Shader(const char * vertexPath, const char * fragPath)
{
	//Create the shader program and error check it
	programID = glCreateProgram();
	if (!programID)
	{
		//std::cout << "Failed to create shader program" << std::endl;
		//TraceLog::Error("Shader: Failed to create shader program");
		return;
	}

	//Reads the vertex shader from a file
	VertexRead(vertexPath);
	//Reads the fragment shader from a file
	FragmentRead(fragPath);
	//Compiles the shaders and links them to the shader program
	CompileAndLink();

}


void Shader::VertexRead(const char * vertexPath)
{
	std::ifstream vtxShaderFile;
	//Open the file to read from and error check if it opened properly
	vtxShaderFile.open(vertexPath);
	if (!(vtxShaderFile.is_open()))
	{
		//std::cout << "Failed to open file: " << vertexPath << std::endl;
		//TraceLog::Error("Shader: Failed to open vertex shader file: %s", vertexPath);
		return;
	}

	//Create a buffer to store the read in file
	std::stringstream vtxShaderStream;
	vtxShaderStream << vtxShaderFile.rdbuf();
	vtxShaderFile.close();

	//Input the buffer stream into an actual string for use
	vertexCode = vtxShaderStream.str();
}

void Shader::FragmentRead(const char * fragPath)
{
	std::ifstream fragShaderFile;
	//Open the file to read from and error check if it opened properly
	fragShaderFile.open(fragPath);
	if (!(fragShaderFile.is_open()))
	{
		//Prints error message and exits since file failed to open
		//std::cout << "Failed to open file: " << fragPath << std::endl;
		//TraceLog::Error("Shader: Failed to open fragment shader file: %s", fragPath);
		return;
	}

	//Create a buffer to store the read in file
	std::stringstream fragShaderStream;
	fragShaderStream << fragShaderFile.rdbuf();
	fragShaderFile.close();

	//Input the buffer stream into an actual string for use
	fragCode = fragShaderStream.str();
}


void Shader::CompileAndLink()
{
	GLuint vertexShaderID = CompileVertex();
	if (!vertexShaderID)
	{
		return;
	}
	GLuint fragmentShaderID = CompileFrag();
	if (!fragmentShaderID)
	{
		return;
	}

	//Now that the vertex and fragment shaders have compiled, added them to the shader program
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);

	//Now, link the shader program and error check
	glLinkProgram(programID);

	//Error check for if shader program linked
	GLint success;
	GLchar infoLog[1024] = { 0 };
	//glLinkProgram(programID);
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(programID, sizeof(infoLog), NULL, infoLog);
		//std::cout << "Error linking shader program: " << shaderProgramLog << std::endl;
		//TraceLog::Error("Shader: Error linking shader program: %s", shaderProgramLog);
		return;
	}

	// Clean up
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

}


GLuint Shader::CompileVertex()
{
	//Convert c++ string into c-style string for opengl functions
	const char* vertChar = vertexCode.c_str();

	//Create vertex shader and error checks if it was created properly
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	if (!vertexShaderID)
	{
		std::cout << "Failed to create vertex shader" << std::endl;
		//TraceLog::Error("Shader: Failed to create vertex shader");
		return NULL;
	}

	//Compile vertex shader and error check
	glShaderSource(vertexShaderID, 1, &vertChar, NULL);
	glCompileShader(vertexShaderID);


	//Error check to see if vertex shader compiled
	GLint success;
	GLchar infoLog[512] = { 0 };
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShaderID, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR: VERTEX COMPILATION FAILED\n" << infoLog << std::endl;
		return NULL;
	}

	return vertexShaderID;
}

GLuint Shader::CompileFrag()
{
	//Convert c++ string into c-style string for opengl functions
	const char* fragChar = fragCode.c_str();

	//Create fragment shader and error checks if it was created properly
	int fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	if (!fragmentShaderID)
	{
		std::cout << "Failed to create vertex shader" << std::endl;
		//TraceLog::Error("Shader: Failed to create vertex shader");
		return NULL;
	}

	//Compile fragment shader and error check
	glShaderSource(fragmentShaderID, 1, &fragChar, NULL);
	glCompileShader(fragmentShaderID);

	//Error check to see if fragment shader compiled
	GLint success;
	GLchar infoLog[512] = { 0 };
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderID, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR: FRAGMENT COMPILATION FAILED\n" << infoLog << std::endl;
		return NULL;
	}

	return fragmentShaderID;
}



void Shader::Use()
{
  glUseProgram(programID);
}

void Shader::UnUse()
{
	glUseProgram(0);
}


void Shader::Unload()
{
	glDeleteProgram(programID);
}



void Shader::SetBool( const std::string& variable, bool value ) const
{
  glUniform1i( glGetUniformLocation( programID, variable.c_str() ), (int)value) ;
}

void Shader::SetInt( const std::string& variable, int value ) const
{
  glUniform1i( glGetUniformLocation( programID, variable.c_str() ), value );
}

void Shader::SetFloat( const std::string& variable, float value ) const
{
  glUniform1f( glGetUniformLocation( programID, variable.c_str() ), value );
}

void Shader::SetVec2( const std::string& variable, glm::vec2 value ) const
{
  glUniform2f( glGetUniformLocation( programID, variable.c_str() ), value.x, value.y );
}

void Shader::SetVec3( const std::string& variable, glm::vec3 value ) const
{
  glUniform3f( glGetUniformLocation( programID, variable.c_str() ), value.x, value.y, value.z );
}

void Shader::SetVec4( const std::string& variable, glm::vec4 value ) const
{
  glUniform4f( glGetUniformLocation( programID, variable.c_str() ), value.x, value.y, value.z, value.w );
}

void Shader::SetMat3( const std::string& variable, glm::mat3 mat ) const
{
  glUniformMatrix3fv( glGetUniformLocation( programID, variable.c_str() ), 1, GL_FALSE, glm::value_ptr(mat) );
}

void Shader::SetMat4( const std::string& variable, glm::mat4 mat ) const
{
  glUniformMatrix4fv( glGetUniformLocation(programID, variable.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}


void Shader::SetUniform(const GLchar * uniformName, GLint value)
{
	GLint location = glGetUniformLocation(programID, uniformName);
	if (location >= 0)
	{
		glUniform1i(location, value);
	}
	else
	{
		//Error Message for failing to set uniform/for uniform not existing
		//TraceLog::Error("Shader: ERROR - Failed to set uniform / uniform does not exist [%s]", uniformName);
	}
}

void Shader::SetUniform(const GLchar * uniformName, const glm::mat4 &matrix)
{
	GLint location = glGetUniformLocation(programID, uniformName);
	if (location >= 0)
	{
		glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(matrix));
	}
	else
	{
		//Error Message for failing to set uniform/for uniform not existing
		//TraceLog::Error("Shader: ERROR - Failed to set uniform / uniform does not exist [%s]", uniformName);
	}
}

