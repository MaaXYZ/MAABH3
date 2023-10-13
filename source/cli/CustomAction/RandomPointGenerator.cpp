#include "RandomPointGenerator.h"

RandomPointGenerator::RandomPointGenerator() : rng_(std::random_device{}()) {}

void RandomPointGenerator::generate_point(int32_t x, int32_t y, int32_t w, int32_t h, int32_t& rx, int32_t& ry)
{
    std::uniform_int_distribution<int32_t> uni_x(x, x + w);
    std::uniform_int_distribution<int32_t> uni_y(y, y + h);
    rx = uni_x(rng_);
    ry = uni_y(rng_);
}