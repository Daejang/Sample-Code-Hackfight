//-----------------------------------------------------------------------------
// File Name:    Texture.h
// Author(s):    Kalby Jang
// Course:       GAM 200
// Copyright     "Copyright Symbol"  2020 DigiPen (USA) Corporation.
//-----------------------------------------------------------------------------
#pragma once
#include <string>

#include <GL/glew.h>
//-----------------------------------------------------------------------------
// Forward References
//-----------------------------------------------------------------------------
typedef class Texture *TexturePtr;

class Texture
{
public:
  Texture();
  Texture(const std::string& filename);
//  Texture(const std::string& filename, int width, int height);
  ~Texture();


  // Getters-------------------------------------------------------------------
  GLuint GetGLTextureHandle();
  GLuint GetTextureID();
  const char* FileName();
  const unsigned char* ImageData();
  const int RGBAChannels();
  const int Width() const;
  const int Height() const;

  // Setters-------------------------------------------------------------------
	void FileName(const char* filename);
	void WidthAndHeight(const int, const int);


private:

  //GLuint _textureBuffer;
  GLuint textureID;
  const char* _filename = "";
  unsigned char* imageData;
  int RGBAchannels;
  int _width;
  int _height;

  int u;
  int v;
  int nrChannels;


  // Private ------------------------------------------------------------------
  void LoadTexture(const char * filename);
  void UnloadTexture();
  void AssignToRenderer();

};