//-----------------------------------------------------------------------------
// File Name:    TexturedSquareMesh.cpp
// Author(s):    Kalby Jang
// Course:       GAM 200
// Copyright     "Copyright Symbol"  2020 DigiPen (USA) Corporation.
//-----------------------------------------------------------------------------
#include "TexturedSquareMesh.h"

// Private static variables initialization ------------------------------------

TexturedSquareMesh::TexturedSquareMesh()
{
	texcoords[0] = Affine::Point(0, 0);
	texcoords[1] = Affine::Point(1, 0);
	texcoords[2] = Affine::Point(1, 1);
	texcoords[3] = Affine::Point(0, 1);
}

// Public Methods -------------------------------------------------------------
const glm::vec4* TexturedSquareMesh::texcoordArray() const
{
  return texcoords;
}

void TexturedSquareMesh::SetTextureCoordinates(float start_u, float start_v, float u, float v)
{
	/**************************************************/
	/*!
	    ------------------------------------------
		|\										 |
		|  \									 |
		|    \		goes this way				 |
		|	   \								 |
		|		 \								 |
		|		   \						     |
		|		   							     |
		|										 |
		|										 |
		|										 |
		|										 |
		|										 |
		|										 |
		------------------------------------------
    */
	/****************************************************/

	float left_top_corner_tex_square = 1.0f - start_v;
	float right_bottom_corner_tex_square = 1.0f - v;

	float v_len = abs(start_v - v);

	start_v = left_top_corner_tex_square - v_len;
	v = right_bottom_corner_tex_square + v_len;

	texcoords[0] = Affine::Point(start_u, start_v);
	texcoords[1] = Affine::Point(u, start_v);
	texcoords[2] = Affine::Point(u, v);
	texcoords[3] = Affine::Point(start_u, v);
}
