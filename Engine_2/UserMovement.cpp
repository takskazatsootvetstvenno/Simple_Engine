#include "UserMovement.h"

#include <iostream>
ICamera* camera;
//bool keys[1024];
GLfloat lastXuserMov, lastYuserMov;
bool firstMouseUserMov;
float* windowWidth;
float* windowHeight;
bool cameraIsUsing;
//bool last_clicked;
//bool grave_clicked;
double xpos, ypos;
KeyStates& UserMovement::getAllStates() {
  /*  switch (state) {
        case -1:
            break;
        case 1:
            generalKeyStates.grave_clicked = true;
            break;
        case 0:
            generalKeyStates.grave_clicked = false;
        default:
            break;
    }*/
    return generalKeyStates;
}
UserMovement::UserMovement(ICamera* curcam, float* windowW, float* windowH) {
    windowWidth = windowW;
    windowHeight = windowH;
    camera = curcam;

    memset(generalKeyStates.keys, 0, sizeof(generalKeyStates.keys));
    cameraIsUsing = true;
    firstMouseUserMov = true;
    lastXuserMov = 0.f;
    lastYuserMov = 0.f;
}
bool UserMovement::checkAndGetLastMousePos(float& x, float& y) {
    x = xpos;
    y = ypos;
    if (generalKeyStates.last_clicked) {
        generalKeyStates.last_clicked = false;
        return true;
    }
    return false;
}
void UserMovement::setWindowSize(float windowW, float windowH) {
    *windowWidth = windowW;
    *windowHeight = windowH;
}
void UserMovement::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        cameraIsUsing = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        // double xpos, ypos;
        generalKeyStates.last_clicked = true;
        // getting cursor position
        glfwGetCursorPos(window, &xpos, &ypos);

        std::cout << "Cursor Position at (" << xpos << " : " << ypos << ")\n";
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        cameraIsUsing = true;
        firstMouseUserMov = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        // double xpos, ypos;
        // getting cursor position
        glfwGetCursorPos(window, &xpos, &ypos);
        std::cout << "Cursor Position at (" << xpos << " : " << ypos << ")\n";
    }
}
void UserMovement::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (cameraIsUsing) {
        if (firstMouseUserMov) {
            lastXuserMov = static_cast<float>(xpos);
            lastYuserMov = static_cast<float>(ypos);
            firstMouseUserMov = false;
        }
        float xoffset = static_cast<float>(static_cast<float>(xpos) - lastXuserMov);
        float yoffset = static_cast<float>(lastYuserMov - static_cast<float>(ypos));

        lastXuserMov = static_cast<float>(xpos);
        lastYuserMov = static_cast<float>(ypos);
        camera->ProcessMouseMovement(xoffset, yoffset);
    }
}
void UserMovement::movement(float deltaTime) {
    if (cameraIsUsing) {
        if (generalKeyStates.keys[GLFW_KEY_W]) {
            camera->ProcessKeyboard(FORWARD, deltaTime);
            // (*camera).ProcessKeyboard(0, deltaTime);
            // *local_eternal_matrix= glm::translate(*local_eternal_matrix, glm::vec3(1.0, 0.0, 0.0));
            // *local_m= (*local_m) * (*local_eternal_matrix);
        }

        if (generalKeyStates.keys[GLFW_KEY_S]) {
            camera->ProcessKeyboard(BACKWARD, deltaTime);
            // (*camera).ProcessKeyboard(1, deltaTime);
            // *local_eternal_matrix = glm::translate(*local_eternal_matrix, glm::vec3(-1.0, 0.0, 0.0));
        }
        if (generalKeyStates.keys[GLFW_KEY_A]) {
            camera->ProcessKeyboard(LEFT, deltaTime);
            // (*camera).ProcessKeyboard(2, deltaTime);
            // *local_eternal_matrix = glm::translate(*local_eternal_matrix, glm::vec3(0.0, 1.0, 0.0));
        }
        if (generalKeyStates.keys[GLFW_KEY_D]) {
            camera->ProcessKeyboard(RIGHT, deltaTime);
            // (*camera).ProcessKeyboard(3, deltaTime);
            //*local_eternal_matrix = glm::translate(*local_eternal_matrix, glm::vec3(0.0, -1.0, 0.0));
        }
        if (generalKeyStates.keys[GLFW_KEY_Q]) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            // (*camera).ProcessKeyboard(2, deltaTime);
            //*local_eternal_matrix = glm::translate(*local_eternal_matrix, glm::vec3(0.0, 0.0, 1.0));
        }
        if (generalKeyStates.keys[GLFW_KEY_E]) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            //(*camera).ProcessKeyboard(3, deltaTime);
            //*local_eternal_matrix = glm::translate(*local_eternal_matrix, glm::vec3(0.0, 0.0, -1.0));
        }
    }
}
void UserMovement::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    // float cameraSpeed = 3.f * deltaTime;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) camera->LoadDefault();
    if (generalKeyStates.keys[GLFW_KEY_GRAVE_ACCENT]) {
        generalKeyStates.grave_clicked = true; }
    if (generalKeyStates.keys[GLFW_KEY_T]) {
        generalKeyStates.grave_clicked = true; }

    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS)
            generalKeyStates.keys[key] = true;
        else if (action == GLFW_RELEASE)
            generalKeyStates.keys[key] = false;
    }
    /*  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
          camera->ProcessKeyboard(FORWARD, deltaTime);
      if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
          camera->ProcessKeyboard(BACKWARD, deltaTime);
      if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
          camera->ProcessKeyboard(LEFT, deltaTime);
      if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
          camera->ProcessKeyboard(RIGHT, deltaTime);
          */
}
void UserMovement::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    if (cameraIsUsing) camera->ProcessMouseScroll(static_cast<float>(yoffset));
}
void UserMovement::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    // aspect = static_cast<float>(width / height);
    *windowWidth = width;
    *windowHeight = height;
    glViewport(0, 0, width, height);
}
float UserMovement::getAspect() {
    return float(*windowWidth) / float(*windowHeight);
}
