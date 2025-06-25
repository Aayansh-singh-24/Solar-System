#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<tuple>

#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "Movement.h"

static glm::vec3 CameraPos(0.5f, 0.0f, 1.5f);
static glm::vec3 CameraFront(0.0f, 0.0f, -7.0f);
static glm::vec3 CameraUp(0.0f, 1.0f, 0.0f);

std::tuple<glm::vec3&, glm::vec3&, glm::vec3&> GetCamera()
{
    return std::tie(CameraPos, CameraFront, CameraUp);
}

void KeyboardInput(GLFWwindow* window , float deltaTime)
{
    float CameraSpeed = 0.005f;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);


    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        CameraPos += CameraFront * CameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        CameraPos -= CameraSpeed * CameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        CameraPos -= glm::normalize(glm::cross(CameraFront, CameraUp)) * CameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        CameraPos += glm::normalize(glm::cross(CameraFront, CameraUp)) * CameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        CameraPos = glm::vec3(0.5f, 0.0f, 1.5f);

} 

float yaw = -90.0f;
float pitch = 0.0f;
bool firstmouse = true;
float lastX = 900.0f / 2.0f;
float lastY = 900.0f / 2.0f;

void Mouse_Callback(GLFWwindow* window,double Xpos, double Ypos)
{
    if (firstmouse)
    {
        lastX = Xpos;
        lastY = Ypos;
        firstmouse = false;
    }

    //Adding  the offset values into Yaw and  pitch::
    float Xoffset = Xpos - lastX;
    float Yoffset = lastY - Ypos;
    lastX = Xpos;
    lastY = Ypos;

    float sensitivity = 0.1f;
    Xoffset *= sensitivity;
    Yoffset *= sensitivity;

    yaw += Xoffset;
    pitch += Yoffset;

    // Adding Constrain::
    if (pitch > 89.9)
        pitch = 89.9;
    if (pitch < -89.9)
        pitch = -89.9;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    CameraFront = glm::normalize(direction);

}



