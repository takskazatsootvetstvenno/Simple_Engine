#ifndef SHADER_H
#define SHADER_H

#include <glm.hpp>

#include <string>
#include <iostream>
class Shader {
   public:
    unsigned int ID;
    unsigned int* UBO = nullptr;
    std::string vertShader = "Shaders/vertex.vert";
    std::string fragShader = "Shaders/fragment.frag";
    bool isUsed = false;
    Shader(const std::string& vertexShader = "Shaders/vertex.vert",
           const std::string& fragmentShader = "Shaders/fragment.frag");
    //set uniforms
    void setVec3(const std::string& name,const glm::vec3& value);
    void setInt(const std::string& name,const int value);
    void setUInt(const std::string& name,const uint32_t value);
    void setFloat(const std::string& name,const float value);
    void setMat4(const std::string& name,const glm::mat4& matrix);

   private:
    unsigned int vertexShaderLoad(const std::string& vertexPath);
    unsigned int fragmentShaderLoad(const std::string& fragmentPath);
};
#endif
