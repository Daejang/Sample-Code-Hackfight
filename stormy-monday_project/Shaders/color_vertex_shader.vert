#version 450 core
layout (location = 0) in vec3 aPos;

out vec4 vertexColor;

uniform mat4 transform;

uniform mat4 camera;

// Shrinkig the screen to NDC
uniform mat4 world_to_ndc;


uniform float red = 0.0f;
uniform float green = 1.0f;
uniform float blue = 0.0f;
uniform float alpha = 1.0f;

void main()
{
  gl_Position = world_to_ndc * camera * transform * vec4(aPos, 1.0);
  vertexColor = vec4(red, green, blue, alpha);
}