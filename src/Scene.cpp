#include "Scene.h"

#include<fstream>
#include<cmath>
#include <iostream>
using namespace std;

//constructor
Scene::Scene(const char* filename, int width, int height, float fov) : filename(filename), width(width), height(height), fov(fov) {}

std::ofstream createFile(const char* filename, int width, int height) {
    std::ofstream imageFile(filename);
    imageFile << "P3" << std::endl;
    imageFile << width << " " << height << std::endl;
    imageFile << "255" << std::endl;
    return imageFile;
}
void writePixel(std::ofstream& imageFile, int r, int g, int b) {
    imageFile << r << " " << g << " " << b << std::endl;
}

std::vector<Sphere> loadSpheres() {
    std::vector<Sphere> spheres;
    Sphere sphere2 = Sphere(1, {3, 0, 4}, {1, 1, 0}, 0);
    spheres.push_back(sphere2);
    Sphere sphere = Sphere(2, {0, 0, 5}, {1, 1, 1}, 0);
    spheres.push_back(sphere);
    
    return spheres;
}

void Scene::renderScene() {

    std::ofstream imageFile = createFile(filename, width, height);

    float ratio = (float)height / (float)width;

    std::vector<Sphere> spheres = loadSpheres();

    float scale = tan(fov * M_PI / 180 / 2);

    for(int i = 0; i < height; ++i) {
        for(int j = 0; j < width; ++j) {
            float x = sin(2 * ((float)j / (float)width - 0.5) * scale);
            float y = sin(2 * ((float)i / (float)height - 0.5) * ratio * scale);
            float z = 1;

            Ray ray;
            ray.position = {0, 0, 0};
            ray.direction = {x, y, z};
            ray.color = {1, 1, 1};

            Hit closestHit = Hit::emptyHit();

            for(Sphere& sphere : spheres) {
                Hit hit = sphere.getIntersection(ray);
                if(closestHit.t == -1 && hit.t != -1) {
                    closestHit = hit;
                }
                else if(hit.t != -1 && hit.t < closestHit.t) {
                    closestHit = hit;
                }
            }

            int r = i*256/height;
            int g = j*256/width;
            int b = 255;

            if(closestHit.t != -1) {
                r = (int)(*closestHit.sphere).color[0] * 255.0;
                g = (int)(*closestHit.sphere).color[1] * 255.0;
                b = (int)(*closestHit.sphere).color[2] * 255.0;
            }

            writePixel(imageFile, r, g, b);
        }
    }
    imageFile.close();
}