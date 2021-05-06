#version 460
layout(location = 0) in vec2 Pos;
layout(location = 1) in vec2 Texcoords;

layout(location = 0) out vec2 texcoords_out;
void main()
{
	texcoords_out = Texcoords;
	gl_Position = vec4(Pos.x, Pos.y, 0.f, 1.0f);
};