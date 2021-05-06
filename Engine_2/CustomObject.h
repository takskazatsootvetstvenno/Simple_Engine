#ifndef CUSTOMOBJECT_H
#define CUSTOMOBJECT_H
#include "TreeNode.h"
#include "mesh.h"
#include "Renderer.h"
#include <vector>
enum objectTypes {
    TRIANGLE,
    PLANE,
    CUBE,
    SIMPLE_OBJ  // only 1 rang massive models with 1 texture per mesh
};
class CustomObject {
   public:
    vector<Texture> textures_loaded;  // (оптимизация) сохраняем все загруженные текстуры, чтобы убедиться, что они не
                                      // загружены более одного раза
    vector<Mesh> meshes;
    CustomObject(string const& path, glm::vec3 position, objectTypes objectType, TreeNode& root,Renderer* rend,
                 glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0));
    void createNewObject(string const& path, glm::vec3 position, objectTypes objectType, TreeNode& root,
                         glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0));
    void Draw(Shader shader);
    unsigned int TextureFromFile(string const& path);

   private:
       void readMtlFile(string const& path,
           vector<Texture>& textures, string const& material_load, string const& name);
   Renderer* render;
    bool readObjFile(string const& path);
};
#endif CUSTOMOBJECT_H
