#ifndef TREE_NODE_H
#define TREE_NODE_H
#include "Mesh.h"
#include "Shader.h"
#include "Renderer.h"

#include <iostream>
#include <vector>

enum NodeProp { DEFAULTPROP = 0x00, HIGHLIGHT = 0x01, HIDE = 0x02, CHOOSECOLORID = 0x04, TRANSPARENT = 0x08 };

using namespace std;

class TreeNode {
   public:
    std::string name;
    glm::mat4 localMatrix = glm::mat4(1.0);
    std::vector<Mesh> meshes;
    std::vector<TreeNode*> children;
    std::vector<Shader> shaders;

    // TreeNode destructor. Also the destructor goes to the parent and calls checkChildren(this)
    ~TreeNode();
    
    // TreeNode constructor.
    TreeNode(vector<Shader>& initShaders, Renderer* initTreeRender);

    // return child with id
    TreeNode* getChild(unsigned int ind);

    // return parent
    TreeNode* getParent();

    // return current id
    float getId();

    // adds new Child to the end
    void addChild(TreeNode* Child);

    // check all children and swap parametr child and a last child. After that a last child will be destroyed
    void checkChildren(TreeNode* childDel);

    // adds new Mesh to the end1
    void addMesh(Mesh meshNew);

    void setProp(char new_prop);

    void setProjection(glm::mat4* newLocalProjection);

    void setView(glm::mat4* newLocalView);

    // draws all meshes in this Node and calls useMeshesRec() in children
    void useMeshesRec(glm::mat4 model);

    void shaderRewriteRec(vector<Shader>& init_shaders);

    void setSkyboxID(unsigned int* id);

    void giveUniqIdRec(float id);

    void SetPropByID(float id, char prop);

   private:
    glm::mat4* localProjection;
    glm::mat4* localView;
    float id = 0;
    int currentShader = -1;
    unsigned int* skyboxID = nullptr;
    TreeNode* parent = nullptr;
    char prop = 0x00;
    Renderer* treeRender;
};
#endif TREE_NODE_H
