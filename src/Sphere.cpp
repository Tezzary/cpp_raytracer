#include "Sphere.h"
#include "Ray.h"

#include <cmath>

Hit Sphere::getIntersection(Ray ray) {
    std::array<float, 3> difference = {
        ray.position[0] - position[0],
        ray.position[1] - position[1],
        ray.position[2] - position[2]
    };
    float a = ray.direction[0] * ray.direction[0] + ray.direction[1] * ray.direction[1] + ray.direction[2] * ray.direction[2];
    float b = 2 * (ray.direction[0] * difference[0] + ray.direction[1] * difference[1] + ray.direction[2] * difference[2]);
    float c = difference[0] * difference[0] + difference[1] * difference[1] + difference[2] * difference[2] - radius * radius;

    float discriminant = b * b - 4 * a * c;

    if(discriminant < 0) {
        return Hit::emptyHit();
    }
    
    float t = (-b - sqrt(discriminant)) / (2 * a);

    if(t < 0.00001) {
        return Hit::emptyHit();
    }

    Hit hit;
    hit.t = t;
    hit.sphere = this;
    hit.position = {
        ray.position[0] + ray.direction[0] * t,
        ray.position[1] + ray.direction[1] * t,
        ray.position[2] + ray.direction[2] * t
    };
    hit.normal = {
        hit.position[0] - position[0],
        hit.position[1] - position[1],
        hit.position[2] - position[2]
    };

    return hit;
}