#include "Scene.h"

const int WIDTH = 800;
const int HEIGHT = 600;
const float FOV = 90; // degrees

const char* FILENAME = "output.ppm";

int main() {
    Scene scene(FILENAME, WIDTH, HEIGHT, FOV);
    scene.renderScene();
    return 0;
}