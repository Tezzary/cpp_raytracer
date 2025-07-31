#include "Scene.h"

const int WIDTH = 800;
const int HEIGHT = 600;
const float FOV = 90; // degrees
const int BOUNCES = 5;
const int SAMPLESPERPIXEL = 1000;

const char* FILENAME = "output.ppm";

int main() {
    Scene scene(FILENAME, WIDTH, HEIGHT, FOV, BOUNCES, SAMPLESPERPIXEL);
    scene.renderScene();
    return 0;
}