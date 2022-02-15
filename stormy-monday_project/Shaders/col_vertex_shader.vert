#version 330
layout (location = 0) in vec3 position;

out vec4 vertexColor;

uniform mat4 transform;

uniform float red = 0.0f;
uniform float green = 1.0f;
uniform float blue = 1.0f;
uniform float alpha = 1.0f;

void main()
{
  gl_Position = transform * vec4(position, 1.0);
  vertexColor = vec4(red, green, blue, alpha);
}