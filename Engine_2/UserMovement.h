#ifndef USERMOVEMENT
#define USERMOVEMENT

#include "Camera.h"

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <iostream>
 struct KeyStates
{
    bool grave_clicked = false;
    bool last_clicked = false;
    bool ToText_clicked = false;
    bool keys[1024];
  //  bool ToText_clicked = false;
};
 static KeyStates generalKeyStates;

class UserMovement {
   public:
    UserMovement(ICamera* curcam, float* windowW, float* windowH);
    void movement(float deltaTime);
    float getAspect();
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    void setWindowSize(float windowWidth, float windowHeight);
    bool checkAndGetLastMousePos(float& x, float& y);
    KeyStates& getAllStates();
   
};
//bool UserMovement::grave_clicked = false;
#endif USERMOVEMENT
