#include"planet.h"

// Function to update a planet's position based on time
 void updatePlanetPosition(Planet& planet, float time)
 {
    // Base theta with offset
    float theta = time * planet.revolvingSpeed + planet.initialThetaOffset;
    
    // Distance from sun at this theta (r)
    float r = (planet.semi_major_axis * planet.semi_minor_axis) / sqrt(pow(planet.semi_minor_axis * cos(theta), 2) + pow(planet.semi_major_axis * sin(theta), 2));
    float c = sqrt(planet.semi_major_axis * planet.semi_major_axis -
        planet.semi_minor_axis * planet.semi_minor_axis);

    // Adjust theta using Kepler's second law approximation (angular velocity ∝ 1 / r^2)
    theta += time * planet.revolvingSpeed / (r * r);



    // Recompute position
    planet.x = planet.semi_major_axis * cos(theta) - c;
    planet.z = planet.semi_minor_axis * sin(theta);
    planet.y = 0.0f; // flat orbit 
 }