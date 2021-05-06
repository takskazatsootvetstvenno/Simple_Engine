#version 460 core
in vec2 TexCoords;
out vec4 color;
 
uniform sampler2D text;

//uniform vec3 u_textColor;
  layout (std140, binding = 3) uniform textColorBlock
{
    vec3 u_textColor;
};

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    color = vec4(u_textColor, 1.0) * sampled;
}