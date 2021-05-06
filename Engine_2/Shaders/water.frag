#version 460
layout(location = 0) in vec4 colorIn;
layout(location = 1) in vec2 textureCoords;
layout(location = 2) in vec3 cameraPos;
layout(location = 3) in vec3 worldPos;
layout(location = 4) in vec4 TEMP_VEC_TEST;
layout(location = 5) in mat4 viewMAT;


layout(location = 0) out vec4 FragColor;
uniform sampler2D DUDV;
uniform sampler2D u_water;
uniform samplerCube skybox;
uniform vec3 u_cameraPosition;
uniform float moveFactor;
const float waweStrength = 0.04;
const vec3 norm = vec3(0.0f,1.0f,0.0f);
float getModul(vec3 vec)
{
	return sqrt(vec.x*vec.x + vec.y*vec.y +vec.z*vec.z);
}
void main()
{
 float gamma = 2.2;
    vec2 distortion1 = (texture(DUDV, vec2(textureCoords.x+moveFactor, textureCoords.y)).rg*2.0-1.0)*waweStrength;
	vec2 distortion2 = (texture(DUDV, vec2(textureCoords.x, textureCoords.y+moveFactor)).rg*2.0-1.0)*waweStrength;

	vec2 distortionALL = distortion1+ distortion2;
	vec2 newTexCoords = textureCoords;
	vec3 I = normalize(worldPos - u_cameraPosition);
	//vec3 I = normalize(vec3(viewMAT*vec4(worldPos,1.0)));
	vec3 ref = (reflect(I,norm));
	newTexCoords += distortionALL;
	//ref+=newTexCoords;
	vec4 finalColor;
	vec4 WaterTextureColor=pow(texture(u_water, newTexCoords), vec4(gamma));
	vec4 SkyboxTextureColor=pow(texture(skybox, ref), vec4(gamma));
	finalColor = mix(SkyboxTextureColor,WaterTextureColor,abs(dot(I,norm)/(getModul(I)*getModul(norm))));
	
	FragColor =pow(finalColor,vec4(1.0/gamma));//0.7*vec4(color,1.0) + texture(u_texture1, tex_coord);//, texture(u_texture2, tex_coord), 0.6);

	//mix(texture(skybox,ref),texture(u_water, newTexCoords),
	//normalize(dot(I,norm)/(getModul(I)*getModul(norm))));// + texture(skybox,ref);
	//normalize(dot(I,norm)/(getModul(I)*getModul(norm)))*texture(skybox,ref);
};