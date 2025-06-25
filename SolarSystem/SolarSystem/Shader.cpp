#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"Shader.h"

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"layout (location = 2) in vec2 aTex;\n"


"out vec3 OurColor;\n"
"out vec2 TexCoords;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"

"void main()\n"
"{\n"
"		 OurColor = aColor;\n"
"        gl_Position = projection * view * model * vec4(aPos,1.0f);\n"
"		 TexCoords = aTex;\n"

"}\0";

const char* fragmentShaderSource = "#version 330 core\n"

"out vec4 FragColor;\n"
"in vec3 OurColor;\n"
"in vec2 TexCoords;\n"
"uniform sampler2D texture1;\n"
"uniform sampler2D texture2;\n"


"void main()\n"
"{\n"
"		FragColor = vec4(0.4f , 0.5, 0.23, 1.0f);\n"
"}\n\0";


unsigned int ShaderCompilation(){
	//Shader-Compilation:--
	unsigned int VertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(VertexShader);

	unsigned int FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(FragmentShader);

	// Compilation-Check
	int success{};
	char infolog[100];

	// For vertex Shader
	glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &success); // check if shader is compile successfully then store 1 in "success".
	if (!success)
	{
		glGetShaderInfoLog(VertexShader, 512, NULL, infolog); // store the error massages in infolog.
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION" << std::endl;
		std::cout << infolog << std::endl;
	}

	// for vertex Shader
	glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(FragmentShader, 512, NULL, infolog); // store the error massages in infolog.
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION" << std::endl;
		std::cout << infolog << std::endl;
	}

	// Linling shaders
	unsigned ShaderProgram = glCreateProgram();
	glAttachShader(ShaderProgram, VertexShader);
	glAttachShader(ShaderProgram, FragmentShader);
	glLinkProgram(ShaderProgram);

	// Delete the shaders
	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);

	return ShaderProgram;
}