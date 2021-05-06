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
    // ������ ��������� (������, ���������������� ������������ ������� � ������� �������)
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
    // ������ ����
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    
    string name;
    // �����������
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, Renderer* meshRender);

    // ��������� ����
    void Draw(Shader& shader);
    void setSkyBoxID(unsigned int* ID);
   private:
    // ������ ��� ����������
    MeshRenderInfo ForRender;
    Renderer* render= nullptr;
    // �������������� ��� �������� �������/�������
    void setupMesh();
};
#endif
