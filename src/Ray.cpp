#include "Ray.h"

#include<cmath>

std::array<float, 3> randomCircleSurfacePoint(std::normal_distribution<>& distribution, std::mt19937& gen) {
    float x = distribution(gen);
    float y = distribution(gen);
    float z = distribution(gen);
    float length = sqrt(x * x + y * y + z * z);
    return {x / length, y / length, z / length};
}
std::array<float, 3> Ray::trace(std::normal_distribution<>& distribution, std::mt19937& gen, std::vector<Sphere>& spheres, int bounces) {
    std::array<float, 3> accumulatedLight;
    accumulatedLight.fill(0);

    for(int i = 0; i < bounces; ++i) {
        Hit closestHit = Hit::emptyHit();

        for(Sphere& sphere : spheres) {
            Hit hit = sphere.getIntersection(*this);
            if(hit.t == -1) {
                continue;
            }
            if(closestHit.t == -1 || hit.t < closestHit.t) {
                closestHit = hit;
            }
        }

        if(closestHit.t == -1) {
            return accumulatedLight;
        }

        position = closestHit.position;

        direction = randomCircleSurfacePoint(distribution, gen);

        float dot = direction[0] * closestHit.normal[0] + direction[1] * closestHit.normal[1] + direction[2] * closestHit.normal[2];
        if(dot < 0) {
            direction[0] *= -1;
            direction[1] *= -1;
            direction[2] *= -1;
        }
        //direction = ???

        color[0] *= (*closestHit.sphere).color[0];
        color[1] *= (*closestHit.sphere).color[1];
        color[2] *= (*closestHit.sphere).color[2];

        accumulatedLight[0] += color[0] * (*closestHit.sphere).light;
        accumulatedLight[1] += color[1] * (*closestHit.sphere).light;
        accumulatedLight[2] += color[2] * (*closestHit.sphere).light;
    }
    return accumulatedLight;
}