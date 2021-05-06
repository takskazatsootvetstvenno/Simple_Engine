#include "RenderText.h"

#include <gtc/type_ptr.hpp>

#include <GL/glew.h>
#include <ft2build.h>

#include <iostream>
#include FT_FREETYPE_H

RenderText::RenderText(unsigned int width, unsigned int height) {
    TextShader = new Shader("Shaders/font.vert", "Shaders/font.frag");
    glUseProgram(TextShader->ID);
    glm::mat4 proj = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height));
    // TextShader->setMat4("projection", glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height)));
    TextShader->setInt("text", 0);

    TextShader->UBO = &UB0;
    glGenBuffers(1, TextShader->UBO);
    glBindBuffer(GL_UNIFORM_BUFFER, *(TextShader->UBO));
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBufferRange(GL_UNIFORM_BUFFER, 2, *(TextShader->UBO), 0,
                      sizeof(glm::mat4));  /// GLOBAL BINDING 2 - PROJ.MATRIX

    glBindBuffer(GL_UNIFORM_BUFFER, *(TextShader->UBO));
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(proj));
    glBindBuffer(GL_UNIFORM_BUFFER, 0); 

    glGenBuffers(1, &TextColorUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, TextColorUBO);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec3), NULL, GL_DYNAMIC_DRAW);
    glBindBufferRange(GL_UNIFORM_BUFFER, 3, TextColorUBO, 0,
                     sizeof(glm::vec3));  /// GLOBAL BINDING 3 - FONT COLOR

   // glBindBuffer(GL_UNIFORM_BUFFER, TextColorUBO);
   // glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec3), glm::value_ptr(gl));
   // glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glCreateBuffers(1, &VBO);
    glNamedBufferStorage(VBO, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_STORAGE_BIT); //vec4 - 6 vertex for rectangle
    glCreateVertexArrays(1, &VAO);
    glEnableVertexArrayAttrib(VAO, 0);
    glVertexArrayVertexBuffer(VAO, 0, VBO, 0, 4 * sizeof(float));
    glVertexArrayAttribFormat(VAO, 0, 4, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(VAO, 0, 0);
    glBindVertexArray(0);
}
void RenderText::Load(std::string font, unsigned int fontSize) {
    // Сначала очищаем ранее загруженные символы
    this->Characters.clear();

    // Затем инициализируем и загружаем библиотеку FreeType
    FT_Library ft;
    if (FT_Init_FreeType(&ft))  // все функции в случае ошибки возвращают значение, отличное от 0
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

    // Загрузка шрифта в качестве face
    FT_Face face;
    if (FT_New_Face(ft, font.c_str(), 0, &face)) std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

    // Устанавливаем размер загруженных глифов
    FT_Set_Pixel_Sizes(face, 0, fontSize);

    // Отключаем ограничение на выравнивание байтов
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Затем предварительно загружаем/компилируем символы шрифта и сохраняем их
    for (GLubyte c = 0; c < 255; c++) {
        // Загрузка символа глифа
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }

        // Генерация текстуры
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED,
                     GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

        // Установка параметров текстур
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Теперь сохраняем символы для их дальнейшего использования
        Character character = {texture, glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                               glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top), face->glyph->advance.x};
        Characters.insert(std::pair<char, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    // Как закончили, очищаем ресурсы FreeType
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}
void RenderText::RenderTextOnScreen(std::string text, float x, float y, float scale, glm::vec3 color) {
    // Активируем соответствующее состояние рендеринга
    glUseProgram(TextShader->ID);
    //TextShader->setVec3("textColor", color);
    if (lastColor != color) {
        glBindBuffer(GL_UNIFORM_BUFFER, TextColorUBO);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec3), glm::value_ptr(color));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        lastColor = color;
    }
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // Перебираем все символы
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
        
        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        // Обновляем VBO для каждого символа
        float vertices[6][4] = {
            {xpos, ypos + h, 0.0f, 0.0f}, {xpos, ypos, 0.0f, 1.0f},     {xpos + w, ypos, 1.0f, 1.0f},

            {xpos, ypos + h, 0.0f, 0.0f}, {xpos + w, ypos, 1.0f, 1.0f}, {xpos + w, ypos + h, 1.0f, 0.0f}};

        // Визуализируем текстуру глифа поверх прямоугольника
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);

        // Обновляем содержимое памяти VBO
        glNamedBufferSubData(VBO, 0, sizeof(vertices), vertices);

        // Рендерим прямоугольник
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Теперь производим смещение для отображения следующего глифа (обратите внимание, что данное смещение
        // измеряется в единицах, составляющих 1/64 пикселя)
        x += (ch.Advance >> 6) * scale;  // побитовый сдвиг на 6, чтобы получить значение в пикселях (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
RenderText::~RenderText()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &UB0);
    glDeleteBuffers(1, &TextColorUBO);
    delete TextShader;
}