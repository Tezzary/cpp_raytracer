#pragma once

#include "Hit.h"
#include "Ray.h"

class Hit;
class Ray;

class Sphere {
    public:
        std::array<float, 3> position;
        std::array<float, 3> color;
        float light;
        float smoothness;
        float radius;
        Sphere(float radius, std::array<float, 3> position, std::array<float, 3> color, float light, float smoothness);
        Hit getIntersection(Ray ray);
};