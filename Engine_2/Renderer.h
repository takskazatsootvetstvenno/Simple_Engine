#ifndef RENDERER_H
#define RENDERER_H
#include "Camera.h"
#include "Shader.h"
#include "UserMovement.h"


#include <GLFW/glfw3.h>

#include <string>
#include <vector>

enum ShaderSelector {
    DEFAULT_SHADER = 0,
    TRANSPARENT_SHADER = 1,
    CHOOSECOLOR_SHADER = 2,
    HIGHLIGHT_SHADER = 3,
    SKYBOX_SHADER = 4,
    ONE_COLOR_SHADER = 5,
    WATER_SHADER = 6
};
struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};
struct ScreenFrameInfo {
    unsigned int framebufferTemp = 0;
    unsigned int renderbufferTemp = 0;
    unsigned int texColorBufferTemp = 0;
    unsigned int screenVaoTemp = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;
};
struct MeshRenderInfo {
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;
    unsigned int* skyBoxID = nullptr;
};
struct ObjectToDraw {
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int count = 0;
    unsigned int objID = 0;
    unsigned int textureID = 0;
    unsigned int textureID2 = 0;
    int shader = -1;
    unsigned int EBO = 0;
    bool* isDraw = nullptr;
   // unsigned int* skyBoxID = nullptr;
};
struct FrameInfo {
    unsigned int framebuffer = 0;
    unsigned int renderbuffer = 0;
    unsigned int texColorBuffer = 0;
    unsigned int width = 0;
    unsigned int height = 0;
    unsigned int frameVBO = 0;
    unsigned int frameVAO = 0;
    unsigned int frameEBO = 0;
    std::vector<ObjectToDraw> objectList;
};
class Renderer {
   public:
    std::vector<Shader> shaders;
    Camera* camEuler = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
    ICamera* cam = dynamic_cast<ICamera*>(camEuler);

    unsigned int skyBoxCubemapTexture = 0;
    std::vector<unsigned int> UBO;
    void createSkyBox(const std::string& path = "Skybox/");
    void drawSkybox();
    int drawCheckedNodeAndReturnID(float ClickMousePosX, float ClickMousePosY);
    unsigned int CreateFrame(unsigned int width, unsigned int height,bool withDepthBuffer=true, float x = 0.f,float y = 0.f,float relWidth=1.f,float relHeight=1.f);
    void setInitShaderOptions();
    void setNewResolution(float newWidth, float newHeight);
    void drawObjects();
    unsigned int createRectangle(float x, float y, float width, float height,const glm::vec4& Color,unsigned int FrameID, bool* newBool);
    unsigned int createRectangleWithUV(float x, float y, float width, float height, const glm::vec4& Color, unsigned int FrameID, bool* newBool);
    void updateModelMatrix(const Shader& shader, const glm::mat4& localMatrix);
    void updateViewMatrix(unsigned int UBO, const glm::mat4& viewMatrix);
    void updateProjectionMatrix(unsigned int UBO, const glm::mat4& projMatrix);

    void UseProgram(unsigned int ID);
    void addShader(const std::string& vertPath = "Shaders/vertex.vert",
                   const std::string& fragPath = "Shaders/fragment.frag");

    Renderer();
    void createAndChooseTempFrame();
    void setRenderOptions();

    unsigned int generateTexturesFromFile(const std::string& filename, const std::string& path);
    void meshPresets(MeshRenderInfo& MeshInfo, unsigned int verticesSize, unsigned int indicesSize,
                     unsigned int VertexSize, unsigned int offsetNormal, unsigned int offsetTexCoord,
                     float& firstPos, unsigned int& firstInd);
    void meshDraw(Shader& meshShader, std::vector<Texture>& textures, MeshRenderInfo& MeshInfo,
                  unsigned int indVectorSize);

    unsigned int texColorBuffer = 0;
    unsigned int framebuffer = 0;
    unsigned int screenVAO = 0;
    GLFWwindow* window;
    UserMovement* userMovement;
    float width = 1440;
    float height = 900;
        std::vector<FrameInfo> Frames;
   private:

       void createFramebufferScreen(FrameInfo& screenInfo, float x, float y,float relWidth,float relHeight);
    //std::vector<ObjectToDraw> ObjectsList;
    ObjectToDraw& nextObj(unsigned int FrameID);
    unsigned int loadCubemap(std::vector<std::string> faces);
    unsigned int skyBoxVAO = 0;
    void presetsForRenderCheck(unsigned int& framebuffer, unsigned int& renderbuffer,
                               unsigned int& texColorBuffer, unsigned int& VAO, unsigned int& VBO,
                               unsigned int& EBO, int windowWidth, int windowHeight);
    void deleteRenderOptions();

    ScreenFrameInfo tempScreen;

    unsigned int renderbuffer = -1;
    unsigned int mainVBO = -1;
    unsigned int mainEBO = -1;
};
#endif
