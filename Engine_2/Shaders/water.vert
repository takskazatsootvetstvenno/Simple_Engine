#version 460
layout(location = 0) in vec2 Pos;
layout(location = 1) in vec2 texCoords;
layout(location = 2) in vec4 colorIn;


layout(location = 0) out vec4 colorOut;
layout(location = 1) out vec2 textureCoords;
layout(location = 2) out vec3 cameraPos;
layout(location = 3) out vec3 worldPos;
layout(location = 4) out vec4 TEMP_VEC_TEST;
layout(location = 5) out mat4 viewMat;


layout (std140, binding = 0) uniform Matrices
{
    uniform mat4 u_projection;
    uniform mat4 u_view;
	uniform mat4 u_model;
};
void main()
{
	colorOut = colorIn;
	cameraPos = vec3(u_view[3][2],u_view[3][1],u_view[3][0]);
	worldPos = vec3(Pos.x, 0.0f, Pos.y);
	viewMat = inverse(u_view);
	textureCoords = texCoords;//vec2(Pos.x/2.0 + 0.5, Pos.y/2.0 + 0.5);
	  
	float x=1.0,y=2.0,z=3.0;
	float phi = acos(x/sqrt(x*x+y*y));
	float teta = acos(sqrt(x*x+y*y)/sqrt(x*x+y*y+z*z));
	//0  4  8  12
	//1  5  9  13
	//2  6  10 14
	//3  7  11 15
	mat4 rotAroundZ = mat4(cos(phi),sin(phi),0,0,
						   -sin(phi),cos(phi), 0,0,
						   0,       0,        1,0,
						   0,		0,		  0,1);

	mat4 rotAroundX = mat4(1,       0,        0,0,
						   0,cos(phi), sin(phi),0,
						   0,-sin(phi), cos(phi),0,
						   0,		0,		  0,1);
	vec4 vector=vec4(x,y,z,1);
	TEMP_VEC_TEST = rotAroundZ*vector;

	gl_Position = u_projection *u_view * vec4(Pos.x, 0.1f, Pos.y, 1.0f);

};