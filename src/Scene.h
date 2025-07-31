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
        
        Scene(const char* filename, int width, int height, float fov);
        void renderScene();
};