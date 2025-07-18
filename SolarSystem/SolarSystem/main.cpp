#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>
#include<tuple>
#include<chrono>
#include<iomanip>

#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <AL/al.h>
#include <AL/alc.h>

#include"Shader.h"
#include"Movement.h"
#include"model.h"
#include"skybox.h"
#include"planet.h"



int width = 1024;
int height = 768;
char name[100] = "Solar System";

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//<-----------------------------------------------------------------FPS-COUNTER--------------------------------------------------------------->
// Function to calculate and display FPS
void calculateAndDisplayFPS(float& lastTime, int& frameCount, float& fps) {
	// Get current time
	static auto startTime = std::chrono::high_resolution_clock::now();
	auto currentTime = std::chrono::high_resolution_clock::now();

	// Calculate time difference
	float timeDifference = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	// Calculate FPS
	frameCount++;
	if (timeDifference >= 1.0f) {
		fps = static_cast<float>(frameCount) / timeDifference;
		startTime = currentTime;
		frameCount = 0;
	}

	// Display FPS (example using std::cout)
	std::cout << "FPS: " << std::fixed << std::setprecision(2) << fps << std::endl;
}



int main() {



	//FPS-Counts::
	int frameCount = 0;
	float fps = 0.0f;
	float lastTime = 0.0f;
	
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



	//<---------------------------------------------------------------Dependecies---------------------------------------------->
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


	//<-----------------------------------------------------------------------Planet-Texture-Dependencies---------------------------------------------------->
	std::vector<unsigned int> PlanetTexture =
	{
		TextureLoader("D:/GL/Texture/mercury.jpg"), //Mercury
		TextureLoader("D:/GL/Texture/venus.jpg"), //Venus
		TextureLoader("D:/GL/Texture/earth.jpg"), //Earth
		TextureLoader("D:/GL/Texture/mars.jpg"), //Mars
		TextureLoader("D:/GL/Texture/jupiter.jpg"), //Jupitar
		TextureLoader("D:/GL/Texture/saturn.jpg"), //Saturn
		TextureLoader("D:/GL/Texture/uranus.jpg"), //Uranus
		TextureLoader("D:/GL/Texture/neptune.jpg") //Neptune
	};
	
	//Scalling For Realisiam
	std::vector<float> planetScales =
	{
			0.05f, // Mercury
			0.07f, // Venus
			0.08f, // Earth
			0.06f, // Mars
			0.2f, // Jupiter
			0.13f, // Saturn
			0.11f, // Uranus
			0.10f  // Neptune
	};
	



	//<--------------------------------------------------------Model-Loading------------------------------------------------------>
	std::vector<Mesh> myModel = LoadModel("D:/GL/Models/model.fbx");

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


	//<-------------------------------------------------------Planets-Properties----------------------------------------------------------------------------->
	std::vector<Planet> planets = {
		{18.0f, 17.5f, 0.6f, 7.0f},   // Mercury
		{25.0f, 24.0f, 0.5f, 15.0f},  // Venus
		{33.0f, 32.0f, 0.4f, 20.0f},  // Earth
		{42.0f, 40.0f, 0.3f, 29.0f},  // Mars
		{55.0f, 53.0f, 0.25f, 36.0f}, // Jupiter
		{65.0f, 63.0f, 0.2f, 42.0f},  // Saturn
		{78.0f, 76.0f, 0.15f, 53.0f}, // Uranus
		{90.0f, 88.0f, 0.12f, 66.0f}  // Neptune
	};


	//<------------------------------------------------------------Planet-Configuration----------------------------------------------------------------------------->

	glUseProgram(PlanetShaderProgram);

	//Light Properties:--
	glm::vec3 lightColor;
	lightColor.x = 0.0f;     
	lightColor.y = 0.2f;     
	lightColor.z = 0.6f;  
	
	glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // decrease the influence
	glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
	glUniform3fv(glad_glGetUniformLocation(PlanetShaderProgram, "light.ambient"), 1, glm::value_ptr(ambientColor));
	glUniform3fv(glad_glGetUniformLocation(PlanetShaderProgram, "light.diffuse"), 1, glm::value_ptr(glm::vec3(0.8f,0.8f,0.6f)));
	glUniform3fv(glad_glGetUniformLocation(PlanetShaderProgram, "light.specular"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));



	glUniform3fv(glad_glGetUniformLocation(PlanetShaderProgram, "material.ambient"), 1, glm::value_ptr(glm::vec3(1.0f)));
	//glUniform3fv(glad_glGetUniformLocation(PlanetShaderProgram, "material.diffuse"), 1, glm::value_ptr(glm::vec3( 0.2f,0.4f, 0.6f)));
	glUniform3fv(glad_glGetUniformLocation(PlanetShaderProgram, "material.specular"), 1, glm::value_ptr(glm::vec3(0.5f)));
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

		glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)width / (float)height, 0.5f, 100.0f);


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//<----------------------------------------------------------Planet-Configure---------------------------------------------->
		float time = glfwGetTime();
		for (auto& planet : planets) {
			updatePlanetPosition(planet, time);
		}


		glUseProgram(PlanetShaderProgram);

		for (size_t i = 0; i < planets.size(); i++)
		{
			const Planet& planet = planets[i];

			// Update planet position
			updatePlanetPosition(const_cast<Planet&>(planet), glfwGetTime());

			// Calculate position relative to Sun
			glm::vec3 planetPos = glm::vec3(planet.x, planet.y, planet.z) + glm::vec3(0.0f, 0.0f, -3.0f);

			// Transform (translation + scale)
			glm::mat4 PlanetModel = glm::translate(glm::mat4(1.0f), planetPos);
			PlanetModel = glm::scale(PlanetModel, glm::vec3(planetScales[i])); // Different scales per planet

			// Send matrices to shader
			glUniformMatrix4fv(glGetUniformLocation(PlanetShaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(PlanetModel));
			glUniformMatrix4fv(glGetUniformLocation(PlanetShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(glGetUniformLocation(PlanetShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

			// Bind the unique planet texture
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, PlanetTexture[i]);
			glUniform1i(glGetUniformLocation(PlanetShaderProgram, "material.diffuse_tex"), 0);

			// Draw the planet model
			for (size_t i=0; i<myModel.size(); i++)
			{
				Mesh& mesh = myModel[i];
				glBindVertexArray(mesh.VAO);
				glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
			}
		}


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------



		//<--------------------------------------------------------------Sun-Configuration------------------------------------------------->

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
		
	//<----------------------------------------------------------------SkyBox-Setup::-------------------------------------------------------------->
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