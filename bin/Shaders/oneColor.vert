#version 460
layout(location = 0) in vec2 Pos;
layout(location = 1) in vec4 colorIn;

layout(location = 0) out vec4 colorOut;
/* layout (std140, binding = 2) uniform ProjMatrix
{
    mat4 u_projection;
};
*/
  uniform mat4 u_projection;
void main()
{
	colorOut = colorIn;
	gl_Position = u_projection * vec4(Pos.x, Pos.y, 0.0f, 1.0f);
};