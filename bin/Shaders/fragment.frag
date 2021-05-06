#version 460
layout(location = 0) out vec4 FragColor;

layout(location = 0) in vec3 color;
layout(location = 1) in vec2 tex_coord;
layout(location = 2) in vec3 Normal;
layout(location = 3) in vec3 Position;
//layout(location = 4) in vec3 cameraPos;

uniform sampler2D u_texture_diffuse1;
//uniform sampler2D u_texture_diffuse2;
//uniform sampler2D u_texture_diffuse3;
//uniform sampler2D u_texture2;
//uniform vec3 cameraPos;
uniform samplerCube skybox;

void main() 
{
//	vec3 I = normalize(Position - cameraPos);
    //vec3 R = reflect(I, normalize(Normal));
    //FragColor = vec4(texture(skybox, R).rgb, 1.0);
	//FragColor = vec4(cameraPos,1.0);
	 //FragColor = vec4(texture(skybox, normalize(Normal)).rgb, 1.0);
	FragColor = texture(u_texture_diffuse1, tex_coord);//0.7*vec4(color,1.0) + texture(u_texture1, tex_coord);//, texture(u_texture2, tex_coord), 0.6);
};
