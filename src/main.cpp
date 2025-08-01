#include "Scene.h"

const int WIDTH = 800;
const int HEIGHT = 600;
const float FOV = 100; // degrees
const int BOUNCES = 5;
const int SAMPLESPERPIXEL = 4096;
const int THREADS = 4;
const char* FILENAME = "output.ppm";

int main() {
    Scene scene(FILENAME, WIDTH, HEIGHT, THREADS, FOV, BOUNCES, SAMPLESPERPIXEL);
    scene.renderScene();
    return 0;
}