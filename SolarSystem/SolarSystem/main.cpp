#include<glad/glad.h>
#include<iostream>
#include<tuple>
#include<GLFW/glfw3.h>

#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include"Shader.h"
#include"Movement.h"
#include"model.h"
#include"skybox.h"



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
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, Mouse_Callback);


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) // Load GLAD to use opengl Function .
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);



	//<---------------------------------------------------------------Dependecies------------------------------------------>
	// Shaders::
	unsigned int SunShaderProgram = SunShaderCompilation();
	unsigned int PlanetShaderProgram = PalnetShaderCompilation();
	// Camera::
	auto Camera = GetCamera();
	// Sky-Box::
	unsigned int SkyVAO = Buffer_setUp();
	unsigned int SkyShaderProgram = SkyBoxShaderProgram();
	//Sun-Texture
	unsigned int SunTex = SunTextureLoader();



	//<--------------------------------------------------------Model-Loading------------------------------------------------------>
	std::vector<Mesh> myModel = LoadModel("D:/GL/Models/Sun/sun.fbx");

	// loading checkup::
	if (myModel.empty()) {
		std::cout << "Model failed to load or is empty.\n";
	}

	//------------------------------------------------------------SkyBox-Loading-------------------------------------------------->
	std::vector<std::string> faces =
	{
		"D:/GL/SkyBox/right.png",
		"D:/GL/SkyBox/left.png",
		"D:/GL/SkyBox/top.png",
		"D:/GL/SkyBox/bottom.png",
		"D:/GL/SkyBox/front.png",
		"D:/GL/SkyBox/back.png"

	};
	unsigned int SkyBox = LoadSkyBox(faces);



	//<------------------------------------------Planet-Configuration-------------------------------------->

	glUseProgram(PlanetShaderProgram);

	//Light Properties:--
	glm::vec3 lightColor;
	lightColor.x = 0.678f;
	lightColor.y = 0.847f;
	lightColor.z = 0.902f;

	glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // decrease the influence
	glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
	glUniform3fv(glad_glGetUniformLocation(PlanetShaderProgram, "light.ambient"), 1, glm::value_ptr(ambientColor));
	glUniform3fv(glad_glGetUniformLocation(PlanetShaderProgram, "light.diffuse"), 1, glm::value_ptr(diffuseColor));
	glUniform3fv(glad_glGetUniformLocation(PlanetShaderProgram, "light.specular"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));


	// material properties:--
	glUniform3fv(glad_glGetUniformLocation(PlanetShaderProgram, "material.ambient"), 1, glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.31f)));
	glUniform3fv(glad_glGetUniformLocation(PlanetShaderProgram, "material.diffuse"), 1, glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.31f)));
	glUniform3fv(glad_glGetUniformLocation(PlanetShaderProgram, "material.specular"), 1, glm::value_ptr(glm::vec3(0.5f, 0.5f, 0.5f)));
	glUniform1f(glad_glGetUniformLocation(PlanetShaderProgram, "material.shininess"), 32.0f);

	glUniform3fv(glGetUniformLocation(PlanetShaderProgram, "viewPos"), 1, glm::value_ptr(std::get<0>(Camera)));


	//-----------------------------------------------------Render Loop-----------------------------------------------------------
	float lastFrame = 0.0f;
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		KeyboardInput(window,deltaTime);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//<----------------------------------------------View/Projection----------------------------------->
		glm::vec3 CameraTarget = std::get<0>(Camera) + std::get<1>(Camera);
		glm::mat4 view = glm::lookAt(std::get<0>(Camera), CameraTarget, std::get<2>(Camera));

		glm::mat4 projection = glm::perspective(glm::radians(90.0f), 3.0f / 1.0f, 0.5f, 100.0f);


		glUseProgram(PlanetShaderProgram); 
		//Model:--
		glm::mat4 PlanetModel(1.0f);
		float Orbit = 3.0f;
		//PlanetModel = glm::translate(PlanetModel, glm::vec3(0.0f, 0.0f, -3.0f));



		//---------------------------------------------------------------------------------------------------------------------
		//---------------------------------------------------------------------------------------------------------------------

		/*
			"Generally, orbits are simulated using Kepler's laws."
		*/

		float semi_major_axis = 35.0f;
		float semi_minnor_axis = 20.0f;

		float theta = glfwGetTime() * Orbit;

		float focus = sqrt(pow(semi_major_axis, 2) - pow(semi_minnor_axis, 2));

		float x_axis = semi_major_axis * cos(theta);
		//x_axis = x_axis - focus;
		float z_axis = semi_minnor_axis * sin(theta);

		// Translate whole orbit to place focus (sun) at (0, 0, -3)
		glm::vec3 orbitOffset = glm::vec3(-focus, 0.0f, -3.0f);

		glm::vec3 planetPos = glm::vec3(x_axis, 0.0f, z_axis) + glm::vec3(0.0f, 0.0f, -3.0f);
		//----------------------------------------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------------------------------------
		
		PlanetModel = glm::translate(PlanetModel, planetPos); // Orbit 
		PlanetModel = glm::scale(PlanetModel, glm::vec3(0.2f)); // size of planets

		glm::vec3 lightPos = glm::vec3(PlanetModel[3]);

		unsigned int PlanetmModelloc = glad_glGetUniformLocation(PlanetShaderProgram, "model");
		glUniformMatrix4fv(PlanetmModelloc, 1, GL_FALSE, glm::value_ptr(PlanetModel));

		//View and Projection uniform to LightShaderProgram:--
		unsigned int PlanetViewloc = glad_glGetUniformLocation(PlanetShaderProgram, "view");
		glUniformMatrix4fv(PlanetViewloc, 1, GL_FALSE, glm::value_ptr(view));

		unsigned int PlanetProjectionLoc = glad_glGetUniformLocation(PlanetShaderProgram, "projection");
		glUniformMatrix4fv(PlanetProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Draw Planet
		for (size_t i = 0; i < myModel.size(); i++)
		{
			Mesh& mesh = myModel[i];
			glBindVertexArray(mesh.VAO);
			glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}

		glUseProgram(PlanetShaderProgram);
		glUniform3fv(glad_glGetUniformLocation(PlanetShaderProgram, "light.position"), 1, glm::value_ptr(lightPos));

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


		//<--------------------------------------------------------------Sun-Configuration------------------------->

		glUseProgram(SunShaderProgram);
		glm::mat4 SunModel = glm::mat4(1.0f);
		SunModel = glm::translate(SunModel, glm::vec3(0.0f, 0.0f, -3.0f));
		unsigned int SunModelLoc = glad_glGetUniformLocation(SunShaderProgram, "model");
		glUniformMatrix4fv(SunModelLoc, 1, GL_FALSE, glm::value_ptr(SunModel));

		//View and Projection uniform to LightShaderProgram:--
		unsigned int Sunviewloc = glad_glGetUniformLocation(SunShaderProgram, "view");
		glUniformMatrix4fv(Sunviewloc, 1, GL_FALSE, glm::value_ptr(view));


		unsigned int SunProjectionLoc = glad_glGetUniformLocation(SunShaderProgram, "projection");
		glUniformMatrix4fv(SunProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		//Draw Object:

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, SunTex);
		glUniform1i(glGetUniformLocation(SunShaderProgram, "tex"),0);
		for (size_t i = 0; i < myModel.size(); i++)
		{
			Mesh& mesh = myModel[i];
			glBindVertexArray(mesh.VAO);
			glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
		
		//SkyBox-Setup::
		glm::mat4 viewSky = glm::mat4(glm::mat3(view));
		glDepthFunc(GL_LEQUAL);
		glUseProgram(SkyShaderProgram);
		glUniformMatrix4fv(glGetUniformLocation(SkyShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(viewSky));
		glUniformMatrix4fv(glGetUniformLocation(SkyShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// Draw skybox
		glBindVertexArray(SkyVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, SkyBox);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthFunc(GL_LESS);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}


	glfwTerminate();
	return 0;
}