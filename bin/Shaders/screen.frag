#version 460
layout(location = 0) out vec4 FragColor;

layout(location = 0) in vec2 texCoord;

uniform sampler2D u_screenTexture;

void main() 
{
	FragColor = vec4(texture(u_screenTexture, texCoord).rgb,0.8);
};