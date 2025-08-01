#include "Scene.h"

#include<fstream>
#include<cmath>
#include<iostream>
#include<random>

//constructor
Scene::Scene(const char* filename, int width, int height, int threads, float fov, int bounces, int samplesPerPixel) : filename(filename), width(width), height(height), threads(threads), fov(fov), bounces(bounces), samplesPerPixel(samplesPerPixel) {}

std::ofstream createFile(const char* filename, int width, int height) {
    std::ofstream imageFile(filename);
    imageFile << "P3" << std::endl;
    imageFile << width << " " << height << std::endl;
    imageFile << "255" << std::endl;
    return imageFile;
}
void writePixel(std::ofstream& imageFile, std::array<float, 3> color) {
    imageFile << color[0] * 255.0 << " " << color[1] * 255.0 << " " << color[2] * 255.0 << std::endl;
}

std::vector<Sphere> loadSpheres() {
    std::vector<Sphere> spheres;

    for(int i = 0; i < 5; ++i) {
        Sphere sphere = Sphere(8, {(float)(-36 + i * 18), 0, 0}, {1, 0, i/5.0f}, 0, 0);
        spheres.push_back(sphere);
    }
    //ground
    Sphere sphere = Sphere(1000, {0, 1008, 10}, {1, 1, 1}, 0, 0);
    spheres.push_back(sphere);

    //light behind balls
    sphere = Sphere(50, {-50, -25, 100}, {1, 1, 1}, 3, 0);
    spheres.push_back(sphere);
    //light near balls
    sphere = Sphere(3, {-20, 5, -12}, {1, 1, 1}, 1, 0);
    spheres.push_back(sphere);
    //light behind camera
    sphere = Sphere(50, {-50, -25, -110}, {1, 1, 1}, 0.7, 0);
    spheres.push_back(sphere);

    return spheres;
}

void Scene::renderScene() {
    
    std::vector<Sphere> spheres = loadSpheres();

    std::mt19937 gen(123); 
    //random seed
    //std::random_device rd;
    //std::mt19937 gen(rd()); 

    std::normal_distribution<> disribution(0, 1);

    float ratio = (float)height / (float)width;
    float fovRadians = fov * M_PI / 180;

    std::vector<std::vector<std::array<float, 3>>> image;
    for(int i = 0; i < height; ++i) {
        std::vector<std::array<float, 3>> row;
        for(int j = 0; j < width; ++j) {
            std::array<float, 3> color_storage;
            color_storage.fill(0);
            row.push_back(color_storage);
        }
        image.push_back(row);
    }

    for(int i = 0; i < height; ++i) {

        for(int j = 0; j < width; ++j) {
            float x = sin((((float)j / (float)width - 0.5)) * fovRadians);
            float y = sin(((float)i / (float)height - 0.5) * ratio * fovRadians);
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

            float r = totalColor[0];
            float g = totalColor[1];
            float b = totalColor[2];

            image[i][j][0] = r;
            image[i][j][1] = g;
            image[i][j][2] = b;

        }
    }
    std::ofstream imageFile = createFile(filename, width, height);
    for(int i = 0; i < height; ++i) {
        for(int j = 0; j < width; ++j) {
            writePixel(imageFile, image[i][j]);
        }
    }
        
    imageFile.close();
}