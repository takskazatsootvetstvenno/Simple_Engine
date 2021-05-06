#ifndef MESH_H
#define MESH_H

//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

#include <string>
#include <vector>
#include "Renderer.h"
using namespace std;

struct Vertex {
    glm::fvec3 Position;
    glm::fvec3 Normal;
    glm::fvec2 TexCoord;
    glm::fvec3 Tangent;
    // Вектор бинормали (вектор, перпендикулярный касательному вектору и вектору нормали)
    glm::fvec3 Bitangent;
};
/*
struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};*/

class Mesh {
   public:
   // unsigned int* skyBoxID;
    // Данные меша
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    
    string name;
    // Конструктор
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, Renderer* meshRender);

    // Рендеринг меша
    void Draw(Shader& shader);
    void setSkyBoxID(unsigned int* ID);
   private:
    // Данные для рендеринга
    MeshRenderInfo ForRender;
    Renderer* render= nullptr;
    // Инициализируем все буферные объекты/массивы
    void setupMesh();
};
#endif
