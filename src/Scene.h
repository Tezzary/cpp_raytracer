#pragma once

#include "Sphere.h"
#include<vector>

class Scene {
    std::vector<Sphere> LoadSpheres();
    public: 
        const char* filename;
        int width;
        int height;
        int threads;
        float fov;
        int bounces;
        int samplesPerPixel;
        Scene(const char* filename, int width, int height, int threads, float fov, int bounces, int samplesPerPixel);
        void renderScene();
};