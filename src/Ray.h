#pragma once

#include<array>

class Ray {
public:
    std::array<float, 3> position;
    std::array<float, 3> direction;
    std::array<float, 3> color;
};