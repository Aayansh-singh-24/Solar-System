#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"Shader.h"

const char* SunVertex = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
         gl_Position = projection * view * model * vec4(aPos,1.0f);
		 TexCoords = aTexCoords;
}
)";

const char*  SunFragment= R"(
#version 330 core

in vec2 TexCoords;
out vec4 FragColor;


uniform sampler2D tex;


void main()
{
		FragColor = texture(tex,TexCoords);
}
)";


const char* PlanetVertex = R"(
#version 330 core

layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aNormal;
layout(location=2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{

	FragPos = vec3(model * vec4(aPos,1));
	Normal = mat3(transpose(inverse(model))) * aNormal;
	TexCoords = aTexCoords;

	gl_Position = projection * view * model * vec4(aPos,1);

}
)";

const char* PlanetFragment = R"(
#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

struct Material {
	vec3 ambient;
	sampler2D diffuse_tex;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 Position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};



uniform Material material;
uniform Light light;
uniform vec3 viewPos;

out vec4 FragColor;

void main()
{


	// Ambient
	vec3 ambient = light.ambient * material.ambient;

	// Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.Position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse_tex , TexCoords));

	// Specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = spec * light.specular * material.specular;

	// Final color
	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
	//FragColor = texture(material.diffuse_tex, TexCoords);
}

)";


//<-------------------------------------------------------------------------SUN-------------------------------------------------------->
unsigned int SunShaderCompilation(){
	//Shader-Compilation:--
	unsigned int SunVertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(SunVertexShader, 1, &SunVertex, NULL);
	glCompileShader(SunVertexShader);

	unsigned int SunFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(SunFragmentShader, 1, &SunFragment, NULL);
	glCompileShader(SunFragmentShader);

	// Compilation-Check
	int success{};
	char infolog[100];

	// For vertex Shader
	glGetShaderiv(SunVertexShader, GL_COMPILE_STATUS, &success); // check if shader is compile successfully then store 1 in "success".
	if (!success)
	{
		glGetShaderInfoLog(SunVertexShader, 512, NULL, infolog); // store the error massages in infolog.
		std::cout << "ERROR::SUN_SHADER::VERTEX::COMPILATION" << std::endl;
		std::cout << infolog << std::endl;
	}

	// for vertex Shader
	glGetShaderiv(SunFragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(SunFragmentShader, 512, NULL, infolog); // store the error massages in infolog.
		std::cout << "ERROR::SUN_SHADER::FRAGMENT::COMPILATION" << std::endl;
		std::cout << infolog << std::endl;
	}

	// Linling shaders
	unsigned SunShaderProgram = glCreateProgram();
	glAttachShader(SunShaderProgram, SunVertexShader);
	glAttachShader(SunShaderProgram, SunFragmentShader);
	glLinkProgram(SunShaderProgram);

	// Delete the shaders
	glDeleteShader(SunVertexShader);
	glDeleteShader(SunFragmentShader);

	return SunShaderProgram;
}

//<--------------------------------------------------------------------------PLANET-------------------------------------------------------->
unsigned int PalnetShaderCompilation() {
	//Shader-Compilation:--
	unsigned int PlanetVertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(PlanetVertexShader, 1, &PlanetVertex, NULL);
	glCompileShader(PlanetVertexShader);

	unsigned int PlanetFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(PlanetFragmentShader, 1, &PlanetFragment, NULL);
	glCompileShader(PlanetFragmentShader);

	// Compilation-Check
	int success{};
	char infolog[100];

	// For vertex Shader
	glGetShaderiv(PlanetVertexShader, GL_COMPILE_STATUS, &success); // check if shader is compile successfully then store 1 in "success".
	if (!success)
	{
		glGetShaderInfoLog(PlanetVertexShader, 512, NULL, infolog); // store the error massages in infolog.
		std::cout << "ERROR::PLANET_SHADER::VERTEX::COMPILATION" << std::endl;
		std::cout << infolog << std::endl;
	}

	// for vertex Shader
	glGetShaderiv(PlanetFragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(PlanetFragmentShader, 512, NULL, infolog); // store the error massages in infolog.
		std::cout << "ERROR::PLANET_SHADER::FRAGMENT::COMPILATION" << std::endl;
		std::cout << infolog << std::endl;
	}

	// Linling shaders
	unsigned PlanetShaderProgram = glCreateProgram();
	glAttachShader(PlanetShaderProgram, PlanetVertexShader);
	glAttachShader(PlanetShaderProgram, PlanetFragmentShader);
	glLinkProgram(PlanetShaderProgram);

	// Delete the shaders
	glDeleteShader(PlanetVertexShader);
	glDeleteShader(PlanetFragmentShader);

	return PlanetShaderProgram;
}