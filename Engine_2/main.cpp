//(c)TDV
#include <gl/glew.h>
#include <glm.hpp>
#include "Application.h"
#include "MainDrawingLoop.h"
#include "RenderText.h"
#include <iostream>
using namespace std;


void print(TreeNode* root, long long indent = -2) {
    indent += 2;
    for (auto& child : root->children) {
        cout << std::string(indent, '-') << "Name: " << child->name << " ID: " << child->getId();

        for (auto& Mesh : child->meshes) cout << endl << std::string(indent, '-') << " Meshes: " << Mesh.name;

        cout << endl;
        print(child, indent);
    }
}

bool tempTRUE=true;//////////////////////////////////////
int main() {
    Application app;
    MainDrawingLoop mainLoop(app,&app.render);

    app.setInitShaderOptions();
    app.setRenderOptions();
    app.createSkyBox();

    mainLoop.text = new RenderText(app.getWindowWidth(), app.getWindowHeight());
    mainLoop.text->Load("fonts/arial.ttf", 24); //windows font

    glm::mat4 projection = glm::perspectiveFov(glm::radians(app.getCamera()->GetZoom()), app.getWindowWidth(),
                                               app.getWindowHeight(), 0.1f, 100.f);
    auto view = app.getCamera()->GetViewMatrix();
     
    auto texDUDV = app.render.generateTexturesFromFile("dudv.png", "");
    auto texWater = app.render.generateTexturesFromFile("water.jpg", "");
    glBindTexture(GL_TEXTURE_2D, texDUDV);
    auto objID=app.render.createRectangleWithUV(5, -5, 50, 50, glm::vec4(0.0, 0.0, 1.0, 1.0), 0, &tempTRUE);
    
    app.render.Frames[0].objectList[objID].shader = WATER_SHADER;
    app.render.Frames[0].objectList[objID].textureID = texDUDV;
    app.render.Frames[0].objectList[objID].textureID2 = texWater;
    
    glUseProgram(app.render.shaders[6].ID);
    app.render.shaders[6].setInt("DUDV", (int)0);
    app.render.shaders[6].setInt("u_water", (int)1);
    app.render.shaders[6].setInt("skybox", (int)3);
    app.render.shaders[6].setMat4("u_projection", projection);
    app.render.shaders[6].setMat4("u_view", view);
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //app.LoadModel("models/test_model_transform.dae");
    app.LoadModel("models/man.dae");
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      app.root->getChild(0)->localMatrix = glm::rotate(app.root->getChild(0)->localMatrix, glm::radians(-90.0f),
                                                     glm::vec3(1.0, 0.0, 0.0));

    app.root->shaderRewriteRec(app.render.shaders);
    app.root->setSkyboxID(&app.render.skyBoxCubemapTexture);
    print(app.root, -2);
    mainLoop.Loop();
    
    glfwTerminate();
    delete mainLoop.text;
    return 0;
}
