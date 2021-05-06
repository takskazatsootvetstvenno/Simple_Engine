#ifndef CAMERA_H
#define CAMERA_H

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include <vector>

enum Camera_Movement { FORWARD, BACKWARD, LEFT, RIGHT };

// defaul velues
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 3.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

// Eular Camera class
class ICamera {
   public:
    virtual glm::mat4 GetViewMatrix() = 0;
    float g = 0;
   // glm::vec3 Position;
    virtual float GetZoom() = 0;

    virtual void ProcessKeyboard(Camera_Movement direction, float deltaTime) = 0;

    virtual void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) = 0;

    virtual void ProcessMouseScroll(float yoffset) = 0;
    // load deafult values
    virtual void LoadDefault() = 0;
    virtual glm::vec3 getCameraPosition() =0;
   private:
    /*
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // углы Эйлера
    float Yaw;
    float Pitch;
    // Настройки камеры
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;*/
};
class Camera : public ICamera {
   public:
    // Vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = YAW, float pitch = PITCH);
    // Scalars
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
    // Возвращает матрицу вида, вычисленную с использованием углов Эйлера и LookAt-матрицы
    virtual glm::mat4 GetViewMatrix() override;
    float g = 0;
    virtual float GetZoom() override;

    virtual void ProcessKeyboard(Camera_Movement direction, float deltaTime) override;

    virtual void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) override;

    virtual void ProcessMouseScroll(float yoffset) override;
    // load deafult values
    virtual void LoadDefault() override;
    virtual glm::vec3 getCameraPosition() override;
    
   private:

    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    float Yaw;
    float Pitch;

    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    void updateCameraVectors();
};

/*class Camera{
   public:
    // Атрибуты камеры
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    float Yaw;
    float Pitch;

    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // Vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = YAW, float pitch = PITCH);
    // Scalars
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

     glm::mat4 GetViewMatrix();

     void ProcessKeyboard(Camera_Movement direction, float deltaTime) ;

     void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

     void ProcessMouseScroll(float yoffset);
    // load deafult values
     void LoadDefault();

   private:
     void updateCameraVectors();
};
*/
#endif
