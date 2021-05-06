#version 460
layout(location = 0) out vec4 FragColor;

void main() 
{
	FragColor = vec4(1.0,0.0,1.0,1.0);//texture(u_texture_diffuse1, tex_coord);//0.7*vec4(color,1.0) + texture(u_texture1, tex_coord);//, texture(u_texture2, tex_coord), 0.6);
};
