#include<iostream>
#include<fstream>
#include<cmath>
#include<array>

const int WIDTH = 800;
const int HEIGHT = 600;
const float FOV = 90 * 3.1415926535 / 180.0;

class Sphere;

class Hit {
    public:
        float t;
        Sphere* sphere = nullptr;
        std::array<float, 3> position;
        std::array<float, 3> normal; 

        static Hit emptyHit() {
            Hit hit;
            hit.t = -1;
            return hit;
        }
};

class Ray {
public:
    std::array<float, 3> position;
    std::array<float, 3> direction;
    std::array<float, 3> color;
};

class Sphere {
    public:
        std::array<float, 3> position;
        std::array<float, 3> color;
        float smoothness;
        float radius;

        Hit getIntersection(Ray ray) {
            std::array<float, 3> difference = {
                ray.position[0] - position[0],
                ray.position[1] - position[1],
                ray.position[2] - position[2]
            };
            float a = ray.direction[0] * ray.direction[0] + ray.direction[1] * ray.direction[1] + ray.direction[2] * ray.direction[2];
            float b = 2 * (ray.direction[0] * difference[0] + ray.direction[1] * difference[1] + ray.direction[2] * difference[2]);
            float c = difference[0] * difference[0] + difference[1] * difference[1] + difference[2] * difference[2] - radius * radius;

            float discriminant = b * b - 4 * a * c;

            if(discriminant < 0) {
                return Hit::emptyHit();
            }
            
            float t = (-b - sqrt(discriminant)) / (2 * a);

            if(t < 0.00001) {
                return Hit::emptyHit();
            }

            Hit hit;
            hit.t = t;
            hit.sphere = this;
            hit.position = {
                ray.position[0] + ray.direction[0] * t,
                ray.position[1] + ray.direction[1] * t,
                ray.position[2] + ray.direction[2] * t
            };
            hit.normal = {
                hit.position[0] - position[0],
                hit.position[1] - position[1],
                hit.position[2] - position[2]
            };

            return hit;
        }
};
int main() {
    std::ofstream MyFile("output.ppm");
    MyFile << "P3" << std::endl;
    MyFile << WIDTH << " " << HEIGHT << std::endl;
    MyFile << "255" << std::endl;
    float ratio = (float)HEIGHT / (float)WIDTH;

    Sphere sphere;
    sphere.radius = 2;
    sphere.position = {0, 0, 5};
    sphere.color = {1, 1, 1};

    float scale = tan(FOV / 2);

    for(int i = 0; i < HEIGHT; ++i) {
        for(int j = 0; j < WIDTH; ++j) {
            float x = sin(2 * ((float)j / (float)WIDTH - 0.5) * scale);
            float y = sin(2 * ((float)i / (float)HEIGHT - 0.5) * ratio * scale);
            float z = 1;

            Ray ray;
            ray.position = {0, 0, 0};
            ray.direction = {x, y, z};
            ray.color = {1, 1, 1};

            Hit hit = sphere.getIntersection(ray);

            int r = i*256/HEIGHT;
            int g = j*256/WIDTH;
            int b = 255;

            if(hit.t != -1) {
                r = (int)(*hit.sphere).color[0] * 255.0;
                g = (int)(*hit.sphere).color[1] * 255.0;
                b = (int)(*hit.sphere).color[2] * 255.0;
            }
            MyFile << r << " " << g << " " << b << std::endl;
        }
    }

    MyFile.close();
    return 0;
}