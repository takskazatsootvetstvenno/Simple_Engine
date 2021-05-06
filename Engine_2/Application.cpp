//(c)TDV
#include "Application.h"

#include "Model.h"

#include <gtc/type_ptr.hpp>

// Загрузка текстуры кубической карты из 6 отдельных текстур-граней.
// Порядок:
// +X (правая грань)
// -X (левая грань)
// +Y (верх)
// -Y (низ)
// +Z (фронтальная грань)
// -Z (тыльная грань)
// -------------------------------------------------------

void Application::drawSkybox() {
    render.drawSkybox();
}
void Application::createSkyBox(const string& path) {
    render.createSkyBox();
}
ICamera* Application::getCamera() {
    return cam;
}
void Application::drawCheckedNode() {
    root->giveUniqIdRec(1);
    root->setProp(CHOOSECOLORID);
    glm::mat4 model = glm::mat4(1.0f);
    render.createAndChooseTempFrame();
    root->useMeshesRec(model);

    int tempID = render.drawCheckedNodeAndReturnID(ClickMousePosX, ClickMousePosY);
    clickedID = tempID;
    root->SetPropByID(tempID, HIGHLIGHT);
}
void Application::setRenderOptions() {
    render.setRenderOptions();
}
UserMovement* Application::getUserMovement() {
    return userMovement;
}
Application::Application() {
    // createSkyBox();
    render.userMovement = userMovement;
    width = &render.width;
    height = &render.height;
}
void Application::setInitShaderOptions() {
    render.addShader();                                                 // 0
    render.addShader("Shaders/screen.vert", "Shaders/screen.frag");     // 1
    render.addShader("Shaders/vertex.vert", "Shaders/checknode.frag");  // 2
    render.addShader("Shaders/vertex.vert", "Shaders/border.frag");     // 3
    render.addShader("Shaders/skybox.vert", "Shaders/skybox.frag");     // 4
    render.addShader("Shaders/oneColor.vert", "Shaders/oneColor.frag"); // 5
    render.addShader("Shaders/water.vert", "Shaders/water.frag");       // 6
    render.setInitShaderOptions();
    applyCurrentShadersToRoot();
}
Application::~Application() {
    delete cam;
    delete userMovement;
    delete root;
}
void Application::applyCurrentShadersToRoot() {
    root->shaderRewriteRec(render.shaders);
}
void Application::LoadModel(string const& path, bool gamma) {
    std::cout << "try to load model: " << path<<endl;
    Model myModel(path, *root, render.shaders, &render, gamma);
}
float Application::getWindowWidth() {
    return *width;
}
float Application::getWindowHeight() {
    return *height;
}
