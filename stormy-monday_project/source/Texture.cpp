//-----------------------------------------------------------------------------
// File Name:    Texture.cpp
// Author(s):    Kalby Jang
// Course:       GAM 200
// Copyright     "Copyright Symbol"  2020 DigiPen (USA) Corporation.
//-----------------------------------------------------------------------------
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <GL/glew.h>

#include <iostream>

#include "Texture.h"

//#include "Renderer.h"
#include "Debuggers.h"
#include "GLFWSystems.h"

Texture::Texture() : Texture( "" )
{
}

Texture::Texture(const std::string& filename) : _width(0), _height(0)
{
  imageData = NULL;
  RGBAchannels = 0;

  if (!filename.empty())
  {
    _filename = filename.c_str();
    LoadTexture(filename.c_str());
  }
}



Texture::~Texture()
{
  // freeing the data pointer
  UnloadTexture();
}

/*
GLuint Texture::GetGLTextureHandle()
{
  return _textureBuffer;
}
*/

GLuint Texture::GetTextureID()
{
	return textureID;
}

const char* Texture::FileName()
{
  return _filename;
}

const unsigned char* Texture::ImageData()
{
  return imageData;
}

const int Texture::RGBAChannels()
{
  return RGBAchannels;
}

const int Texture::Width() const
{
  return _width;
}

const int Texture::Height() const
{
  return _height;
}

void Texture::FileName( const char* filename )
{
  UnloadTexture();
  _filename = filename;
  LoadTexture(_filename);
}

void Texture::WidthAndHeight( const int width, const int height)
{
  _width = width;
  _height = height;
}



void Texture::LoadTexture(const char * filename)
{
	glCreateTextures(GL_TEXTURE_2D, 1, &textureID);
	PrintGLError("Texture load");

	// Texture Settings
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

		// set the texture wrapping parameters
	//glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	//glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glGenerateMipmap(GL_TEXTURE_2D);

	PrintGLError("Texture load");

	////////////////////////////////////////
	//
	////////////////////////////////////////
	unsigned char *data = stbi_load(filename, &u, &v, &nrChannels, 0);
	if (data)
	{
		glGetError();


		glTextureStorage2D(textureID, 3, GL_RGBA8, u, v);
		if (auto error = glGetError())
		{
			std::cout << "Error loading/creating texture: " << error << std::endl;
			//TraceLog::Error("glError on texture create: %i", error);
		}
		glTextureSubImage2D(textureID, 0, 0, 0, u, v, GL_RGBA, GL_UNSIGNED_BYTE, data);
		if (auto error = glGetError())
		{
			std::cout << "Error loading/creating texture: " << error << std::endl;
			//TraceLog::Error("glError on texture create: %i", error);
		}
		glBindTexture(GL_TEXTURE_2D, textureID);
		if (auto error = glGetError())
		{
			std::cout << "Error loading/creating texture: " << error << std::endl;
			//TraceLog::Error("glError on texture create: %i", error);
		}
		glGenerateMipmap(GL_TEXTURE_2D);

		if (auto error = glGetError())
		{
			std::cout << "Error loading/creating texture: " << error << std::endl;
			//TraceLog::Error("glError on texture create: %i", error);
		}
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

void /*Texture::*/LoadTexture_Original()
{/*

  stbi_set_flip_vertically_on_load(true);

  // Load image, create texture and generate mipmaps
  imageData = stbi_load(_filename, &_width, &_height, &RGBAchannels, STBI_rgb_alpha);
  
  

//    // Giving texture border
//    float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
//    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
//

  if (!imageData)
  {
    std::cout << "Failed to load Texture: " << _filename << std::endl;
    return;
  }

  //  // Generating the handler
  glGenTextures(1, &_textureBuffer);

  // Load to Renderer
  Renderer::LoadTexture(*this);

  // Free imageData
  stbi_image_free(imageData);

  PrintGLError("Texture");
  */
}

void Texture::UnloadTexture()
{
  //if (_textureBuffer)
   // glDeleteTextures(1, &_textureBuffer);

	if (textureID)
	{
		//GLFWSystem::Inst()->SetFullScreen(false);

		//std::cout << "Unloading texture" << textureID << std::endl;

		glDeleteTextures(1, &textureID);
	}
   
}

void Texture::AssignToRenderer()
{
}





