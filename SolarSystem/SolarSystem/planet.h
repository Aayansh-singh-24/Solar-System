#pragma once
//Include:

#include <GLFW/glfw3.h>
#include <cmath>
#include <vector>
#include <iostream>

//Dependencies:

// Struct to represent a planet
struct Planet {
    float semi_major_axis;
    float semi_minor_axis;
    float revolvingSpeed;
    float initialThetaOffset;

    float x, y, z; // Position in 3D space
};

//Function
void updatePlanetPosition(Planet& planet, float time);
