#include<glad/glad.h>
#include<iostream>
#include<tuple>
#include<GLFW/glfw3.h>

#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include"Shader.h"
#include"Movement.h"


int width = 900;
int height = 900;
char name[100] = "Solar System";

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


int main() {
	
	// Window formation:-
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, name, NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create window." << std::endl;
		glfwTerminate();
		return -1;
	}

	//<-----------------------------CallBacks----------------------------------------------->
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, Mouse_Callback);


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) // Load GLAD to use opengl Function .
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	float vertices[] = {
		// Position            // Color            // TexCoords
		// Back face
		-0.5f, -0.5f, -0.5f,   1, 0, 0,   0.0f, 0.0f,  // Bottom-left
		 0.5f, -0.5f, -0.5f,   0, 1, 0,   1.0f, 0.0f,  // Bottom-right
		 0.5f,  0.5f, -0.5f,   0, 0, 1,   1.0f, 1.0f,  // Top-right
		-0.5f,  0.5f, -0.5f,   1, 1, 0,   0.0f, 1.0f,  // Top-left

		// Front face
		-0.5f, -0.5f,  0.5f,   1, 0, 1,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,   0, 1, 1,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,   0.5f, 0.5f, 0.5f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,   1, 1, 1,   0.0f, 1.0f,

		// Left face
		-0.5f, -0.5f, -0.5f,   1, 0, 0,   0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,   1, 1, 0,   1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,   1, 1, 1,   1.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,   1, 0, 1,   0.0f, 1.0f,

		// Right face
		 0.5f, -0.5f, -0.5f,   0, 1, 0,   0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,   0, 0, 1,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,   0.5f, 0.5f, 0.5f,   1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,   0, 1, 1,   0.0f, 1.0f,

		 // Bottom face
		 -0.5f, -0.5f, -0.5f,   1, 0, 0,   0.0f, 0.0f,
		  0.5f, -0.5f, -0.5f,   0, 1, 0,   1.0f, 0.0f,
		  0.5f, -0.5f,  0.5f,   0, 1, 1,   1.0f, 1.0f,
		 -0.5f, -0.5f,  0.5f,   1, 0, 1,   0.0f, 1.0f,

		 // Top face
		 -0.5f,  0.5f, -0.5f,   1, 1, 0,   0.0f, 0.0f,
		  0.5f,  0.5f, -0.5f,   0, 0, 1,   1.0f, 0.0f,
		  0.5f,  0.5f,  0.5f,   0.5f, 0.5f, 0.5f,   1.0f, 1.0f,
		 -0.5f,  0.5f,  0.5f,   1, 1, 1,   0.0f, 1.0f
	};

	// index of each vertices
	unsigned int indices[] = {
		0, 1, 2, 2, 3, 0,        // Back face
		4, 5, 6, 6, 7, 4,        // Front face
		8, 9,10,10,11, 8,        // Left face
	   12,13,14,14,15,12,        // Right face
	   16,17,18,18,19,16,        // Bottom face
	   20,21,22,22,23,20         // Top face
	};



	// Shaders::
	unsigned int ShaderProgram = ShaderCompilation();
	// Camera::


	// Buffers:::
	unsigned int VAO{};
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO{};
	glGenBuffers(1, &VBO); 
	glBindBuffer(GL_ARRAY_BUFFER, VBO); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 

	unsigned int EBO{};
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);



	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	float lastFrame = 0.0f;
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		KeyboardInput(window,deltaTime);

		glClearColor(0.1f, 0.0f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(ShaderProgram);

		// Model ::
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.5f, 0.0f));
		unsigned int modelLoc = glad_glGetUniformLocation(ShaderProgram, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		// View::
		auto Camera = GetCamera();

		glm::vec3 CameraTarget = std::get<0>(Camera) + std::get<1>(Camera);
		glm::mat4 view = glm::lookAt(std::get<0>(Camera), CameraTarget, std::get<2>(Camera));
		unsigned int viewloc = glad_glGetUniformLocation(ShaderProgram, "view");
		glUniformMatrix4fv(viewloc, 1, GL_FALSE, glm::value_ptr(view));
		
		// Projection::
		glm::mat4 projection = glm::perspective(glm::radians(90.0f), 3.0f / 1.0f, 0.5f, 100.0f);
		unsigned int projectionLoc = glad_glGetUniformLocation(ShaderProgram, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}


	glfwTerminate();
	return 0;
}