#include<iostream>
#include<fstream>

const int WIDTH = 800;
const int HEIGHT = 600;

class ray {
public:
    float position[3];
    float direction[3];
    float color[3];
};
class sphere {
    public:
        float position[3];
        float color[3];
        float smoothness;
        float radius;
};
int main() {
    std::ofstream MyFile("output.ppm");
    MyFile << "P3" << std::endl;
    MyFile << WIDTH << " " << HEIGHT << std::endl;
    MyFile << "255" << std::endl;
    for(int i = 0; i < HEIGHT; ++i) {
        for(int j = 0; j < WIDTH; ++j) {
            int r = i*256/HEIGHT;
            int g = j*256/WIDTH;
            int b = 255;
            MyFile << r << " " << g << " " << b << std::endl;
        }
    }

    MyFile.close();
    return 0;
}