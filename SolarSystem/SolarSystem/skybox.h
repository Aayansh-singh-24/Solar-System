#pragma once
//Headers:-
#include<vector>
#include<string>
#include<tuple>


//Function::
unsigned int LoadSkyBox(std::vector<std::string> faces);
unsigned int  Buffer_setUp();
unsigned int SkyBoxShaderProgram();
unsigned int SunTextureLoader();
unsigned int EarthTextureLoader();