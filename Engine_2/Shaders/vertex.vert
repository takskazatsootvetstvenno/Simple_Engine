#version 460
layout(location = 0) in vec3 Pos;
layout(location = 1) in vec3 Normals;
layout(location = 2) in vec2 Texcoords;
layout(location = 3) in vec3 Color;

layout(location = 0) out vec3 ColorOut;
layout(location = 1) out vec2 texcoords_out;
layout(location = 2) out vec3 Normal;
layout(location = 3) out vec3 Position;
//layout(location = 4) out vec3 cameraPos;

//uniform mat4 u_view;
//uniform mat4 u_projection;

layout (std140, binding = 0) uniform Matrices
{
    mat4 u_projection;
    mat4 u_view;
	mat4 u_model;
};

//uniform mat4 u_model;
void main()
{
	ColorOut = Color;
	texcoords_out = Texcoords;
	Normal = mat3(transpose(inverse(u_model))) * Normals;
	Position = vec3(u_model * vec4(Pos, 1.0));
	gl_Position = u_projection * u_view * u_model * vec4(Pos.x, Pos.y, Pos.z, 1.0f);
};