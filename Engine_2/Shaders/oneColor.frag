#version 460
layout(location = 0) in vec4 colorIn;

layout(location = 0) out vec4 FragColor;
void main()
{
	FragColor = vec4(colorIn);
};