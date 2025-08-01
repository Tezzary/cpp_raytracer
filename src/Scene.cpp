#include "Scene.h"
#include "config.h"

#include<fstream>
#include<cmath>
#include<iostream>
#include<random>
#include<thread>
#include<chrono>
#include <atomic>

//constructor
Scene::Scene(const char* filename, int width, int height, float fov, int bounces, int samplesPerPixel) : filename(filename), width(width), height(height), fov(fov*M_PI/180), bounces(bounces), samplesPerPixel(samplesPerPixel) {}

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
std::array<float, 3> Scene::tracePixel(int i, int j, float ratio) {
    std::vector<Sphere> spheres = loadSpheres();

    //std::mt19937 gen(123); 
    //random seed
    std::random_device rd;
    std::mt19937 gen(rd()); 

    std::normal_distribution<> disribution(0, 1);

    float x = sin((((float)j / (float)width - 0.5)) * fov);
    float y = sin(((float)i / (float)height - 0.5) * ratio * fov);
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

    return totalColor;
}

void Scene::deployChunk(int chunkI, int chunkJ, std::vector<std::vector<std::array<float, 3>>>& image, float ratio) {
    std::vector<std::vector<std::array<float, 3>>> temp_image;
    for(int i = 0; i < CHUNK_SIZE; ++i) {
        std::vector<std::array<float, 3>> row;
        for(int j = 0; j < CHUNK_SIZE; ++j) {
            row.push_back(tracePixel(CHUNK_SIZE*chunkI+i, CHUNK_SIZE*chunkJ+j, ratio));
        }
        temp_image.push_back(row);
    }
    for(int i = 0; i < CHUNK_SIZE; ++i) {
        for(int j = 0; j < CHUNK_SIZE; ++j) {
            image[CHUNK_SIZE*chunkI+i][CHUNK_SIZE*chunkJ+j] = temp_image[i][j];
        }
    }
}

struct ThreadJob {
    std::thread thread;
    std::atomic<bool> done = false;
};

void Scene::renderScene() {

    float ratio = (float)height / (float)width;

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

    #if THREADS == 1
    for(int i = 0; i < height; ++i) {
        for(int j = 0; j < width; ++j) {
            image[i][j] = tracePixel(i, j, ratio);
        }
    }
    #else
    int chunksHeight = height / CHUNK_SIZE;
    int chunksWidth = width / CHUNK_SIZE;
    
    std::vector<ThreadJob> threads;
    std::vector<bool> flags;
    for(int i = 0; i < chunksHeight; ++i) {
        for(int j = 0; j < chunksWidth; ++j) {
            while(true) {
                for(int k = (int)threads.size()-1; k >= 0; --k) {
                    ThreadJob& t = threads[k];
                    if(t.done) {
                        t.thread.join();
                        threads.erase(threads.begin() + k);
                    }
                }
                if(threads.size() < THREADS) {
                    ThreadJob thread;
                    thread.done = false;
                    thread.thread = std::thread([this, i, j, &image, ratio, &thread]() {
                        this->deployChunk(i, j, image, ratio);
                        thread.done = true;
                    });
                    threads.push_back(std::move(thread));
                    break;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
    }

    for(int i = 0; i < (int)threads.size(); ++i) {
        threads[i].join();
    }

    #endif

    std::ofstream imageFile = createFile(filename, width, height);
    for(int i = 0; i < height; ++i) {
        for(int j = 0; j < width; ++j) {
            writePixel(imageFile, image[i][j]);
        }
    }
        
    imageFile.close();
}