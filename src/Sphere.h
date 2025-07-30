#pragma once

#include "Hit.h"
#include "Ray.h"

class Hit;

class Sphere {
    public:
        std::array<float, 3> position;
        std::array<float, 3> color;
        float smoothness;
        float radius;

        Hit getIntersection(Ray ray);
};