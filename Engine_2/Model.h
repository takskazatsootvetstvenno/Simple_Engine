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
    // ƒанные модели
    vector<Texture> textures_loaded;  // (оптимизаци€) сохран€ем все загруженные текстуры, чтобы убедитьс€, что они не
                                      // загружены более одного раза
    vector<Mesh> meshes;
    string directory;
    bool gammaCorrection;
    TreeNode& root;
    vector<Shader>& model_shaders;
    //  онструктор в качестве аргумента использует путь к 3D-модели
    Model(string const& path, TreeNode& root, vector<Shader>& shaders, Renderer* modelRender, bool gamma = false)
        : gammaCorrection(gamma), root(root), model_shaders(shaders), render(modelRender) {
        loadModel(path);
    }
    // ќтрисовываем модель, а значит и все еЄ меши
    void Draw(Shader shader) {
        for (unsigned int i = 0; i < meshes.size(); i++) meshes[i].Draw(shader);
    }

   private:
       Renderer* render;
    // «агружаем модель с помощью Assimp и сохран€ем полученные меши в векторе meshes
    void loadModel(string const& path);
    unsigned int TextureFromFile(const char* path, const string& directory);
    // –екурсивна€ обработка узла. ќбрабатываем каждый отдельный меш, расположенный в узле, и повтор€ем этот процесс дл€
    // своих дочерних углов (если таковы вообще имеютс€)
    void processNode(aiNode* node, TreeNode&, const aiScene* scene);

    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    // ѕровер€ем все текстуры материалов заданного типа и загружам текстуры, если они еще не были загружены.
    // Ќеобходима€ информаци€ возвращаетс€ в виде структуры Texture
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};

#endif
