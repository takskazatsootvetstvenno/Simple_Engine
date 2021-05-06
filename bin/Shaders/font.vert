#version 460 core
layout (location = 0) in vec4 vertex;
out vec2 TexCoords;
 
//uniform mat4 projection;

 layout (std140, binding = 2) uniform ProjMatrix
{
    mat4 u_projection;
};

void main()
{
    gl_Position = u_projection * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}  