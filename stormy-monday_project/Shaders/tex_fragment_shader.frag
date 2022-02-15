#version 330
out vec4 frag_color;
out vec4 debugFragColor;

in vec2 vTexCoord;

uniform sampler2D texture_;
uniform vec4 color;
uniform vec4 debugColor;


void main(void)
{
  frag_color = texture(texture_, vTexCoord);
  //debugFragColor = debugColor;
}