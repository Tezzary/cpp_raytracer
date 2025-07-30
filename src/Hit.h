#pragma once

#include "Sphere.h"

#include <array>

class Sphere;

class Hit {
    public:
        float t;
        Sphere* sphere = nullptr;
        std::array<float, 3> position;
        std::array<float, 3> normal; 

        static Hit emptyHit();
};