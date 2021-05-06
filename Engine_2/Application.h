//(c)TDV
#ifndef APPLICATION_H
#define APPLICATION_H
#include "Camera.h"
#include "TreeNode.h"
#include "UserMovement.h"
#include "Renderer.h"

class Application {
public:
    // unsigned int texColorBuffer = 0;
    // unsigned int framebuffer = 0;
    // unsigned int screenVAO = 0;
    Renderer render;

    //vector<Shader> shaders;
    TreeNode* root = new TreeNode(render.shaders, &render);

    void LoadModel(const string& path, bool gamma = false);
    ICamera* getCamera();
    UserMovement* getUserMovement();
    float getWindowWidth();
    float getWindowHeight();
    void drawCheckedNode();
    void drawSkybox();
    void setRenderOptions();
    void setInitShaderOptions();
    void  applyCurrentShadersToRoot();
    void createSkyBox(const string& path = "Skybox/");
    
    Application();
    ~Application();

    float ClickMousePosX = 0;
    float ClickMousePosY = 0;
    bool clicked = false;
    bool showConsole = false;
    int clickedID = 0;
   private:

    float* width = nullptr;
    float* height = nullptr;
    ICamera* cam = render.cam;//new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
    UserMovement* userMovement = new UserMovement(cam, width, height);
};
#endif
