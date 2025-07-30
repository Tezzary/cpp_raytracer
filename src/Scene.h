#pragma once

class Scene {
    public: 
        const char* filename;
        int width;
        int height;
        float fov;
        
        Scene(const char* filename, int width, int height, float fov);
        void renderScene();
};