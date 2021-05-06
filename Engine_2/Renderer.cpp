#include "GL/glew.h"
#include "Renderer.h"

#include <gtc/type_ptr.hpp>

	#define STB_IMAGE_IMPLEMENTATION
	#include <stb_image.h>


#include <iostream>

void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length,
                            const char* message, const void* userParam) {
    // Игнорируем несущественные коды ошибок/предупреждений
    if (
        id == 131169 ||
        id == 131185 ||
        id == 131204 ||
        id == 131218)return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;

    switch (source) {
        case GL_DEBUG_SOURCE_API:
            std::cout << "Source: API";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            std::cout << "Source: Window System";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            std::cout << "Source: Shader Compiler";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            std::cout << "Source: Third Party";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            std::cout << "Source: Application";
            break;
        case GL_DEBUG_SOURCE_OTHER:
            std::cout << "Source: Other";
            break;
    }
    std::cout << std::endl;

    switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            std::cout << "Type: Error";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            std::cout << "Type: Deprecated Behaviour";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            std::cout << "Type: Undefined Behaviour";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            std::cout << "Type: Portability";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            std::cout << "Type: Performance";
            break;
        case GL_DEBUG_TYPE_MARKER:
            std::cout << "Type: Marker";
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            std::cout << "Type: Push Group";
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            std::cout << "Type: Pop Group";
            break;
        case GL_DEBUG_TYPE_OTHER:
            std::cout << "Type: Other";
            break;
    }
    std::cout << std::endl;

    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            std::cout << "Severity: high";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            std::cout << "Severity: medium";
            break;
        case GL_DEBUG_SEVERITY_LOW:
            std::cout << "Severity: low";
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            std::cout << "Severity: notification";
            break;
    }
    std::cout << std::endl;
    std::cout << std::endl;
}

GLFWwindow* init(int windowWidth, int windowHeight) {
    if (glfwInit() != GLFW_TRUE) { std::cout << "Error! glfwInit error!\n"; }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);  // DEBUG

    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "OpenGL", nullptr, nullptr);
    if (!window) { std::cout << "Error! Can't create window!\n"; }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    glewExperimental = GL_TRUE;
    glEnable(GL_DEPTH_TEST);
    if (glewInit() != GLEW_OK) { std::cout << "Error! glewInit error!\n"; }
    int flags;

    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);  // DEBUG
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
    return window;
}

void Renderer::setRenderOptions() {

    presetsForRenderCheck(framebuffer, renderbuffer, texColorBuffer, screenVAO, mainVBO, mainEBO, width, height);
    FrameInfo temp;
    temp.framebuffer = framebuffer;
    temp.renderbuffer = renderbuffer;
    temp.texColorBuffer = texColorBuffer;
    Frames.push_back(temp);
}
unsigned int Renderer::CreateFrame(unsigned int newWidth, unsigned int newHeight, bool withDepthBuffer, float x, float y,float relWidth,float relHeight) {
    FrameInfo FB;
    FB.width = newWidth;
    FB.height = newHeight;
    if (withDepthBuffer)
        presetsForRenderCheck(FB.framebuffer, FB.renderbuffer, FB.texColorBuffer, FB.frameVAO, FB.frameVBO, FB.frameEBO, FB.width, FB.height);
    else
        createFramebufferScreen(FB, x, y, relWidth, relHeight);
    Frames.push_back(FB);
    glBindFramebuffer(GL_FRAMEBUFFER, FB.framebuffer);
    glClearColor(0.0f, 0.5f, 0.5f, 0.5f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    return (Frames.size() - 1);
}
void Renderer::setNewResolution(float newWidth, float newHeight) {
    width = newWidth;
    height = newHeight;
    int widthInt = static_cast<int>(width);
    int heightInt = static_cast<int>(height);
    if(framebuffer!=0)
    deleteRenderOptions();

    presetsForRenderCheck(framebuffer, renderbuffer, texColorBuffer, screenVAO, mainVBO, mainEBO, widthInt, heightInt);
    glViewport(0, 0, widthInt, heightInt);
    glfwSetWindowSize(window, widthInt, heightInt);
}
void Renderer::deleteRenderOptions() {
   // unsigned int VBO = 0;
   // unsigned int EBO = 0;
    glDeleteVertexArrays(1, &screenVAO);
    glDeleteFramebuffers(1, &framebuffer);
    glDeleteRenderbuffers(1, &renderbuffer);
    glDeleteTextures(1, &texColorBuffer);
    glDeleteBuffers(1, &mainVBO);
    glDeleteBuffers(1, &mainEBO);
}
void Renderer::createFramebufferScreen(FrameInfo& screenInfo, float x, float y,float relWidth,float relHeight)
{
  //  screenInfo.width = 900;
  //  screenInfo.height = 600;
    auto& VBO = screenInfo.frameVBO;
    auto& VAO = screenInfo.frameVAO;
    auto& EBO = screenInfo.frameEBO;
    glCreateFramebuffers(1, &(screenInfo.framebuffer));
    glBindFramebuffer(GL_FRAMEBUFFER, screenInfo.framebuffer);

    glCreateTextures(GL_TEXTURE_2D, 1, &(screenInfo.texColorBuffer));
    
    glTextureParameteri(screenInfo.texColorBuffer, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(screenInfo.texColorBuffer, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTextureStorage2D(screenInfo.texColorBuffer, 1, GL_RGBA8,  screenInfo.width,  screenInfo.height);
    glNamedFramebufferTexture(screenInfo.framebuffer, GL_COLOR_ATTACHMENT0, screenInfo.texColorBuffer, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    float vertices[] = {
        -1.0f*relWidth + x, -1.0f*relHeight + y, 0.0f, 0.0f,
        -1.0f*relWidth + x, 1.0f*relHeight + y, 0.0f, 1.0f,
        1.0f*relWidth + x, -1.0f*relHeight+y, 1.0f, 0.0f,
        1.0f*relWidth+ x, 1.0f*relHeight+y, 1.0f, 1.0f};
    int indices[] = {0, 1, 2, 3};

    glCreateBuffers(1, &VBO);
    glNamedBufferStorage(VBO, sizeof(vertices), &vertices, GL_DYNAMIC_STORAGE_BIT);
    glCreateBuffers(1, &EBO);
    glNamedBufferStorage(EBO, sizeof(indices), indices, GL_DYNAMIC_STORAGE_BIT);
    glCreateVertexArrays(1, &VAO);
    glVertexArrayVertexBuffer(VAO, 0, VBO, 0, 4 * sizeof(float));
    glVertexArrayElementBuffer(VAO, EBO);

     glEnableVertexArrayAttrib(VAO, 0);
    glEnableVertexArrayAttrib(VAO, 1);

    glVertexArrayAttribFormat(VAO, 0, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribFormat(VAO, 1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float));

    glVertexArrayAttribBinding(VAO, 0, 0);
    glVertexArrayAttribBinding(VAO, 1, 0);

    glBindVertexArray(0);
}
void Renderer::presetsForRenderCheck(unsigned int& framebuffer, unsigned int& renderbuffer,
                                     unsigned int& texColorBuffer, unsigned int& VAO, unsigned int& VBO,
                                     unsigned int& EBO, int windowWidth, int windowHeight) {
    glCreateFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glCreateTextures(GL_TEXTURE_2D, 1, &texColorBuffer);
    
    glTextureParameteri(texColorBuffer, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(texColorBuffer, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTextureParameteri(texColorBuffer, GL_TEXTURE_BASE_LEVEL, 0);
    glTextureParameteri(texColorBuffer, GL_TEXTURE_MAX_LEVEL, 0);

    glTextureStorage2D(texColorBuffer, 1, GL_RGBA8, windowWidth, windowHeight);
    glNamedFramebufferTexture(framebuffer, GL_COLOR_ATTACHMENT0, texColorBuffer, 0);
    glGenRenderbuffers(1, &renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);

    glNamedRenderbufferStorage(renderbuffer, GL_DEPTH24_STENCIL8, windowWidth, windowHeight);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glNamedFramebufferRenderbuffer(framebuffer, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    float vertices[] = {
        -1.0f, -1.0f, 0.0f, 0.0f,
        -1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f};
    int indices[] = {0, 1, 2, 3};

    glCreateBuffers(1, &VBO);
    glNamedBufferStorage(VBO, sizeof(vertices), &vertices, GL_DYNAMIC_STORAGE_BIT);
    glCreateBuffers(1, &EBO);
    glNamedBufferStorage(EBO, sizeof(indices), indices, GL_DYNAMIC_STORAGE_BIT);
    glCreateVertexArrays(1, &VAO);
    glVertexArrayVertexBuffer(VAO, 0, VBO, 0, 4 * sizeof(float));
    glVertexArrayElementBuffer(VAO, EBO);

    glEnableVertexArrayAttrib(VAO, 0);
    glEnableVertexArrayAttrib(VAO, 1);

    glVertexArrayAttribFormat(VAO, 0, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribFormat(VAO, 1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float));

    glVertexArrayAttribBinding(VAO, 0, 0);
    glVertexArrayAttribBinding(VAO, 1, 0);

    glBindVertexArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glClear(GL_COLOR_BUFFER_BIT);
    //glBindTexture(GL_TEXTURE, 0);
}
unsigned int Renderer::loadCubemap(std::vector<std::string> faces) {
    /*unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB,
                         GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        } else {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    return textureID;
    */
    unsigned int textureID;
    glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &textureID);
    bool checktemp = true;
    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            if (checktemp) {
                glTextureStorage2D(textureID, 1, GL_RGB8, width, height);
                checktemp = false;
            }
            glTextureSubImage3D(textureID, 0, 0, 0, i, width, height, 1, GL_RGB, GL_UNSIGNED_BYTE, data);
            // glTextureSubImage3D(textureID, i, 0, 0, 0, width, height, i, GL_RGB, GL_UNSIGNED_BYTE, data);
            // glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB,
            //              GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        } else {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(textureID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return textureID;
}
void Renderer::drawSkybox() {
    glUseProgram(shaders[SKYBOX_SHADER].ID);
    glDepthFunc(GL_LEQUAL);  // меняем функцию глубины, чтобы обеспечить прохождение теста глубины, когда
                             // значения равны содержимому буфера глубины
    glm::mat4 view = glm::mat4(glm::mat3(
        cam->GetViewMatrix()));  // убираем из матрицы вида секцию, отвечающую за операцию трансляции
    glm::mat4 projection = glm::perspectiveFov(glm::radians(cam->GetZoom()), width, height, 0.1f, 100.f);

    glBindBuffer(GL_UNIFORM_BUFFER, UBO[1]);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));

    glBindBuffer(GL_UNIFORM_BUFFER, UBO[1]);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // Куб скайбокса
    glBindVertexArray(skyBoxVAO);
   // glActiveTexture(GL_TEXTURE0);
  //  glBindTexture(GL_TEXTURE_CUBE_MAP, skyBoxCubemapTexture);
    glBindTextureUnit(0, skyBoxCubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);  // восстанавливаем стандартное значение функции теста глубины
}
void Renderer::createSkyBox(const std::string& path) {
    stbi_set_flip_vertically_on_load(false);
    float skyboxVertices[] = {-1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f,
                              1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f,

                              -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f,
                              -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,

                              1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
                              1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,

                              -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
                              1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,

                              -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,
                              1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,

                              -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f,
                              1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f};
    std::vector<std::string> faces{path + "right.jpg",  path + "left.jpg",  path + "top.jpg",
                                   path + "bottom.jpg", path + "front.jpg", path + "back.jpg"};
    unsigned int skyBoxVAOtemp;
    unsigned int skyboxVBO;
    glGenVertexArrays(1, &skyBoxVAOtemp);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyBoxVAOtemp);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    skyBoxCubemapTexture = loadCubemap(faces);
    // glUseProgram(shaders[4].ID);
    // shaders[4].setInt("skybox", 0);
    skyBoxVAO = skyBoxVAOtemp;
    stbi_set_flip_vertically_on_load(true);
}
ObjectToDraw& Renderer::nextObj(unsigned int FrameID)
{
    auto& objList =  Frames[FrameID].objectList;
    for(unsigned int i=0; i < objList.size(); i++)
    {
        if (objList[i].VAO == -1)
        {
            objList[i].objID = i;
            return objList[i];
        }
    }
    ObjectToDraw newLastObj; 
    newLastObj.objID = objList.size();
    objList.push_back(newLastObj);
    return objList[objList.size()-1];
}
unsigned int Renderer::createRectangleWithUV(float x, float y, float width, float height, const glm::vec4& Color,unsigned int FrameID, bool* newBool)
{
    //ObjectToDraw Rectangle;
    auto& Rectangle = nextObj(FrameID);
    float vertices[] = {
           x,  y,          0.f,0.f, Color.x, Color.y, Color.z, Color.w,
           x,  y  + height,0.f,1.f, Color.x, Color.y, Color.z, Color.w,
       x + width,y+ height,1.f,1.f, Color.x, Color.y, Color.z, Color.w,
       x + width,y,        1.f,0.f, Color.x, Color.y, Color.z, Color.w,};
    unsigned int indices[] = {0, 1, 2, 2, 3, 0};

    glCreateBuffers(1, &Rectangle.VBO);
    glNamedBufferStorage(Rectangle.VBO, sizeof(vertices), &vertices, GL_DYNAMIC_STORAGE_BIT);
    glCreateBuffers(1, &Rectangle.EBO);
    glNamedBufferStorage(Rectangle.EBO, sizeof(indices), indices, GL_DYNAMIC_STORAGE_BIT);
    glCreateVertexArrays(1, &Rectangle.VAO);
    glVertexArrayVertexBuffer(Rectangle.VAO, 0, Rectangle.VBO, 0, 8 * sizeof(float));
    glVertexArrayElementBuffer(Rectangle.VAO, Rectangle.EBO);

    glEnableVertexArrayAttrib(Rectangle.VAO, 0);
    glEnableVertexArrayAttrib(Rectangle.VAO, 1);
    glEnableVertexArrayAttrib(Rectangle.VAO, 2);

    glVertexArrayAttribFormat(Rectangle.VAO, 0, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribFormat(Rectangle.VAO, 1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float));
    glVertexArrayAttribFormat(Rectangle.VAO, 2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float));

    glVertexArrayAttribBinding(Rectangle.VAO, 0, 0);
    glVertexArrayAttribBinding(Rectangle.VAO, 1, 0);
    glVertexArrayAttribBinding(Rectangle.VAO, 2, 0);

    glBindVertexArray(0);
    Rectangle.count = 6;
    Rectangle.isDraw = newBool;
    Rectangle.shader = ONE_COLOR_SHADER;
    return Rectangle.objID;
}
unsigned int Renderer::createRectangle(float x, float y, float width, float height, const glm::vec4& Color,unsigned int FrameID, bool* newBool)
{
    //ObjectToDraw Rectangle;
    auto& Rectangle = nextObj(FrameID);
    float vertices[] = {
           x,  y,   Color.x, Color.y, Color.z, Color.w,
           x,  y  + height, Color.x, Color.y, Color.z, Color.w,
       x + width,y+ height, Color.x, Color.y, Color.z, Color.w,
       x + width,y,   Color.x, Color.y, Color.z, Color.w,};
    unsigned int indices[] = {0, 1, 2, 2, 3, 0};

    glCreateBuffers(1, &Rectangle.VBO);
    glNamedBufferStorage(Rectangle.VBO, sizeof(vertices), &vertices, GL_DYNAMIC_STORAGE_BIT);
    glCreateBuffers(1, &Rectangle.EBO);
    glNamedBufferStorage(Rectangle.EBO, sizeof(indices), indices, GL_DYNAMIC_STORAGE_BIT);
    glCreateVertexArrays(1, &Rectangle.VAO);
    glVertexArrayVertexBuffer(Rectangle.VAO, 0, Rectangle.VBO, 0, 6 * sizeof(float));
    glVertexArrayElementBuffer(Rectangle.VAO, Rectangle.EBO);

    glEnableVertexArrayAttrib(Rectangle.VAO, 0);
    glEnableVertexArrayAttrib(Rectangle.VAO, 1);

    glVertexArrayAttribFormat(Rectangle.VAO, 0, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribFormat(Rectangle.VAO, 1, 4, GL_FLOAT, GL_FALSE, 2 * sizeof(float));

    glVertexArrayAttribBinding(Rectangle.VAO, 0, 0);
    glVertexArrayAttribBinding(Rectangle.VAO, 1, 0);

    glBindVertexArray(0);
    Rectangle.count = 6;
    Rectangle.isDraw = newBool;
    Rectangle.shader = ONE_COLOR_SHADER;
    return Rectangle.objID;
}
void Renderer::drawObjects()
{
    for (auto& frame : Frames)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, frame.framebuffer);
        for (auto& obj : frame.objectList)
        {
            if (*(obj.isDraw)) {
              //  glClear(GL_COLOR_BUFFER_BIT);
                glUseProgram(shaders[obj.shader].ID);
                glBindVertexArray(obj.VAO);
                //glBindTexture(GL_TEXTURE_2D, obj.textureID);
                //glBindTexture(GL_TEXTURE_2D, obj.textureID);
                //glActiveTexture(GL_TEXTURE0); 
                glBindTextureUnit(0, obj.textureID);
                //glActiveTexture(GL_TEXTURE1); 
                glBindTextureUnit(1, obj.textureID2);
                glBindTextureUnit(3, skyBoxCubemapTexture);
                // glActiveTexture(GL_TEXTURE_CUBE_MAP);
                //glBindTexture(GL_TEXTURE_CUBE_MAP, skyBoxCubemapTexture);
                //glBindTextureUnit(0, skyBoxCubemapTexture);
                if (obj.EBO == 0)
                    glDrawArrays(GL_TRIANGLES, 0, obj.count);
                else
                    glDrawElements(GL_TRIANGLES, obj.count, GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);
            }
        }
    }
     glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void Renderer::createAndChooseTempFrame() {
    memset(&tempScreen, 0, sizeof(tempScreen));
    int widthInt = static_cast<int>(width);
    int heightInt = static_cast<int>(height);
    presetsForRenderCheck(tempScreen.framebufferTemp, tempScreen.renderbufferTemp,
                          tempScreen.texColorBufferTemp, tempScreen.screenVaoTemp, tempScreen.VBO,
                          tempScreen.EBO, widthInt, heightInt);
    glBindFramebuffer(GL_FRAMEBUFFER, tempScreen.framebufferTemp);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
int Renderer::drawCheckedNodeAndReturnID(float ClickMousePosX, float ClickMousePosY) {
    int widthInt = static_cast<int>(width);
    int heightInt = static_cast<int>(height);

    unsigned char res[4] = {1, 1, 1, 1};
    glViewport(0, 0, widthInt, heightInt);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glReadPixels(ClickMousePosX, heightInt - ClickMousePosY, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, res);
    std::cout << (int)res[0] << " " << (int)res[1] << " " << (int)res[2] << " " << (int)res[3] << std::endl;

    int tempID = 0;
    tempID = static_cast<int>(res[0] * 65536 + res[1] * 256 + res[2]);
    std::cout << tempID;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteVertexArrays(1, &tempScreen.screenVaoTemp);
    glDeleteFramebuffers(1, &tempScreen.framebufferTemp);
    glDeleteRenderbuffers(1, &tempScreen.renderbufferTemp);
    glDeleteTextures(1, &tempScreen.texColorBufferTemp);
    glDeleteBuffers(1, &tempScreen.VBO);
    glDeleteBuffers(1, &tempScreen.EBO);
    return tempID;
}
void Renderer::addShader(std::string const& vertPath, std::string const& fragPath) {
    shaders.push_back(Shader(vertPath, fragPath));
}
void Renderer::setInitShaderOptions() {
    // unsigned int uboMatrices;

    UBO.resize(2);
    glGenBuffers(1, &UBO[0]);
    // UBO.push_back(UBO[0]);//0 - general shader
    shaders[0].UBO = &UBO[0];
    shaders[2].UBO = &UBO[0];
    shaders[3].UBO = &UBO[0];
    shaders[6].UBO = &UBO[0];
    // shaders[4].UBO = &UBO[0];
    glBindBuffer(GL_UNIFORM_BUFFER, UBO[0]);
    glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    // cout << "mat4: " << sizeof(glm::mat4) << " vec3: " << sizeof(glm::vec3);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBO[0], 0, 3 * sizeof(glm::mat4));

    // skybox:
    // unsigned int uboSkyBox;
    glGenBuffers(1, &UBO[1]);
    // UBO.push_back(UBO[1]);

    shaders[4].UBO = &UBO[1];
    shaders[5].UBO = &UBO[1];

    glBindBuffer(GL_UNIFORM_BUFFER, UBO[1]);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferRange(GL_UNIFORM_BUFFER, 1, UBO[1], 0, 2 * sizeof(glm::mat4));
}
unsigned int Renderer::generateTexturesFromFile(const std::string& filename, const std::string& path) {
    /*unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }
    return textureID;*/
    unsigned int textureID;
   glCreateTextures(GL_TEXTURE_2D, 1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

      //  glBindTexture(GL_TEXTURE_2D, textureID);
        glTextureStorage2D(textureID, 1, GL_RGB8,  width,  height);
        glTextureSubImage2D(textureID, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, data);

     //   glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }
    return textureID;
}
void Renderer::UseProgram(unsigned int ID) {
    glUseProgram(ID);
}
void Renderer::updateModelMatrix(const Shader& shader,const glm::mat4& localMatrix) {
    glBindBuffer(GL_UNIFORM_BUFFER, *shader.UBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(localMatrix));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
void Renderer::updateViewMatrix(unsigned int UBO,const glm::mat4& viewMatrix) {
    glBindBuffer(GL_UNIFORM_BUFFER, UBO);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(viewMatrix));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
void Renderer::updateProjectionMatrix(unsigned int UBO,const glm::mat4& projMatrix) {
    glBindBuffer(GL_UNIFORM_BUFFER, UBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projMatrix));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
void Renderer::meshPresets(MeshRenderInfo& MeshInfo, unsigned int verticesSize, unsigned int indicesSize,
                           unsigned int VertexSize, unsigned int offsetNormal, unsigned int offsetTexCoord,
                           float& firstPos, unsigned int& firstInd) {
    glCreateBuffers(1, &MeshInfo.VBO);
    glNamedBufferStorage(MeshInfo.VBO, verticesSize, &firstPos, GL_DYNAMIC_STORAGE_BIT);
    glCreateBuffers(1, &MeshInfo.EBO);
    glNamedBufferStorage(MeshInfo.EBO, indicesSize, &firstInd, GL_DYNAMIC_STORAGE_BIT);
    glCreateVertexArrays(1, &MeshInfo.VAO);
    glVertexArrayVertexBuffer(MeshInfo.VAO, 0, MeshInfo.VBO, 0, VertexSize);
    glVertexArrayElementBuffer(MeshInfo.VAO, MeshInfo.EBO);

    glEnableVertexArrayAttrib(MeshInfo.VAO, 0);  // pos
    glEnableVertexArrayAttrib(MeshInfo.VAO, 1);  // norm
    glEnableVertexArrayAttrib(MeshInfo.VAO, 2);  // texcoord

    glVertexArrayAttribFormat(MeshInfo.VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribFormat(MeshInfo.VAO, 1, 3, GL_FLOAT, GL_FALSE, offsetNormal);
    glVertexArrayAttribFormat(MeshInfo.VAO, 2, 2, GL_FLOAT, GL_FALSE, offsetTexCoord);

    glVertexArrayAttribBinding(MeshInfo.VAO, 0, 0);
    glVertexArrayAttribBinding(MeshInfo.VAO, 1, 0);
    glVertexArrayAttribBinding(MeshInfo.VAO, 2, 0);

    glBindVertexArray(0);
}
Renderer::Renderer(){
    window = init(static_cast<int>(width), static_cast<int>(height));
    glfwSetKeyCallback(window, userMovement->keyCallback);
    glfwSetCursorPosCallback(window, userMovement->mouseCallback);
    glfwSetFramebufferSizeCallback(window, userMovement->framebufferSizeCallback);
    glfwSetScrollCallback(window, userMovement->scrollCallback);
    glfwSetMouseButtonCallback(window, userMovement->mouseButtonCallback);
    glViewport(0, 0, width, height);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
void Renderer::meshDraw(Shader& meshShader, std::vector<Texture>& textures, MeshRenderInfo& MeshInfo,
                        unsigned int indVectorSize) {
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;

    for (unsigned int i = 0; i < textures.size(); i++) {
        // glActiveTexture(GL_TEXTURE0 + i); // перед связыванием активируем нужный текстурный юнит

        // Получаем номер текстуры (номер N в diffuse_textureN)
        std::string number;
        std::string name = textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);  // конвертируем unsigned int в строку
        else if (name == "texture_normal")
            number = std::to_string(normalNr++);  // конвертируем unsigned int в строку
        else if (name == "texture_height")
            number = std::to_string(heightNr++);  // конвертируем unsigned int в строку

        // Теперь устанавливаем сэмплер на нужный текстурный юнит
        // glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
        meshShader.setInt((name + number).c_str(), i);
        // и связываем текстуру
        // glBindTexture(GL_TEXTURE_2D, textures[i].id);
        glBindTextureUnit(i, textures[i].id);
    }
    //if (textures.size() == 0) glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_CUBE_MAP, *MeshInfo.skyBoxID);
  //  glBindTextureUnit(0, *MeshInfo.skyBoxID);
   // glUniform1i(glGetUniformLocation(meshShader.ID, "skybox"), *MeshInfo.skyBoxID);
  //  meshShader.setInt("skybox", *MeshInfo.skyBoxID);
  //  meshShader.setInt("skybox", 0);
    // Отрисовываем меш
    glBindVertexArray(MeshInfo.VAO);
    glDrawElements(GL_TRIANGLES, indVectorSize, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}