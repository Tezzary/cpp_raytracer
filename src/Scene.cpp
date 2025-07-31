#include "Scene.h"

#include<fstream>
#include<cmath>
#include<iostream>
#include<random>

//constructor
Scene::Scene(const char* filename, int width, int height, float fov, int bounces, int samplesPerPixel) : filename(filename), width(width), height(height), fov(fov), bounces(bounces), samplesPerPixel(samplesPerPixel) {}

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

    for(int i = 0; i < 5; ++i) {
        Sphere sphere = Sphere(8, {(float)(-36 + i * 18), 0, 0}, {1, 0, i/5.0f}, 0, 0);
        spheres.push_back(sphere);
    }
    Sphere sphere = Sphere(1000, {0, 1008, 10}, {0.5, 0.5, 0}, 0, 0);
    spheres.push_back(sphere);
    sphere = Sphere(50, {-50, -25, 100}, {1, 1, 1}, 3, 0);
    spheres.push_back(sphere);

    return spheres;
}

void Scene::renderScene() {

    std::ofstream imageFile = createFile(filename, width, height);

    float ratio = (float)height / (float)width;

    std::vector<Sphere> spheres = loadSpheres();

    float scale = tan(fov * M_PI / 180 / 2);

    std::mt19937 gen(123); 
    //random seed
    //std::random_device rd;
    //std::mt19937 gen(rd()); 

    std::normal_distribution<> disribution(0, 1);

    for(int i = 0; i < height; ++i) {
        for(int j = 0; j < width; ++j) {
            float x = sin(2 * ((float)j / (float)width - 0.5) * scale);
            float y = sin(2 * ((float)i / (float)height - 0.5) * ratio * scale);
            float z = 1;

            std::array<float, 3> totalColor;
            totalColor.fill(0);

            Ray ray;

            for(int i = 0; i < samplesPerPixel; ++i) {
                ray.position = {0, 0, -50};
                ray.direction = {x, y, z};
                ray.color = {1, 1, 1};

                std::array<float, 3> color = ray.trace(disribution, gen, spheres, bounces);

                totalColor[0] += color[0];
                totalColor[1] += color[1];
                totalColor[2] += color[2];
            }

            totalColor[0] /= samplesPerPixel;
            totalColor[1] /= samplesPerPixel;
            totalColor[2] /= samplesPerPixel;
            

            //rainbow gradient background if not hit
            // int r = i*256/height;
            // int g = j*256/width;
            // int b = 255;

            int r = totalColor[0] * 255.0;
            int g = totalColor[1] * 255.0;
            int b = totalColor[2] * 255.0;

            writePixel(imageFile, r, g, b);
        }
    }
    imageFile.close();
}