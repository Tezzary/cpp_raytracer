#pragma once

#include "Sphere.h"
#include<vector>

class Scene {
    std::vector<Sphere> LoadSpheres();
    public: 
        const char* filename;
        int width;
        int height;
        float fov;
        int bounces;
        int samplesPerPixel;
        Scene(const char* filename, int width, int height, float fov, int bounces, int samplesPerPixel);
        std::array<float, 3> tracePixel(int i, int j, float ratio);
        void deployChunk(int chunkI, int chunkJ, std::vector<std::vector<std::array<float, 3>>>& image, float ratio);
        void renderScene();
};