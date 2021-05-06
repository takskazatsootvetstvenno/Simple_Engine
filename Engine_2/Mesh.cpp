#include "Mesh.h"

Mesh::Mesh(vector<Vertex> vert, vector<unsigned int> ind, vector<Texture> tex, Renderer* meshRender)
:render(meshRender),vertices(vert),indices(ind),textures(tex){
    //this->vertices = vertices;
    //this->indices = indices;
    //this->textures = textures;
    // Теперь, когда у нас есть все необходимые данные, устанавливаем вершинные буферы и указатели атрибутов
    setupMesh();
}
void Mesh::setSkyBoxID(unsigned int* ID) {
    ForRender.skyBoxID = ID;
}
void Mesh::Draw(Shader& shader) {
    // Связываем соответствующие текстуры
    render->meshDraw(shader, textures, ForRender, indices.size());
    /*
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;
    
    for (unsigned int i = 0; i < textures.size(); i++) {
        // glActiveTexture(GL_TEXTURE0 + i); // перед связыванием активируем нужный текстурный юнит

        // Получаем номер текстуры (номер N в diffuse_textureN)
        string number;
        string name = textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);  // конвертируем unsigned int в строку
        else if (name == "texture_normal")
            number = std::to_string(normalNr++);  // конвертируем unsigned int в строку
        else if (name == "texture_height")
            number = std::to_string(heightNr++);  // конвертируем unsigned int в строку

        // Теперь устанавливаем сэмплер на нужный текстурный юнит
       // glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
        shader.setInt((name + number).c_str(), i);
        // и связываем текстуру
        // glBindTexture(GL_TEXTURE_2D, textures[i].id);
        glBindTextureUnit(i, textures[i].id);
    }
    if (textures.size() == 0) glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, *skyBoxID);
     glUniform1i(glGetUniformLocation(shader.ID, "skybox"), *skyBoxID);
    // Отрисовываем меш
    glBindVertexArray(ForRender.VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    */
}
void Mesh::setupMesh() {
    render->meshPresets(ForRender, vertices.size() * sizeof(Vertex),
        indices.size() * sizeof(unsigned int),
        sizeof(Vertex),
        offsetof(Vertex, Normal),
        offsetof(Vertex, TexCoord),
        vertices[0].Position.x,
        indices[0]);

    /*
    glCreateBuffers(1, &ForRender.VBO);
    glNamedBufferStorage(ForRender.VBO, vertices.size() * sizeof(Vertex), &vertices[0], GL_DYNAMIC_STORAGE_BIT);
    glCreateBuffers(1, &ForRender.EBO);
    glNamedBufferStorage(ForRender.EBO, indices.size() * sizeof(unsigned int), &indices[0], GL_DYNAMIC_STORAGE_BIT);
    glCreateVertexArrays(1, &ForRender.VAO);
    glVertexArrayVertexBuffer(ForRender.VAO, 0, ForRender.VBO, 0, sizeof(Vertex));
    glVertexArrayElementBuffer(ForRender.VAO, ForRender.EBO);

    glEnableVertexArrayAttrib(ForRender.VAO, 0);  // pos
    glEnableVertexArrayAttrib(ForRender.VAO, 1);  // norm
    glEnableVertexArrayAttrib(ForRender.VAO, 2);  // texcoord

    glVertexArrayAttribFormat(ForRender.VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribFormat(ForRender.VAO, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, Normal));
    glVertexArrayAttribFormat(ForRender.VAO, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, TexCoord));

    glVertexArrayAttribBinding(ForRender.VAO, 0, 0);
    glVertexArrayAttribBinding(ForRender.VAO, 1, 0);
    glVertexArrayAttribBinding(ForRender.VAO, 2, 0);

    glBindVertexArray(0);
    */
     /*
      // Создаем буферные объекты/массивы
      glGenVertexArrays(1, &VAO);
      glGenBuffers(1, &VBO);
      glGenBuffers(1, &EBO);

      glBindVertexArray(VAO);

      // Загружаем данные в вершинный буфер
      glBindBuffer(GL_ARRAY_BUFFER, VBO);

      // Самое замечательное в структурах то, что расположение в памяти их внутренних переменных является
    последовательным.
      // Смысл данного трюка в том, что мы можем просто передать указатель на структуру, и она прекрасно преобразуется в
    массив данных с элементами типа glm::vec3 (или glm::vec2), который затем будет преобразован в массив данных float,
    ну а в конце – в байтовый массив glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0],
    GL_STATIC_DRAW);

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

      // Устанавливаем указатели вершинных атрибутов

      // Координаты вершин
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

      // Нормали вершин
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

      // Текстурные координаты вершин
      glEnableVertexAttribArray(2);
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));

      // Касательный вектор вершины
     // glEnableVertexAttribArray(3);
     // glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

      // Вектор бинормали вершины
     // glEnableVertexAttribArray(4);
    //  glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
    */
    //      glDeleteBuffers(1, &VBO);
    //      glDeleteBuffers(1, &EBO);
}
