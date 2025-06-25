#pragma once
#ifndef MOVEMENT_H
#define MOVEMENT_H
#include<iostream>
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

std::tuple<glm::vec3&, glm::vec3&, glm::vec3&> GetCamera();
void KeyboardInput(GLFWwindow* window, float deltaTime);
void Mouse_Callback(GLFWwindow* window, double Xpos, double Ypos);

#endif // !MOVEMENT_H
