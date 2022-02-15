#version 450 core
out vec4 FragColor;

in vec4 vertexColor;
in vec2 texCoord;

uniform sampler2D texture_;

void main()
{
	//FragColor = vertexColor;
    FragColor = texture(texture_, texCoord);
}