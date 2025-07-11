#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<vector>

#define STB_IMAGE_IMPLEMENTATION
#include<STB/stb_image.h>
#include"Shader.h"

char const* SkyBoxVertexSource = R"(
#version 330 core
layout(location = 0) in vec3  aPos;

uniform mat4 view;
uniform mat4 projection;

out vec3 TexCoords;

void main()
{
	TexCoords = aPos;
	vec4 pos = projection * view *vec4(aPos , 1.0f);
	gl_Position = vec4(pos.x , pos.y , pos.w , pos.w);
}

)";

char const* SkyBoxFragmentSource = R"(
#version 330 core

in vec3 TexCoords;
uniform samplerCube skybox;

out vec4 FragColor;

void main()
{
  FragColor = texture(skybox, TexCoords);	
}

)";

float skyboxVertices[] = {
	-1.0f,  1.0f, -1.0f,  -1.0f, -1.0f, -1.0f,   1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,   1.0f,  1.0f, -1.0f,  -1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,  -1.0f, -1.0f, -1.0f,  -1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,  -1.0f,  1.0f,  1.0f,  -1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,   1.0f, -1.0f,  1.0f,   1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,   1.0f,  1.0f, -1.0f,   1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,  -1.0f,  1.0f,  1.0f,   1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,   1.0f, -1.0f,  1.0f,  -1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,   1.0f,  1.0f, -1.0f,   1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,  -1.0f,  1.0f,  1.0f,  -1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,  -1.0f, -1.0f,  1.0f,   1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,  -1.0f, -1.0f,  1.0f,   1.0f, -1.0f,  1.0f
};

unsigned int skyboxIndices[] = {
    // Back face
    0, 1, 2,
    2, 3, 0,

    // Left face
    5, 1, 0,
    0, 4, 5,

    // Right face
    2, 6, 7,
    7, 3, 2,

    // Front face
    5, 4, 7,
    7, 6, 5,

    // Top face
    0, 3, 7,
    7, 4, 0,

    // Bottom face
    1, 5, 6,
    6, 2, 1
};


unsigned int  Buffer_setUp()
{
    unsigned int SkyVAO{};
    glGenVertexArrays(1, &SkyVAO);
    glBindVertexArray(SkyVAO);

    unsigned int SkyVBO{};
    glGenBuffers(1, &SkyVBO);
    glBindBuffer(GL_ARRAY_BUFFER, SkyVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);


    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    
    //Disabling Binding::
    glBindVertexArray(0);

    return SkyVAO;
}


unsigned int SkyBoxShaderProgram()
{
	unsigned int SkyBoxVertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(SkyBoxVertexShader, 1, &SkyBoxVertexSource, NULL);
	glCompileShader(SkyBoxVertexShader);

	unsigned int SkyBoxFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(SkyBoxFragmentShader, 1, &SkyBoxFragmentSource, NULL);
	glCompileShader(SkyBoxFragmentShader);

	//Linking::
	unsigned int SkyBoxProgram = glCreateProgram();
	glAttachShader(SkyBoxProgram, SkyBoxVertexShader);
	glAttachShader(SkyBoxProgram, SkyBoxFragmentShader);
	glLinkProgram(SkyBoxProgram);

    glUseProgram(SkyBoxProgram);
    glUniform1i(glGetUniformLocation(SkyBoxProgram, "skybox"), 0);

	// Delete-Shader::
	glDeleteShader(SkyBoxVertexShader);
	glDeleteShader(SkyBoxFragmentShader);


	return SkyBoxProgram;
}


unsigned int LoadSkyBox(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            stbi_set_flip_vertically_on_load(false);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i ,0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;

}


unsigned int SunTextureLoader()
{
    unsigned int textureID0;
    glGenTextures(1, &textureID0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID0);

    //Load-image::
    int width, height, nrChannels;
    unsigned char* SunTexture = stbi_load("D:/GL/Models/sun/suncyl1.jpg", &width, &height, &nrChannels, 0);
    if (SunTexture)
    {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, SunTexture);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed! to load Sun Texture " << std::endl;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(SunTexture);
    return textureID0;
}

