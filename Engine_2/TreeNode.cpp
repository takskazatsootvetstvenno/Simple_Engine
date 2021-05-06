#include "TreeNode.h"

#include <gtc/type_ptr.hpp>
TreeNode::~TreeNode() {
    if (parent != nullptr) parent->checkChildren(this);
    for (auto& child : children) delete child;

    children.clear();
    meshes.clear();
    std::cout << "Deleted: " << name << endl;
}
TreeNode* TreeNode::getChild(unsigned int ind) {
    if (ind < children.size()) { return children[ind]; }
    return nullptr;
}
float TreeNode::getId() {
    return id;
}
void TreeNode::setSkyboxID(unsigned int* id) {
    skyboxID = id;
    for (auto& child : children) child->setSkyboxID(id);
    for (auto& mesh : meshes) mesh.setSkyBoxID(id);
}
TreeNode::TreeNode(vector<Shader>& initShaders, Renderer* initTreeRender) : treeRender(initTreeRender) {
    for (auto& shader : initShaders) shaders.push_back(shader);
}
void TreeNode::shaderRewriteRec(vector<Shader>& init_shaders) {  // also change skybox
    shaders.clear();
    for (auto& shader : init_shaders) shaders.push_back(shader);

    for (auto& child : children) child->shaderRewriteRec(init_shaders);
}
TreeNode* TreeNode::getParent() {
    return parent;
}
void TreeNode::addChild(TreeNode* Child) {
    Child->parent = this;
    children.push_back(Child);
}
void TreeNode::checkChildren(TreeNode* childDel) {
    for (int i = 0; i < children.size(); i++)
        if (children[i] == childDel) {
            children[i] = children[children.size() - 1];
            children.pop_back();
        }
}
void TreeNode::addMesh(Mesh meshNew) {
    meshes.push_back(meshNew);
}
void TreeNode::useMeshesRec(glm::mat4 model) {
    glm::mat4 new_local = model * localMatrix;

    switch (prop) {
        case 0:
            currentShader = DEFAULT_SHADER;
            break;
        case HIGHLIGHT:
            currentShader = HIGHLIGHT_SHADER;  // Highlight the node
            break;
        case CHOOSECOLORID:
            currentShader = CHOOSECOLOR_SHADER;  // For chosing node for id
            break;
        default:
            break;
    }
    /*
   // (shaders[currentShader].ID);

    glBindBuffer(GL_UNIFORM_BUFFER, *shaders[currentShader].UBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(new_local));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    */
    treeRender->UseProgram(shaders[currentShader].ID);
    treeRender->updateModelMatrix(shaders[currentShader], new_local);

    if (prop & CHOOSECOLORID)  //Только поиска по цвету, checknode.frag
    {
        float temp1;
        temp1 = (float)((int)id % 16777216) / 65536;
        shaders[CHOOSECOLOR_SHADER].setFloat("u_nodeColor1", temp1);
        float temp2;
        temp2 = (float)((int)id % 65536) / 256;
        shaders[CHOOSECOLOR_SHADER].setFloat("u_nodeColor2", temp2);
        float temp3;
        temp3 = (float)((int)id % 256);
        shaders[CHOOSECOLOR_SHADER].setFloat("u_nodeColor3", id);
        // shaders[CHOOSECOLOR_SHADER].setFloat("u_nodeColor3", id);
    }

    for (auto& mesh : meshes) mesh.Draw(shaders[currentShader]);
    for (auto& child : children) child->useMeshesRec(new_local);
    treeRender->UseProgram(0);
}
void TreeNode::setProp(char new_prop) {
    prop = new_prop;
    for (auto& child : children) child->setProp(prop);
}
void TreeNode::setProjection(glm::mat4* newLocalProjection) {
    localProjection = newLocalProjection;
}
void TreeNode::setView(glm::mat4* newLocalView) {
    localView = newLocalView;
}
void TreeNode::giveUniqIdRec(float id) {
    this->id = id;
    for (auto& child : children) {
        id += 1;
        child->giveUniqIdRec(id);
    }
}
void TreeNode::SetPropByID(float id, char prop) {
    if (this->id == id) {
        setProp(prop);
    } else
        this->prop = DEFAULTPROP;
    for (auto& child : children) { child->SetPropByID(id, prop); }
}
