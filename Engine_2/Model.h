#ifndef MODEL_H
#define MODEL_H

//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "TreeNode.h"
#include "mesh.h"

#include <assimp/Importer.hpp>

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "Renderer.h"
//#include <map>
#include <vector>
using namespace std;

unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);

class Model {
   public:
    // ������ ������
    vector<Texture> textures_loaded;  // (�����������) ��������� ��� ����������� ��������, ����� ���������, ��� ��� ��
                                      // ��������� ����� ������ ����
    vector<Mesh> meshes;
    string directory;
    bool gammaCorrection;
    TreeNode& root;
    vector<Shader>& model_shaders;
    // ����������� � �������� ��������� ���������� ���� � 3D-������
    Model(string const& path, TreeNode& root, vector<Shader>& shaders, Renderer* modelRender, bool gamma = false)
        : gammaCorrection(gamma), root(root), model_shaders(shaders), render(modelRender) {
        loadModel(path);
    }
    // ������������ ������, � ������ � ��� � ����
    void Draw(Shader shader) {
        for (unsigned int i = 0; i < meshes.size(); i++) meshes[i].Draw(shader);
    }

   private:
       Renderer* render;
    // ��������� ������ � ������� Assimp � ��������� ���������� ���� � ������� meshes
    void loadModel(string const& path);
    unsigned int TextureFromFile(const char* path, const string& directory);
    // ����������� ��������� ����. ������������ ������ ��������� ���, ������������� � ����, � ��������� ���� ������� ���
    // ����� �������� ����� (���� ������ ������ �������)
    void processNode(aiNode* node, TreeNode&, const aiScene* scene);

    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    // ��������� ��� �������� ���������� ��������� ���� � �������� ��������, ���� ��� ��� �� ���� ���������.
    // ����������� ���������� ������������ � ���� ��������� Texture
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};

#endif
