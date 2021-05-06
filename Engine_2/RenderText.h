#ifndef RENDER_TEXT_H
#define RENDER_TEXT_H

#include <glm.hpp>
#include "Shader.h"
#include <map>

struct Character {
    unsigned int TextureID; // ID �������� �����
    glm::ivec2 Size; // ������ �����
    glm::ivec2 Bearing; // �������� �� ����� ������ �� ��������/������ ���� �����
    unsigned int Advance; // �������� �� ���������� �����
};

class RenderText
{
public:
    std::map<char, Character> Characters;
    
    RenderText(unsigned int width, unsigned int height);
    ~RenderText();
    void Load(std::string font, unsigned int fontSize);
    void RenderTextOnScreen(std::string text, float x, float y, float scale, glm::vec3 color);
      Shader* TextShader;
private:
     std::string lastText;
   
     unsigned int VAO=-1, VBO=-1, UB0 = -1, TextColorUBO = -1;
     glm::vec3 lastColor = glm::vec3(1.0f,1.0f,1.0f);
};

#endif