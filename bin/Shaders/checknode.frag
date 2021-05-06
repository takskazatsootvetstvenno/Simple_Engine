#version 460
layout(location = 0) out vec4 FragColor;

uniform float u_nodeColor1;
uniform float u_nodeColor2;
uniform float u_nodeColor3;
void main() 
{
	FragColor = vec4(u_nodeColor1/256.0, u_nodeColor2/256.0, u_nodeColor3/256.0, 1.0);
};