#include "Shader.h"

#include <GL/glew.h>

#include <gtc/type_ptr.hpp>

#include <fstream>
#include <iostream>
#include <vector>

Shader::Shader(const std::string& vertexShader, const std::string& fragmentShader)
    : vertShader(vertexShader), fragShader(fragmentShader) {
    unsigned int vertexShaderID;
    unsigned int fragmentShaderID;

    ID = glCreateProgram();
    /// Вершинный шейдер
    vertexShaderID = vertexShaderLoad(vertexShader);
    /// Фрагментный шейдер
    fragmentShaderID = fragmentShaderLoad(fragmentShader);

    glAttachShader(ID, vertexShaderID);
    glAttachShader(ID, fragmentShaderID);
    // glAttachShader(ShaderProgram, geometryShader);
    glLinkProgram(ID);

    int success;
    glGetProgramiv(ID, GL_LINK_STATUS, &success);

    char Info[256];
    memset(Info, 0, sizeof(Info));
    if (!success) {
        glGetProgramInfoLog(ID, 256, NULL, Info);
        std::cout << "Link program error:\n" << Info << std::endl;
    }

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
}
unsigned int Shader::vertexShaderLoad(const std::string& vertexPath) {
    char* vertex_shader_source;
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    std::ifstream in;
    in.open(vertexPath, std::ios::binary);
    if (in.is_open()) {
        int64_t size = in.seekg(0, std::ios::end).tellg();
        in.seekg(0);
        vertex_shader_source = new char[static_cast<int64_t>(size + 1)];
        memset(vertex_shader_source, 0, static_cast<int64_t>(size + 1) * sizeof(char));
        in.read(vertex_shader_source, size);
    } else {
        std::cout << "can't load vertex shaders\n";
    }
    in.close();

    glShaderSource(vertexShader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertexShader);

    int success;

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    char Info[256];
    memset(Info, 0, sizeof(Info));

    if (!success) {
        glGetShaderInfoLog(vertexShader, 256, NULL, Info);
        std::cout << "Vertex shader compilation error:\n" << Info << std::endl;
    }
    return vertexShader;
}
unsigned int Shader::fragmentShaderLoad(const std::string& fragmentPath) {
    char* fragment_shader_source;
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    std::ifstream in2;
    in2.open(fragmentPath, std::ios::binary);
    if (in2.is_open()) {
        int64_t size = in2.seekg(0, std::ios::end).tellg();
        in2.seekg(0);
        fragment_shader_source = new char[static_cast<int64_t>(size + 1)];
        memset(fragment_shader_source, 0, static_cast<int64_t>(size + 1) * sizeof(char));
        in2.read(fragment_shader_source, size);
    } else {
        std::cout << "can't load fragment shaders\n";
    }
    in2.close();

    glShaderSource(fragmentShader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragmentShader);

    int success;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    char Info[256];
    memset(Info, 0, sizeof(Info));
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 256, NULL, Info);
        std::cout << "Fragment shader compilation error:\n" << Info << std::endl;
    }
    return fragmentShader;
}
void Shader::setInt(const std::string& name,const int value) {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setUInt(const std::string& name,const uint32_t value) {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string& name,const float value) {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setVec3(const std::string& name,const glm::vec3& value) {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()),1, &value[0]);
}
void Shader::setMat4(const std::string& name,const glm::mat4& matrix) {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}
