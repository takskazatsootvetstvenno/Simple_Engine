#ifndef MAINDRAWINGLOOP_H
#define MAINDRAWINGLOOP_H
#include "Application.h"
#include "RenderText.h"
#include "Renderer.h"
#include "ConsoleObj.h"

class MainDrawingLoop {
   public:
    Application& app;
    RenderText* text = nullptr;
    MainDrawingLoop(Application& curApp,Renderer* initRender);
    void Loop();
    
   private:
    ConsoleObj tinyConsole;
    void drawScreen();
    void updateProjViewUBO();
    void deltaTimeAndFPS();
    unsigned long int frame_counter = 0;
    double deltaTime = 0.0;     // время между текущим и последним кадрами
    double previousTime = 0.0;  // время последнего кадра
    double currentTime = 0.0;
    double previous = 0.0;

    float xpos=0.0f;
    float ypos=0.0f;
    char str[10]{};

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 lastProjection = glm::mat4(1.0f);
    Renderer* curRender;
};
#endif
