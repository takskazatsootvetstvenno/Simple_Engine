#version 460 core
layout (location = 0) in vec3 aPos;

//out vec3 Normal;
//out vec3 Position;
out vec3 TexCoords;

//uniform mat4 view;
//uniform mat4 projection;

layout (std140, binding = 1) uniform Matrices
{
    mat4 u_projection;
    mat4 u_view;
};
void main()
{
    TexCoords = aPos;
    vec4 pos = u_projection * u_view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}