#pragma once

#include "Sphere.h"

#include<array>
#include<vector>
#include<random>

class Sphere;

class Ray {
public:
    std::array<float, 3> position;
    std::array<float, 3> direction;
    std::array<float, 3> color;
    
    std::array<float, 3> trace(std::normal_distribution<>& distribution, std::mt19937& gen, std::vector<Sphere>& spheres, int bounces);
};