#version 330
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;

out vec2 vTexCoord;
out vec4 vertexColor;

uniform mat4 transform;


void main()
{
  gl_Position = transform * vec4(position, 1.0);
  vertexColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
  vTexCoord = texcoord;
}